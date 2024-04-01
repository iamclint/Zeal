#include "camera_mods.h"
#include "EqFunctions.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include <thread>
#include <algorithm>
#include <windowsx.h>
#include "StringUtil.h"
#include "camera_math.h"
//#define debug_cam
bool CameraMods::update_cam()
{
    if (!enabled)
        return false;

    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_view_actor_entity();
    if (!self)
        return false;



    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
    Vec3 head_pos = Zeal::EqGame::get_view_actor_head_pos();
    Vec3 wanted_pos = camera_math::get_cam_pos_behind(head_pos, current_zoom, zeal_cam_yaw/*self->Heading*/, -zeal_cam_pitch);

#ifdef debug_cam
    ZealService::get_instance()->labels_hook->print_debug_info("View actor: %i\nHead: %s\nWanted: %s", self, head_pos.toString().c_str(), wanted_pos.toString().c_str());
#endif
    bool rval = Zeal::EqGame::collide_with_world(head_pos, wanted_pos, wanted_pos);
    cam->Position = wanted_pos;
    cam->Heading = zeal_cam_yaw;// self->Heading;
    cam->Pitch = camera_math::get_pitch(cam->Position, head_pos);
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


void CameraMods::toggle_zeal_cam(bool enabled)
{
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_controlled();
    if (enabled)
    {
        *Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::ZealCam;
        if (self) 
        {
            zeal_cam_pitch = camera_math::pitch_to_normal(self->Pitch);
            zeal_cam_yaw = self->Heading;
        }
        desired_zoom += zoom_speed;
        mem::set(0x4db8ce, 0x90, 6, original_cam[0] == 0x0 ? original_cam : 0);
        mem::write<BYTE>(0x4db8d9, 0xEB); //fix the camera bad location print
    }
    else if (original_cam[0] != 0x0)
    {
        if (self)
            self->Pitch = camera_math::pitch_to_game(zeal_cam_pitch);
        current_zoom = 0;
        if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam && Zeal::EqGame::is_in_game())
           *Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::FirstPerson;
        mem::copy(0x4db8ce, original_cam, 6);
        mem::write<BYTE>(0x4db8d9, 0x74); //fix the camera bad location print
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
    if (!enabled)
        return;
    if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam || *Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::FirstPerson)
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
            float t = 1.0f / 1.5f;
            /*if (fps < 50)
            {
                delta_y *= fps/2;
                delta_x *= fps/2;
            }*/
            smoothMouseDeltaX = camera_math::lerp(delta_x * sensitivity_x, smoothMouseDeltaX, t);
            smoothMouseDeltaY = camera_math::lerp(delta_y * sensitivity_y, smoothMouseDeltaY, t);
            if (fabs(smoothMouseDeltaX) > 5)
                smoothMouseDeltaY /= 2;

            delta->y = 0;
            delta->x = 0;

            if (*(byte*)0x7985E8) //invert
                smoothMouseDeltaY = -smoothMouseDeltaY;

            if (Zeal::EqGame::can_move())
            {
                if (rbutton)
                {
                    if (fabs(smoothMouseDeltaX) > 0.1)
                        self->MovementSpeedHeading = -smoothMouseDeltaX/1000;
                    else
                        self->MovementSpeedHeading = 0;

                    if (camera_view == Zeal::EqEnums::CameraView::ZealCam)
                    {
                        zeal_cam_yaw -= smoothMouseDeltaX;
                        zeal_cam_yaw = fmodf(zeal_cam_yaw, 512.f); //eventually the player would stop turning but camera would without normalizing
                        self->Heading = zeal_cam_yaw;
                    }
                    else
                        self->Heading += -smoothMouseDeltaX;
                }
            }
            if (lbutton || (rbutton && !Zeal::EqGame::can_move()))
            {
                zeal_cam_yaw -= smoothMouseDeltaX;
            }



            if (fabs(smoothMouseDeltaY) > 0)
            {
                if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
                {
                    zeal_cam_pitch -= smoothMouseDeltaY;
                    zeal_cam_pitch = std::clamp(zeal_cam_pitch, -89.9f, 89.9f);
                    if (Zeal::EqGame::KeyMods->Shift)
                        self->Pitch = camera_math::pitch_to_game(zeal_cam_pitch);
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
    ZealService::get_instance()->ui->UpdateOptions();
}

void CameraMods::interpolate_zoom() 
{
    if (!enabled)
        return;
    if (*Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
    {
        current_zoom = current_zoom + (desired_zoom - current_zoom) * .3f; //linear interpolation
        if (current_zoom < .5)
            toggle_zeal_cam(false);
    }
}


void CameraMods::handle_camera_motion_binds(int cmd, bool is_down)
{
    DWORD camera_view = *Zeal::EqGame::camera_view;
    if (!enabled)
    {
        cmd_key_map.clear();
        return;
    }

   // if ((camera_view == Zeal::EqEnums::CameraView::ZealCam) || (cmd == 19)) //allow zoom out
 //   {
        if (is_down)
            cmd_key_map[cmd] = true;
        else
            cmd_key_map[cmd] = false;
   //     return;
  //  }
 //   cmd_key_map[cmd] = false;
}

// didn't want to clutter handle_binds() with specific logic to this situation.
// feel free to change this if you would prefer to have it elsewhere.
void CameraMods::handle_cycle_camera_views(int cmd, bool is_down)
{
  DWORD camera_view = *Zeal::EqGame::camera_view;
  if (cmd == 20) //removed enabled
  {
    int strafe_camera_count = Zeal::EqEnums::CameraView::TotalCameras;
    if (camera_view == Zeal::EqEnums::CameraView::ThirdPerson3 && !camera3_strafe_enabled)
    {
      strafe_camera_count = Zeal::EqEnums::CameraView::ThirdPerson2;
    }
    else if (camera_view == Zeal::EqEnums::CameraView::ThirdPerson4 && !camera4_strafe_enabled)
    {
      strafe_camera_count = Zeal::EqEnums::CameraView::ThirdPerson2;
    }

    mem::write<byte>(0x53fa50, strafe_camera_count);
    mem::write<byte>(0x53f648, strafe_camera_count);
  }
  else
  {
      cmd_key_map.clear();
  }
}

// check to help fix left mouse panning from preventing repositioning the game in windowed mode.
static bool is_over_title_bar(void)
{
  // was going to reuse Zeal::EqStructures::MouseDelta* but the values flow over INT16
  //Zeal::EqStructures::MouseDelta* mouse_pos = (Zeal::EqStructures::MouseDelta*)0x798580;
  WORD mouse_y = *(WORD*)0x798582;
  return (mouse_y >= 0xE6FF && mouse_y <= 0xFFFF);
}


void CameraMods::tick_key_move()
{
    if (!enabled)
    {
        if (cmd_key_map.size())
            cmd_key_map.clear();
        return;
    }
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    DWORD camera_view = *Zeal::EqGame::camera_view;
    //if (camera_view != Zeal::EqEnums::CameraView::ZealCam && current_key_cmd != 19)
    //    current_key_cmd = 0;

    if (camera_view == Zeal::EqEnums::CameraView::FirstPerson && cmd_key_map[19])
    {
        mouse_wheel(-120);
    }

    if (!*Zeal::EqGame::is_right_mouse_down && *Zeal::EqGame::is_left_mouse_down && camera_view == Zeal::EqEnums::CameraView::ZealCam && !is_over_title_bar() && !Zeal::EqGame::is_game_ui_window_hovered())
    {
        if (!lmouse_time)
        {
            GetCursorPos(&lmouse_cursor_pos);
            hide_cursor = true;
            lmouse_time = GetTickCount64();
        }

        if (GetTickCount64() - lmouse_time > pan_delay)
        {
            HWND gwnd = Zeal::EqGame::get_game_window();
            POINT cursor_pos_for_window;
            GetCursorPos(&cursor_pos_for_window);
            if (gwnd == WindowFromPoint(cursor_pos_for_window))
            {
                if (hide_cursor && GetTickCount64() - lmouse_time>pan_delay)
                {
                    mem::write<byte>(0x53edef, 0xEB);
                    hide_cursor = false;
                }
                proc_mouse();
                SetCursorPos(lmouse_cursor_pos.x, lmouse_cursor_pos.y);
            }
        }
    }
    else
    {
        if (lmouse_time)
            mem::write<byte>(0x53edef, 0x75);
        lmouse_time = 0;
    }
    for (auto& [current_key_cmd, down] : cmd_key_map)
    {
        if (down)
        {
            switch (current_key_cmd)
            {
            case 5:
            case 6:
            {
                if (Zeal::EqGame::can_move())
                {
                    if (camera_view == Zeal::EqEnums::CameraView::ZealCam && (self->StandingState == Stance::Stand || self->StandingState == Stance::Duck))
                    {
                        //if your camera is panned more than 5 degrees different than your players heading then shift the player to match the camera
                        if (fabs(camera_math::angle_difference(zeal_cam_yaw, self->Heading)) > 10 && fps > 40)
                        {
                            self->Heading = zeal_cam_yaw;
                        }
                        else
                            zeal_cam_yaw = self->Heading;
                    }
                }
                break;
            }
            case 15: //up
                //zeal_cam_pitch -= 0.3f;
                //if (zeal_cam_pitch <= -89.99f)
                //    zeal_cam_pitch = -89.99f;
                break;
            case 16: //down
                //zeal_cam_pitch += 0.3f;
                //if (zeal_cam_pitch >= 89.99f)
                //    zeal_cam_pitch = 89.99f;
                break;
            case 18: //zoom in
                if (desired_zoom > 0)
                    desired_zoom -= .3f;
                break;
            case 19: //zoom out
                desired_zoom += .3f;
                if (desired_zoom > max_zoom_out)
                    desired_zoom = max_zoom_out;
                break;
            }
        }
    }
}

void CameraMods::update_fps_sensitivity()
{
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
}
void CameraMods::callback_render()
{
    if (enabled)
    {
        if (Zeal::EqGame::is_in_game() && Zeal::EqGame::get_self() && Zeal::EqGame::get_char_info() && !*Zeal::EqGame::is_right_mouse_down && *Zeal::EqGame::camera_view == Zeal::EqEnums::CameraView::ZealCam)
        {
            if (Zeal::EqGame::get_self()->Position.Dist(Zeal::EqGame::get_char_info()->ZoneEnter) != 0)
                update_cam();
        }
    }
}
void CameraMods::callback_main()
{
    static int prev_view = *Zeal::EqGame::camera_view;
    DWORD camera_view = *Zeal::EqGame::camera_view;
    if (enabled && Zeal::EqGame::is_in_game())
    {
        interpolate_zoom();
        update_fps_sensitivity();
        tick_key_move();

        if (prev_view != *Zeal::EqGame::camera_view) //this simply checks if your camera type has changed
        {
            if (*Zeal::EqGame::camera_view != Zeal::EqEnums::CameraView::ZealCam)
            {
                toggle_zeal_cam(false);
            }
            else
            {
                toggle_zeal_cam(true);
                desired_zoom = zoom_speed;
            }
        }
        

           
    }
    prev_view = *Zeal::EqGame::camera_view;
}
void CameraMods::callback_characterselect()
{
    ZealService* zeal = ZealService::get_instance();
    zeal->camera_mods->toggle_zeal_cam(false);
 }

void CameraMods::load_settings(IO_ini* ini)
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

    if (!ini->exists("Zeal", "Camera3StrafeEnabled"))
        ini->setValue<bool>("Zeal", "Camera3StrafeEnabled", true);
    if (!ini->exists("Zeal", "Camera4StrafeEnabled"))
        ini->setValue<bool>("Zeal", "Camera4StrafeEnabled", true);
    if (!ini->exists("Zeal", "CycleToZealCamEnabled"))
        ini->setValue<bool>("Zeal", "CycleToZealCamEnabled", true);
    if (!ini->exists("Zeal", "PanDelay"))
        ini->setValue<int>("Zeal", "PanDelay", 200);

    pan_delay = ini->getValue<int>("Zeal", "PanDelay");
    camera3_strafe_enabled = ini->getValue<bool>("Zeal", "Camera3StrafeEnabled");
    camera4_strafe_enabled = ini->getValue<bool>("Zeal", "Camera4StrafeEnabled");
    cycle_to_zeal_cam_enabled = ini->getValue<bool>("Zeal", "CycleToZealCamEnabled");
}

void CameraMods::proc_rmousedown(int x, int y)
{
    DWORD camera_view = *Zeal::EqGame::camera_view;
    if (camera_view == Zeal::EqEnums::CameraView::ZealCam && Zeal::EqGame::can_move())
    {
        //if your camera is panned more than 5 degrees different than your players heading then shift the player to match the camera
        if (fabs(camera_math::angle_difference(zeal_cam_yaw, Zeal::EqGame::get_self()->Heading)) > 5)
            Zeal::EqGame::get_self()->Heading = zeal_cam_yaw;
    }
}

void __stdcall procRightMouse(int x, int y)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->camera_mods->proc_rmousedown(x,y);
    zeal->hooks->hook_map["procRightMouse"]->original(procRightMouse)(x, y);
}

void CameraMods::set_pan_delay(int value_ms)
{
   pan_delay = value_ms;
   ZealService::get_instance()->ini->setValue<int>("Zeal", "PanDelay", pan_delay);
   ZealService::get_instance()->ui->UpdateOptions();
}

void CameraMods::update_sensitivity()
{
    ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX", user_sensitivity_x);
    ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY", user_sensitivity_y);
    ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityX3rd", user_sensitivity_x_3rd);
    ZealService::get_instance()->ini->setValue<float>("Zeal", "MouseSensitivityY3rd", user_sensitivity_y_3rd);
    ZealService::get_instance()->ui->UpdateOptions();
}

CameraMods::CameraMods(ZealService* zeal, IO_ini* ini)
{
    load_settings(ini);

    mem::write<byte>(0x53fa50, Zeal::EqEnums::CameraView::TotalCameras); //allow for strafing whenever in zeal cam
    mem::write<byte>(0x53f648, Zeal::EqEnums::CameraView::TotalCameras); //allow for strafing whenever in zeal cam

    if (enabled && cycle_to_zeal_cam_enabled)
        mem::write<byte>(0x4adcd9, Zeal::EqEnums::CameraView::TotalCameras); //allow for the camera toggle hotkey to cycle through the new camera
    else
        mem::write<byte>(0x4adcd9, Zeal::EqEnums::CameraView::ZealCam);

    lastTime = std::chrono::steady_clock::now();
    fps = 0;
    height = 0;
    zeal->callbacks->add_callback([this]() { callback_main();  });
    zeal->callbacks->add_callback([this]() { callback_render();  }, callback_fn::Render);
    //zeal->main_loop_hook->add_callback([this]() { callback_characterselect();  }, callback_fn::CharacterSelect);
    zeal->callbacks->add_callback([this]() { callback_characterselect(); }, callback_fn::EndMainLoop);
    zeal->hooks->Add("HandleMouseWheel", Zeal::EqGame::EqGameInternal::fn_handle_mouseweheel, handle_mouse_wheel, hook_type_detour);
    zeal->hooks->Add("procMouse", 0x537707, procMouse, hook_type_detour);
    zeal->hooks->Add("procRightMouse", 0x54699d, procRightMouse, hook_type_detour);
    zeal->commands_hook->add("/pandelay", { "/pd" },
        [this, ini](std::vector<std::string>& args) {
            int delay = 200;
            if (args.size() == 2)
            {
                if (StringUtil::tryParse(args[1], &delay))
                {
                    set_pan_delay(delay);
                    Zeal::EqGame::print_chat("Click to pan delay is now %i", pan_delay);
                }
            }
            else
                Zeal::EqGame::print_chat("Invalid arguments for pandelay example usage: /pandelay 200");
            return true;
        });
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
                update_sensitivity();
                Zeal::EqGame::print_chat("New camera sensitivity [%f] [%f]", user_sensitivity_x, user_sensitivity_y);
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
                update_sensitivity();
                Zeal::EqGame::print_chat("New camera sensitivity FirstPerson: [%f] [%f] ThirdPerson: [%f] [%f]", user_sensitivity_x, user_sensitivity_y, user_sensitivity_x_3rd, user_sensitivity_y_3rd);
            }
            else
            {
                set_smoothing(!ZealService::get_instance()->camera_mods->enabled);
                if (ZealService::get_instance()->camera_mods->enabled)
                {
                    Zeal::EqGame::print_chat("Zealcam enabled");
                    Zeal::EqGame::print_chat("camera sensitivity FirstPerson : [% f] [% f] ThirdPerson : [% f] [% f] ", user_sensitivity_x, user_sensitivity_y, user_sensitivity_x_3rd, user_sensitivity_y_3rd);
                    if (cycle_to_zeal_cam_enabled)
                        mem::write<byte>(0x4adcd9, Zeal::EqEnums::CameraView::TotalCameras);
                }
                else
                {
                    Zeal::EqGame::print_chat("Zealcam disabled");
                    mem::write<byte>(0x4adcd9, Zeal::EqEnums::CameraView::ZealCam);
                }
            }
            return true;
        });
    zeal->binds_hook->replace_bind(15, [this](int state) { handle_camera_motion_binds(15, state); return false; });
    zeal->binds_hook->replace_bind(115, [this](int state) { handle_camera_motion_binds(15, state); return false; });
    zeal->binds_hook->replace_bind(111, [this](int state) { handle_camera_motion_binds(15, state); return false; });
    zeal->binds_hook->replace_bind(16, [this](int state) { handle_camera_motion_binds(16, state); return false; });
    zeal->binds_hook->replace_bind(116, [this](int state) { handle_camera_motion_binds(16, state); return false; });
    zeal->binds_hook->replace_bind(112, [this](int state) { handle_camera_motion_binds(16, state); return false; });
    zeal->binds_hook->replace_bind(18, [this](int state) { handle_camera_motion_binds(18, state); return false; });
    zeal->binds_hook->replace_bind(19, [this](int state) { handle_camera_motion_binds(19, state); return false; });
    zeal->binds_hook->replace_bind(20, [this](int state) { handle_cycle_camera_views(20, state); return false; });

}

CameraMods::~CameraMods()
{
    shutting_down = true;
    toggle_zeal_cam(false);
}
