#include "camera_mods.h"
#include "EqFunctions.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include <thread>
#include <algorithm>
#include <windowsx.h>
#include "StringUtil.h"
float Smooth(float rawDelta, float smoothDelta) {
    return std::lerp(smoothDelta, static_cast<float>(rawDelta), 1.0f / 1.5f);
}
float get_pitch(Vec3 cameraPos, Vec3 targetPos) {
    Vec3 direction = targetPos - cameraPos;
    float horizontalDistance = (float)direction.Length2D();
    float verticalDistance = (float)direction.z;
    float pitch = std::atan2(verticalDistance, horizontalDistance);
    float pitchDegrees = pitch * (180.0f / (float)M_PI);

    // Scale the pitch to fit into the [-128, 128] range
    pitchDegrees *= (128.0f / 90.0f);

    return pitchDegrees;
}

Vec3 calculatePositionBehind(const Vec3& playerHead, float distance, float playerYaw, float pitch) {
    float yaw_rads = playerYaw * (2.0f * (float)M_PI) / 512.0f;
    float yaw_deg = yaw_rads * (180.0f / (float)M_PI);

    // Convert pitch to radians
    float pitch_rads = pitch * ((float)M_PI / 180.0f);

    Vec3 positionBehind;

    // Calculate the position behind based on player's orientation and pitch
    float horizontalDistance = distance * cos(pitch_rads);
    float verticalDistance = distance * sin(pitch_rads);

    static Vec3 last_good;
    // Limit vertical distance to prevent camera from moving too far above the player's head
    if (pitch_rads > -M_PI / 2 && pitch_rads < M_PI / 2) {
        positionBehind.x = playerHead.x - horizontalDistance * cos(yaw_rads);
        positionBehind.y = playerHead.y - horizontalDistance * sin(yaw_rads);
        positionBehind.z = playerHead.z + verticalDistance;
        last_good = positionBehind;
    }
    else {
        // If pitch is too steep, keep the camera level with the player's head
        positionBehind = last_good;
    }

    return positionBehind;
}
bool CameraMods::update_cam()
{
    if (!enabled)
        return false;
    
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_view_actor_entity();
    if (!self)
        return false;
    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
    Vec3 head_pos = Zeal::EqGame::get_view_actor_head_pos();
    Vec3 wanted_pos = calculatePositionBehind(head_pos, current_zoom, zeal_cam_yaw/*self->Heading*/, -zeal_cam_pitch);
    bool rval = Zeal::EqGame::collide_with_world(head_pos, wanted_pos, wanted_pos);
    cam->Position = wanted_pos;
    cam->Heading = zeal_cam_yaw;// self->Heading;
    cam->Pitch = get_pitch(cam->Position, head_pos);
    cam->RegionNumber = Zeal::EqGame::get_region_from_pos(&cam->Position);
    return rval;
}

void CameraMods::mouse_wheel(int delta)
{
    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
    DWORD camera_view = *Zeal::EqGame::camera_view;
    if (delta > 0)
    {
        if (camera_view == Zeal::EqEnums::CameraView::ZealCam)
            update_zoom(-zoom_speed);
    }
    else if (delta < 0)
    {
        if (camera_view == Zeal::EqEnums::CameraView::FirstPerson)
        {
            toggle_zeal_cam(true);
            update_zoom(zoom_speed);
        }
        else if (camera_view == Zeal::EqEnums::CameraView::ZealCam)
           update_zoom(zoom_speed);
    }
}
float toZealPitch(float game_pitch) {
    float scaledAmplitude = (game_pitch / 128.0f) * 90.0f;
    if (scaledAmplitude > 89.99f) {
        scaledAmplitude = 89.99f;
    }
    else if (scaledAmplitude < -89.99f) {
        scaledAmplitude = -89.99f;
    }

    return scaledAmplitude;
}
float toGamePitch(float zeal_pitch) {
    float normalizedAmplitude = zeal_pitch / 90.0f;
    float unscaledAmplitude = normalizedAmplitude * 128.0f;
    if (unscaledAmplitude > 128.0f) {
        unscaledAmplitude = 128.0f;
    }
    else if (unscaledAmplitude < -128.0f) {
        unscaledAmplitude = -128.0f;
    }

    return unscaledAmplitude;
}

void CameraMods::toggle_zeal_cam(bool enabled)
{
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_controlled();
    if (enabled)
    {
        *Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::ZealCam;
        zeal_cam_pitch = toZealPitch(self->Pitch);
        zeal_cam_yaw = self->Heading;
        desired_zoom += zoom_speed;
        mem::set(0x4db8ce, 0x90, 6, original_cam[0] == 0x0 ? original_cam : 0);
    }
    else if (original_cam[0] != 0x0)
    {
        self->Pitch = toGamePitch(zeal_cam_pitch);
        current_zoom = 0;
        if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
            *Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::FirstPerson;
        mem::copy(0x4db8ce, original_cam, 6);
    }
}
void CameraMods::update_zoom(float zoom)
{
    desired_zoom += zoom;
    if (desired_zoom > max_zoom_out)
        desired_zoom = max_zoom_out;
    if (desired_zoom < 0)
        desired_zoom = 0;
    if (update_cam()) //there was a collision
    {
        if (zoom > 0)
        {
            desired_zoom -= zoom;
            update_cam();
        }
    }
}

int handle_mouse_wheel(int delta)
{
    CameraMods* c = ZealService::get_instance()->camera_mods.get();
    DWORD camera_view = *Zeal::EqGame::camera_view;
    if (!Zeal::EqGame::is_mouse_hovering_window() && c->enabled &&  (camera_view == Zeal::EqEnums::CameraView::FirstPerson || camera_view == Zeal::EqEnums::CameraView::ZealCam))
    {
        c->mouse_wheel(delta);
        return 0;
    }
    else
        return ZealService::get_instance()->hooks->hook_map["HandleMouseWheel"]->original(handle_mouse_wheel)(delta);
}


void CameraMods::proc_mouse()
{
    if (enabled && (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam || *Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::FirstPerson))
    {
        static float smoothMouseDeltaX = 0;
        static float smoothMouseDeltaY = 0;
        Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
        DWORD camera_view = *Zeal::EqGame::camera_view;
        Zeal::EqStructures::MouseDelta* delta = (Zeal::EqStructures::MouseDelta*)0x798586;
        Zeal::EqStructures::Entity* self = Zeal::EqGame::get_controlled();
        bool lbutton = *Zeal::EqGame::is_left_mouse_down;
        bool rbutton = *Zeal::EqGame::is_right_mouse_down;
        if (!self)
            return;
        if (rbutton || (camera_view== Zeal::EqEnums::CameraView::ZealCam && lbutton))
        {
            float delta_y = delta->y;
            float delta_x = delta->x;
            smoothMouseDeltaX = Smooth(delta_x * sensitivity_x, smoothMouseDeltaX);
            smoothMouseDeltaY = Smooth(delta_y * sensitivity_y, smoothMouseDeltaY);
            if (fabs(smoothMouseDeltaX) > 5)
                smoothMouseDeltaY /= 2;
            //ZealService::get_instance()->labels_hook->print_debug_info("delta x %f  delta y %f", smoothMouseDeltaX, smoothMouseDeltaY);
            delta->y = 0;
            delta->x = 0;

            if (*(byte*)0x7985E8 && rbutton)
                smoothMouseDeltaY = -smoothMouseDeltaY;

            if (Zeal::EqGame::can_move())
            {

                if (rbutton)
                {
                    
                    if (fabs(smoothMouseDeltaX) > 0.1)
                        self->MovementSpeedHeading = -smoothMouseDeltaX;
                    else
                        self->MovementSpeedHeading = 0;
                    zeal_cam_yaw = self->Heading;
                }
                else
                    zeal_cam_yaw -= smoothMouseDeltaX;
            }



            if (fabs(smoothMouseDeltaY) > 0)
            {
                if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
                {
                    zeal_cam_pitch -= smoothMouseDeltaY;
                    zeal_cam_pitch = std::clamp(zeal_cam_pitch, -89.9f, 89.9f);
                    if (Zeal::EqGame::KeyMods->Shift)
                        self->Pitch = toGamePitch(zeal_cam_pitch);
                }
                else
                {
                    if (Zeal::EqGame::is_view_actor_me())
                    {
                        self->Pitch -= smoothMouseDeltaY;
                        self->Pitch = std::clamp(self->Pitch, -128.f, 128.f);
                    }
                }
            }
        }
        if (camera_view == Zeal::EqEnums::CameraView::ZealCam)
           update_cam();
    }
}


void __fastcall procMouse(int eq, int unused, int a1)
{
    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
    ZealService* zeal = ZealService::get_instance();
    if (eq)
        zeal->camera_mods->eq_ptr = eq;
    zeal->camera_mods->proc_mouse();
    if (eq == 0)
        return;
    else
        zeal->hooks->hook_map["procMouse"]->original(procMouse)(eq, unused, a1);
}

void CameraMods::set_smoothing(bool val)
{
    int camera_view = *Zeal::EqGame::camera_view;
    enabled = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "MouseSmoothing", enabled);
    if (enabled && (camera_view==Zeal::EqEnums::CameraView::ZealCam || camera_view==Zeal::EqEnums::CameraView::ThirdPerson2)) //thirdperson2 is the mouse wheel out normally
        toggle_zeal_cam(enabled);
    else
        toggle_zeal_cam(false);
}

void CameraMods::InterpolateZoom() 
{
    if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
    {
        current_zoom = current_zoom + (desired_zoom - current_zoom) * .3f; //linear interpolation
        if (current_zoom < .5)
            toggle_zeal_cam(false);
    }
}


void CameraMods::handle_binds(int cmd, bool is_down)
{
    DWORD camera_view = *Zeal::EqGame::camera_view;
    if ((enabled && camera_view == Zeal::EqEnums::CameraView::ZealCam) || (enabled && cmd == 19)) //allow zoom out
    {
        if (is_down)
        {
            current_key_cmd = cmd;
           
        }
        else
            current_key_cmd = 0;
    }
    else
    current_key_cmd = 0;
}


void CameraMods::callback_main()
{
    static int prev_view = *Zeal::EqGame::camera_view;
    DWORD camera_view = *Zeal::EqGame::camera_view;

    if (camera_view != Zeal::EqEnums::CameraView::ZealCam && current_key_cmd!=19)
        current_key_cmd = 0;

    if (camera_view == Zeal::EqEnums::CameraView::FirstPerson && current_key_cmd == 19)
    {
        mouse_wheel(-120);
    }

    if (*Zeal::EqGame::is_left_mouse_down)
    {
        if (!lmouse_time)
        {
            hide_cursor = true;
            lmouse_time = GetTickCount64();
        }
        
        if (GetTickCount64() - lmouse_time > 200)
        {
            if (hide_cursor)
            {
                GetCursorPos(&lmouse_cursor_pos);
                mem::write<byte>(0x53edef, 0xEB);
                hide_cursor = false;
            }
            proc_mouse();
            SetCursorPos(lmouse_cursor_pos.x, lmouse_cursor_pos.y);
        }
    }
    else
    {
        if (lmouse_time)
        {
            mem::write<byte>(0x53edef, 0x75);
        }
        lmouse_time = 0;
    }
    if (enabled)
    {
        InterpolateZoom();
        switch (current_key_cmd)
        {
        case 15: //up
            zeal_cam_pitch -= 0.3f;
            if (zeal_cam_pitch <= -89.99f)
                zeal_cam_pitch = -89.99f;
            break;
        case 16: //down
            zeal_cam_pitch += 0.3f;
            if (zeal_cam_pitch >= 89.99f)
                zeal_cam_pitch = 89.99f;
            break;
        case 18: //zoom in
            if (desired_zoom > 0)
                desired_zoom -= .3;
            break;
        case 19: //zoom out
            desired_zoom += .3;
            if (desired_zoom > max_zoom_out)
                desired_zoom = max_zoom_out;
            break;
        }
    }
    else
    {
        current_key_cmd = 0;
    }




    if (prev_view != *Zeal::EqGame::camera_view && enabled)
    {
        if (*Zeal::EqGame::camera_view != Zeal::EqEnums::CameraView::ZealCam)
            toggle_zeal_cam(false);
        else
        {
            toggle_zeal_cam(true);
            desired_zoom = zoom_speed;
        }
    }



    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
    if (!cam)
        return;
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    if (elapsedTime > 0) {
        fps = 1000.0f / elapsedTime;
    }

    sensitivity_x = user_sensitivity_x * (fps / 144.f);
    sensitivity_y = user_sensitivity_y * (fps / 144.f);

    if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
    {
        sensitivity_x = user_sensitivity_x_3rd * (fps / 144.f);
        sensitivity_y = user_sensitivity_y_3rd * (fps / 144.f);
    }

    float current_sens = (float)(*(byte*)0x798b0c);
    float multiplier = current_sens / 4.0f;
    sensitivity_x *= multiplier;
    sensitivity_y *= multiplier;
    lastTime = currentTime;
    if (!*Zeal::EqGame::is_right_mouse_down && Zeal::EqGame::is_in_game() && enabled)
    {
        if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
            update_cam();
    }
    prev_view = *Zeal::EqGame::camera_view;
}

void _fastcall doCharacterSelection(int t, int u)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->camera_mods->toggle_zeal_cam(false);
    zeal->hooks->hook_map["DoCharacterSelection"]->original(doCharacterSelection)(t, u);
}
void CameraMods::LoadSettings(IO_ini* ini)
{

    if (!ini->exists("Zeal", "MouseSmoothing"))
        ini->setValue<bool>("Zeal", "MouseSmoothing", true);
    if (!ini->exists("Zeal", "MouseSensitivityX"))
        ini->setValue<float>("Zeal", "MouseSensitivityX", user_sensitivity_x);
    if (!ini->exists("Zeal", "MouseSensitivityY"))
        ini->setValue<float>("Zeal", "MouseSensitivityY", user_sensitivity_y);

    enabled = ini->getValue<bool>("Zeal", "MouseSmoothing");
    user_sensitivity_x = ini->getValue<float>("Zeal", "MouseSensitivityX");
    user_sensitivity_y = ini->getValue<float>("Zeal", "MouseSensitivityY");

    if (!ini->exists("Zeal", "MouseSensitivityX3rd"))
        ini->setValue<float>("Zeal", "MouseSensitivityX3rd", user_sensitivity_x);
    if (!ini->exists("Zeal", "MouseSensitivityY3rd"))
        ini->setValue<float>("Zeal", "MouseSensitivityY3rd", user_sensitivity_y);

    user_sensitivity_x_3rd = ini->getValue<float>("Zeal", "MouseSensitivityX3rd");
    user_sensitivity_y_3rd = ini->getValue<float>("Zeal", "MouseSensitivityY3rd");

}


CameraMods::CameraMods(ZealService* zeal, IO_ini* ini)
{
    mem::write<byte>(0x53fa50, 06); //allow for strafing whenever in zeal cam
    mem::write<byte>(0x53f648, 06); //allow for strafing whenever in zeal cam
    mem::write<byte>(0x4adcd9, 06); //allow for the camera toggle hotkey to cycle through the new camera
    LoadSettings(ini);

    lastTime = std::chrono::steady_clock::now();
    fps = 0;
    height = 0;
    zeal->main_loop_hook->add_callback([this]() { callback_main();  });
	zeal->hooks->Add("HandleMouseWheel", Zeal::EqGame::EqGameInternal::fn_handle_mouseweheel, handle_mouse_wheel, hook_type_detour);
    zeal->hooks->Add("procMouse", 0x537707, procMouse, hook_type_detour);
    zeal->hooks->Add("DoCharacterSelection", 0x53b9cf, doCharacterSelection, hook_type_detour);

    zeal->commands_hook->add("/zealcam", { "/smoothing" },
        [this](std::vector<std::string>& args) {
            if (args.size() == 2 && StringUtil::caseInsensitive(args[1], "info"))
            {
                Zeal::EqGame::print_chat("camera sensitivity FirstPerson : [% f] [% f] ThirdPerson : [% f] [% f] ", user_sensitivity_x, user_sensitivity_y, user_sensitivity_x_3rd, user_sensitivity_y_3rd);
                return true;
            }
            else if (args.size() == 3) //the first arg is the command name itself
            {
                float x_sens = 0;
                float y_sens = 0;

                if (!StringUtil::tryParse(args[1], &x_sens))
                    return true;
                if (!StringUtil::tryParse(args[2], &y_sens))
                    return true;

               user_sensitivity_x = x_sens;
               user_sensitivity_y = y_sens;
               user_sensitivity_x_3rd = x_sens;
               user_sensitivity_y_3rd = y_sens;
               set_smoothing(true);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX",user_sensitivity_x);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY",user_sensitivity_y);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX3rd",user_sensitivity_x_3rd);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY3rd",user_sensitivity_y_3rd);
                Zeal::EqGame::print_chat("New camera sensitivity [%f] [%f]",user_sensitivity_x,user_sensitivity_y);
            }
            else if (args.size() == 5)
            {
                float x_sens = 0;
                float y_sens = 0;
                float x_sens_3rd = 0;
                float y_sens_3rd = 0;

                if (!StringUtil::tryParse(args[1], &x_sens))
                    return true;
                if (!StringUtil::tryParse(args[2], &y_sens))
                    return true;
                if (!StringUtil::tryParse(args[3], &x_sens_3rd))
                    return true;
                if (!StringUtil::tryParse(args[4], &y_sens_3rd))
                    return true;


                user_sensitivity_x = x_sens;
                user_sensitivity_y = y_sens;
                user_sensitivity_x_3rd = x_sens_3rd;
                user_sensitivity_y_3rd = y_sens_3rd;
               set_smoothing(true);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX",user_sensitivity_x);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY",user_sensitivity_y);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX3rd",user_sensitivity_x_3rd);
                ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY3rd",user_sensitivity_y_3rd);

                Zeal::EqGame::print_chat("New camera sensitivity FirstPerson: [%f] [%f] ThirdPerson: [%f] [%f]",user_sensitivity_x,user_sensitivity_y,user_sensitivity_x_3rd,user_sensitivity_y_3rd);
            }
            else
            {
               set_smoothing(!ZealService::get_instance()->camera_mods->enabled);
               if (ZealService::get_instance()->camera_mods->enabled)
               {
                   Zeal::EqGame::print_chat("Zealcam enabled");
                   Zeal::EqGame::print_chat("camera sensitivity FirstPerson : [% f] [% f] ThirdPerson : [% f] [% f] ", user_sensitivity_x, user_sensitivity_y, user_sensitivity_x_3rd, user_sensitivity_y_3rd);
               }
                else
                    Zeal::EqGame::print_chat("Zealcam disabled");
            }
            return true;
        });
    zeal->binds_hook->replace_bind(15, [this](int state) { handle_binds(15, state); return false; });
    zeal->binds_hook->replace_bind(115, [this](int state) { handle_binds(15, state); return false; });
    zeal->binds_hook->replace_bind(111, [this](int state) { handle_binds(15, state); return false; });
    zeal->binds_hook->replace_bind(16, [this](int state) { handle_binds(16, state); return false; });
    zeal->binds_hook->replace_bind(116, [this](int state) { handle_binds(16, state); return false; });
    zeal->binds_hook->replace_bind(112, [this](int state) { handle_binds(16, state); return false; });
    zeal->binds_hook->replace_bind(18, [this](int state) { handle_binds(18, state); return false; });
    zeal->binds_hook->replace_bind(19, [this](int state) { handle_binds(19, state); return false; });
   }

CameraMods::~CameraMods()
{
    toggle_zeal_cam(false);
}