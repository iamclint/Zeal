#include "camera_mods.h"
#include "EqFunctions.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include <thread>
#include <algorithm>
float Smooth(short rawDelta, float smoothDelta) {
    return std::lerp(smoothDelta, static_cast<float>(rawDelta), 1.0f / 1.5f);
}
static int zeal_cam = Zeal::EqEnums::CameraView::ThirdPerson3;
bool can_move()
{
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    if (!self || !self->CharInfo)
        return false;

    if (self->CharInfo->StunnedState)
        return false;
    if (self->StandingState == Zeal::EqEnums::Stance::Standing || self->StandingState == Zeal::EqEnums::Stance::Ducking)
        return true;
    return false;
}
float get_pitch(Vec3 cameraPos, Vec3 targetPos) {
    Vec3 direction = targetPos - cameraPos;
    float horizontalDistance = direction.Length2D();
    float verticalDistance = direction.z;
    float pitch = std::atan2(verticalDistance, horizontalDistance);
    float pitchDegrees = pitch * (180.0 / M_PI);

    // Scale the pitch to fit into the [-128, 128] range
    pitchDegrees *= (128.0 / 90.0);

    return pitchDegrees;
}

Vec3 calculatePositionBehind(const Vec3& playerHead, float distance, float playerYaw, float pitch) {
    float yaw_rads = playerYaw * (2 * M_PI) / 512.0f;
    float yaw_deg = yaw_rads * (180.0 / M_PI);

    // Convert pitch to radians
    float pitch_rads = pitch * (M_PI / 180.0f);

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
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
    Vec3 head_pos = Zeal::EqGame::get_player_head_pos();
    Vec3 wanted_pos = calculatePositionBehind(head_pos, current_zoom, self->Heading, -self->Pitch);
    bool rval = Zeal::EqGame::CollideWithWorld(head_pos, wanted_pos, wanted_pos);
    cam->Position = wanted_pos;
    cam->Heading = self->Heading;
    cam->Pitch = get_pitch(cam->Position, head_pos);
    return rval;
}

void CameraMods::mouse_wheel(int delta)
{
    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();

    DWORD camera_view = *Zeal::EqGame::camera_view;
            
    if (delta == 120)
    {
        if (camera_view != Zeal::EqEnums::CameraView::FirstPerson)
        {
            if (current_zoom <= zoom_speed && camera_view == zeal_cam)
                toggle_zeal_cam(false);
            else if (camera_view == zeal_cam)
                update_zoom(-zoom_speed);
        }
    }
    else if (delta == -120)
    {
        if (camera_view == Zeal::EqEnums::CameraView::FirstPerson)
        {
            toggle_zeal_cam(true);
            update_zoom(zoom_speed);
        }
        else 
           update_zoom(zoom_speed);
    }
}
void CameraMods::toggle_zeal_cam(bool enabled)
{
    if (enabled)
    {
        *Zeal::EqGame::camera_view = zeal_cam;
        mem::set(0x4db8ce, 0x90, 6, original_cam); //nop the games camera animation
    }
    else if (original_cam[0] != 0x0)
    {
        *Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::FirstPerson;
        mem::copy(0x4db8ce, original_cam, 6);
    }
}
void CameraMods::update_zoom(float zoom)
{
    current_zoom += zoom;
    if (current_zoom > 100)
        current_zoom = 100;
    if (current_zoom < 0)
        current_zoom = 0;
    if (update_cam()) //there was a collision
    {
        if (zoom > 0)
        {
            current_zoom -= zoom;
            update_cam();
        }
    }
}



int handle_mouse_wheel(int delta)
{
    CameraMods* c = ZealService::get_instance()->camera_mods.get();
    if (!Zeal::EqGame::is_mouse_hovering_window() && c->smoothing)
    {
        c->mouse_wheel(delta);
        return 0;
    }
    else
        return ZealService::get_instance()->hooks->hook_map["HandleMouseWheel"]->original(handle_mouse_wheel)(delta);
}


void CameraMods::proc_mouse()
{

    if (smoothing && can_move()) //maybe add some setting to toggle this feature on and off
    {
        static float smoothMouseDeltaX = 0;
        static float smoothMouseDeltaY = 0;
        Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
        DWORD camera_view = *Zeal::EqGame::camera_view;
        Zeal::EqStructures::MouseDelta* delta = (Zeal::EqStructures::MouseDelta*)0x798586;
        Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
        if (!self)
            return;
        if (*Zeal::EqGame::is_right_mouse_down)
        {
            float delta_y = delta->y;
            float delta_x = delta->x;
            smoothMouseDeltaX = Smooth(delta_x * sensitivity_x, smoothMouseDeltaX);
            smoothMouseDeltaY = Smooth(delta_y * sensitivity_y, smoothMouseDeltaY);
            delta->y = 0;
            delta->x = 0;

            if (fabs(smoothMouseDeltaX) > 0.1)
                self->MovementSpeedHeading = -smoothMouseDeltaX;
            else
                self->MovementSpeedHeading = 0;


            if (fabs(smoothMouseDeltaY) > 0)
            {
                self->Pitch -= smoothMouseDeltaY/2.f;

                if (self->Pitch > 89.9)
                    self->Pitch = 89.9;
                if (self->Pitch < -89.9)
                    self->Pitch = -89.9;
            }
        }
        if (camera_view == zeal_cam)
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
    zeal->hooks->hook_map["procMouse"]->original(procMouse)(eq, unused, a1);
}

void CameraMods::set_smoothing(bool val)
{
    smoothing = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "MouseSmoothing", smoothing);
}

void CameraMods::callback_main()
{
    Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
    if (!cam)
        return;
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
    if (elapsedTime > 0) {
        fps = 1000.0 / elapsedTime;
    }

    

    sensitivity_x = .7 * (fps / 144.f);
    sensitivity_y = .3 * (fps / 144.f);

    float current_sens = (float)(*(byte*)0x798b0c);
    float multiplier = current_sens / 4.0f;
    sensitivity_x *= multiplier;
    sensitivity_y *= multiplier;
    lastTime = currentTime;
    if (!*Zeal::EqGame::is_right_mouse_down && Zeal::EqGame::is_in_game())
    {
        if (*Zeal::EqGame::camera_view == zeal_cam)
            update_cam();
    }
    if (!Zeal::EqGame::is_in_game())
        reset_cam();
}

void CameraMods::reset_cam()
{
    if (original_cam && *(BYTE*)0x4db8ce == 0x90)
    {
        mem::copy(0x4db8ce, (BYTE*)original_cam, 6); //re-enable games camera movement
    }
}
void _fastcall doCharacterSelection(int t, int u)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->camera_mods->reset_cam();
    zeal->hooks->hook_map["DoCharacterSelection"]->original(doCharacterSelection)(t, u);
}

CameraMods::CameraMods(ZealService* zeal)
{
    mem::write<byte>(0x53fa50, 03); //allow for strafing whenever in zeal cam
    mem::write<byte>(0x53f648, 03); //allow for strafing whenever in zeal cam
    smoothing = true;
    lastTime = std::chrono::steady_clock::now();
    fps = 0;
    height = 0;
    zeal->main_loop_hook->add_callback([this]() { callback_main();  });
	zeal->hooks->Add("HandleMouseWheel", Zeal::EqGame::EqGameInternal::fn_handle_mouseweheel, handle_mouse_wheel, hook_type_detour);
    zeal->hooks->Add("procMouse", 0x537707, procMouse, hook_type_detour);
    zeal->hooks->Add("DoCharacterSelection", 0x53b9cf, doCharacterSelection, hook_type_detour);
}

CameraMods::~CameraMods()
{
    reset_cam();
}