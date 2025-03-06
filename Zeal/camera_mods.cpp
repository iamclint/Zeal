#include "camera_mods.h"
#include "EqFunctions.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include <thread>
#include <algorithm>
#include <windowsx.h>
#include "string_util.h"
#include "camera_math.h"
//#define debug_cam

int get_camera_view()
{
    return *Zeal::EqGame::camera_view;
}

void set_camera_view(int view)
{
    *Zeal::EqGame::camera_view = view;
}

bool CameraMods::update_cam()
{
    if (!enabled.get())
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
    DWORD camera_view = get_camera_view();
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


void CameraMods::toggle_zeal_cam(bool enabled, bool reset_pitch)
{
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_controlled();
    if (enabled)
    {
        set_camera_view(Zeal::EqEnums::CameraView::ZealCam);
        if (self) 
        {
            if (reset_pitch)
                zeal_cam_pitch = camera_math::pitch_to_normal(self->Pitch);
            zeal_cam_yaw = self->Heading;
        }
        desired_zoom += zoom_speed;
    //    mem::set(0x4db8ce, 0x90, 6, original_cam[0] == 0x0 ? original_cam : 0);
        mem::write<BYTE>(0x4db8d9, 0xEB); //fix the camera bad location print
    }
    else if (original_cam[0] != 0x0)
    {
        if (self)
            self->Pitch = camera_math::pitch_to_game(zeal_cam_pitch);
        current_zoom = 0;
        if (get_camera_view() == Zeal::EqEnums::CameraView::ZealCam && Zeal::EqGame::is_in_game())
           set_camera_view(Zeal::EqEnums::CameraView::FirstPerson);
       // mem::copy(0x4db8ce, original_cam, 6);
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

    if (desired_zoom == 0)
    {
        Zeal::EqGame::get_self()->Pitch = zeal_cam_pitch;
        set_camera_view(Zeal::EqEnums::CameraView::FirstPerson);
    }

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
    DWORD camera_view = get_camera_view();
    BYTE explore_mode = 0;
    if (Zeal::EqGame::get_gamestate() == GAMESTATE_CHARSELECT)
    {
        DWORD character_select = *(DWORD*)0x63D5D8;
        if (character_select)
            explore_mode = *(BYTE*)(character_select + 0x171);
    }

    if ((explore_mode || !Zeal::EqGame::is_mouse_hovering_window()) && c->enabled.get() && (camera_view == Zeal::EqEnums::CameraView::FirstPerson || camera_view == Zeal::EqEnums::CameraView::ZealCam))
    {
        c->mouse_wheel(delta);
        return 0;
    }
    else
        return ZealService::get_instance()->hooks->hook_map["HandleMouseWheel"]->original(handle_mouse_wheel)(delta);
}


void CameraMods::proc_mouse()
{
    if (!enabled.get())
        return;
    if (get_camera_view() == Zeal::EqEnums::CameraView::ZealCam || get_camera_view() == Zeal::EqEnums::CameraView::FirstPerson)
    {
        static float smoothMouseDeltaX = 0;
        static float smoothMouseDeltaY = 0;
        Zeal::EqStructures::CameraInfo* cam = Zeal::EqGame::get_camera();
        DWORD camera_view = get_camera_view();
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
                if (get_camera_view() == Zeal::EqEnums::CameraView::ZealCam)
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
    int camera_view = get_camera_view();
    enabled.set(val);
    if (val && (camera_view == Zeal::EqEnums::CameraView::ZealCam || camera_view == Zeal::EqEnums::CameraView::ThirdPerson2)) //thirdperson2 is the mouse wheel out normally
        toggle_zeal_cam(true);
    else
        toggle_zeal_cam(false);
    ZealService::get_instance()->ui->options->UpdateOptions();
}

void CameraMods::interpolate_zoom() 
{
    if (!enabled.get())
        return;
    if (get_camera_view() == Zeal::EqEnums::CameraView::ZealCam)
    {
        current_zoom = current_zoom + (desired_zoom - current_zoom) * .3f; //linear interpolation
        if (current_zoom < .5)
            toggle_zeal_cam(false);
    }
}


void CameraMods::handle_camera_motion_binds(int cmd, bool is_down)
{
    DWORD camera_view = get_camera_view();
    if (!enabled.get())
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
  DWORD camera_view = get_camera_view();
  if (cmd == 20) //removed enabled
  {
    int strafe_camera_count = Zeal::EqEnums::CameraView::TotalCameras;
    if (camera_view == Zeal::EqEnums::CameraView::ThirdPerson3 && !camera3_strafe_enabled.get())
    {
      strafe_camera_count = Zeal::EqEnums::CameraView::ThirdPerson2;
    }
    else if (camera_view == Zeal::EqEnums::CameraView::ThirdPerson4 && !camera4_strafe_enabled.get())
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
    if (!enabled.get())
    {
        if (cmd_key_map.size())
            cmd_key_map.clear();
        return;
    }
    Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
    DWORD camera_view = get_camera_view();
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

        if (GetTickCount64() - lmouse_time > pan_delay.get())
        {
            HWND gwnd = Zeal::EqGame::get_game_window();
            POINT cursor_pos_for_window;
            GetCursorPos(&cursor_pos_for_window);
            if (gwnd == WindowFromPoint(cursor_pos_for_window))
            {
                if (hide_cursor && GetTickCount64() - lmouse_time>pan_delay.get())
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
                    //if (camera_view == Zeal::EqEnums::CameraView::ZealCam && (self->StandingState == Stance::Stand || self->StandingState == Stance::Duck))
                    //{
                    //    //if your camera is panned more than 5 degrees different than your players heading then shift the player to match the camera
                    //    if (fabs(camera_math::angle_difference(zeal_cam_yaw, self->Heading)) > 20 && fps > 40)
                    //       self->Heading = zeal_cam_yaw;
                    //    else
                        if (cam_lock.get())
                            zeal_cam_yaw = self->Heading;
                   // }
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

    if (use_old_sens.get())
    {
        sensitivity_x = user_sensitivity_x.get() * (fps / 144.f);
        sensitivity_y = user_sensitivity_y.get() * (fps / 144.f);

        if (get_camera_view() == Zeal::EqEnums::CameraView::ZealCam)
        {
            sensitivity_x = user_sensitivity_x_3rd.get() * (fps / 144.f);
            sensitivity_y = user_sensitivity_y_3rd.get() * (fps / 144.f);
        }
    }
    else //this 'should' eliminate the sensitivity going down significantly when going into low fps regions
    {
        sensitivity_x = user_sensitivity_x.get();
        sensitivity_y = user_sensitivity_y.get();
        if (get_camera_view() == Zeal::EqEnums::CameraView::ZealCam)
        {
            sensitivity_x = user_sensitivity_x_3rd.get();
            sensitivity_y = user_sensitivity_y_3rd.get();
        }
    }
    


    float current_sens = (float)(*(byte*)0x798b0c);
    float multiplier = current_sens / 4.0f;
    sensitivity_x *= multiplier;
    sensitivity_y *= multiplier;
    lastTime = currentTime;
}
void CameraMods::callback_render()
{
    if (enabled.get())
    {
        //if (get_camera_view() == Zeal::EqEnums::CameraView::FirstPerson)
        //{
        //    set_camera_view(Zeal::EqEnums::CameraView::ZealCam);
        //    current_zoom = 0;
        //}
        if (Zeal::EqGame::is_in_game() && Zeal::EqGame::get_self() && Zeal::EqGame::get_char_info() /*&& !*Zeal::EqGame::is_right_mouse_down*/ && get_camera_view() == Zeal::EqEnums::CameraView::ZealCam)
        { 
            if (Zeal::EqGame::get_self()->Position.Dist(Zeal::EqGame::get_char_info()->ZoneEnter) != 0)
                update_cam();
            
        }
    }
}

void CameraMods::callback_main()
{
    static int prev_view = get_camera_view();
    DWORD camera_view = get_camera_view();
    if (Zeal::EqGame::is_in_game())
        update_fps_sensitivity();
    if (enabled.get() && Zeal::EqGame::is_in_game() && !main_loop_ended)
    {
        interpolate_zoom();
        tick_key_move();

        if (prev_view != get_camera_view() && Zeal::EqGame::get_self() && Zeal::EqGame::is_in_game()) //this simply checks if your camera view has changed
        {
            if (get_camera_view() != Zeal::EqEnums::CameraView::ZealCam)
            {
                toggle_zeal_cam(false);
            }
            else
            {
                if (get_camera_view() != Zeal::EqEnums::CameraView::ZealCam)
                {
                    toggle_zeal_cam(true);
                    desired_zoom = zoom_speed;
                }
            }
        }
    }
    if (main_loop_ended && prev_view == Zeal::EqEnums::CameraView::ZealCam)
    {
        toggle_zeal_cam(true, false);
    }
    main_loop_ended = false;
    prev_view = get_camera_view();
}
void CameraMods::callback_characterselect()
{
    ZealService* zeal = ZealService::get_instance();
    zeal->camera_mods->toggle_zeal_cam(false);
 }


void CameraMods::proc_rmousedown(int x, int y)
{
    DWORD camera_view = get_camera_view();
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

void CameraMods::callback_endmainloop()
{
    main_loop_ended = true;
}

int SetCameraLens(int a1, float fov, float aspect_ratio, float a4, float a5)
{
    ZealService* zeal = ZealService::get_instance();
    fov = zeal->camera_mods->fov.get();
    int rval = zeal->hooks->hook_map["SetCameraLens"]->original(SetCameraLens)(a1, fov, aspect_ratio, a4, a5);
    if (Zeal::EqGame::get_gamestate()!=GAMESTATE_PRECHARSELECT)
    {
        Zeal::EqStructures::CameraInfo* ci = Zeal::EqGame::get_camera(); 
        if (ci)
            ci->FieldOfView = zeal->camera_mods->fov.get();
    }
    return rval;
}

void __fastcall DoCamAI(int display, int u, float p1)
{
    ZealService* zeal = ZealService::get_instance();
    zeal->hooks->hook_map["DoCamAI"]->original(DoCamAI)(display, u, p1);
    if (Zeal::EqGame::is_in_game() && get_camera_view() == Zeal::EqEnums::CameraView::ZealCam)
        zeal->camera_mods->update_cam();
}

static void __fastcall GetClickedActor(int this_display, int unused_edx, int mouse_x, int mouse_y, int get_on_actor)
{
    auto self = Zeal::EqGame::get_self();
    bool clickthru = !get_on_actor && ZealService::get_instance()->camera_mods->setting_selfclickthru.get();
    float* bounding_radius = (clickthru && self && self->ActorInfo && self->ActorInfo->ViewActor_) ?
        &self->ActorInfo->ViewActor_->BoundingRadius : nullptr;
    float original_radius = bounding_radius ? *bounding_radius : 0;
    if (bounding_radius)
        *bounding_radius = 0.001f;  // Small, hard to click value.
    ZealService* zeal = ZealService::get_instance();
    zeal->hooks->hook_map["GetClickedActor"]->original(GetClickedActor)(this_display, unused_edx, mouse_x, mouse_y, get_on_actor);
    if (bounding_radius)
        *bounding_radius = original_radius;
}

CameraMods::CameraMods(ZealService* zeal, IO_ini* ini)
{
    mem::write<byte>(0x53fa50, Zeal::EqEnums::CameraView::TotalCameras); //allow for strafing whenever in zeal cam
    mem::write<byte>(0x53f648, Zeal::EqEnums::CameraView::TotalCameras); //allow for strafing whenever in zeal cam

    if (enabled.get() && cycle_to_zeal_cam_enabled.get())
        mem::write<byte>(0x4adcd9, Zeal::EqEnums::CameraView::TotalCameras); //allow for the camera toggle hotkey to cycle through the new camera
    else
        mem::write<byte>(0x4adcd9, Zeal::EqEnums::CameraView::ZealCam);

    lastTime = std::chrono::steady_clock::now();
    fps = 0;
    height = 0;
    zeal->callbacks->AddGeneric([this]() { callback_main();  });
    zeal->callbacks->AddGeneric([this]() { callback_main();  }, callback_type::CharacterSelectLoop);
    zeal->callbacks->AddGeneric([this]() { callback_render();  }, callback_type::Render);
    zeal->callbacks->AddGeneric([this]() { callback_endmainloop(); }, callback_type::EndMainLoop);

    //zeal->main_loop_hook->add_callback([this]() { callback_characterselect();  }, callback_fn::CharacterSelect);
    zeal->callbacks->AddGeneric([this]() { callback_characterselect(); }, callback_type::EndMainLoop);
    zeal->hooks->Add("HandleMouseWheel", 0x55B2E0, handle_mouse_wheel, hook_type_detour);
    zeal->hooks->Add("procMouse", 0x537707, procMouse, hook_type_detour);
    zeal->hooks->Add("procRightMouse", 0x54699d, procRightMouse, hook_type_detour);
    zeal->hooks->Add("DoCamAI", 0x4db384, DoCamAI, hook_type_detour);
    zeal->hooks->Add("GetClickedActor", 0x004b008a, GetClickedActor, hook_type_detour);
    FARPROC eqfx = GetProcAddress(GetModuleHandleA("eqgfx_dx8.dll"), "t3dSetCameraLens");
    if (eqfx != NULL)
        zeal->hooks->Add("SetCameraLens", (int)eqfx, SetCameraLens, hook_type_detour);
    zeal->commands_hook->Add("/fov", { }, "Set your field of view requires a value between 45 and 90.",
        [this](std::vector<std::string>& args) {
            Zeal::EqStructures::CameraInfo* ci = Zeal::EqGame::get_camera();
            if (ci)
            {
                float _fov = 0;
                if (args.size() > 1 && Zeal::String::tryParse(args[1], &_fov))
                {
                    if (_fov < 45 || _fov > 90)
                    {
                        Zeal::EqGame::print_chat("Use a fov value between 45 and 90");
                        return true;
                    }

                    fov.set(_fov);
                }
                else
                {
                    Zeal::EqGame::print_chat("Current FOV [%f]", ci->FieldOfView);
                }

            }

            return true; //return true to stop the game from processing any further on this command, false if you want to just add features to an existing cmd
        });
    zeal->commands_hook->Add("/pandelay", { "/pd" }, "Adjust the delay required before left click panning happens in zeal cam.",
        [this, ini](std::vector<std::string>& args) {
            int delay = 200;
            if (args.size() == 2)
            {
                if (Zeal::String::tryParse(args[1], &delay))
                {
                    pan_delay.set(delay);
                    Zeal::EqGame::print_chat("Click to pan delay is now %i", pan_delay.get());
                }
            }
            else
                Zeal::EqGame::print_chat("Invalid arguments for pandelay example usage: /pandelay 200");
            return true;
        });
    zeal->commands_hook->Add("/selfclickthru", {}, "Disable (on) or enable (off) clicking on self.",
        [this](const std::vector<std::string>& args) {
            if (args.size() == 2 && args[1] == "on")
                setting_selfclickthru.set(true);
            else if (args.size() == 2 && args[1] == "off")
                setting_selfclickthru.set(false);
            else
                Zeal::EqGame::print_chat("Usage: /selfclickthru on or /selfclickthru off");
            Zeal::EqGame::print_chat("Selfclickthru is now %s", setting_selfclickthru.get() ? "on" : "off");
            return true;
    });
    zeal->commands_hook->Add("/zealcam", { "/smoothing" }, "Toggles the zealcam on/off as well as adjusting the sensitivities.",
        [this](std::vector<std::string>& args) {
            if (args.size() == 2 && Zeal::String::compare_insensitive(args[1], "info"))
            {
                Zeal::EqGame::print_chat("camera sensitivity FirstPerson : [% f] [% f] ThirdPerson : [% f] [% f] ", user_sensitivity_x, user_sensitivity_y, user_sensitivity_x_3rd, user_sensitivity_y_3rd);
                return true;
            }
            else if (args.size() == 3) //the first arg is the command name itself
            {
                float x_sens = 0;
                float y_sens = 0;

                if (!Zeal::String::tryParse(args[1], &x_sens))
                    return true;
                if (!Zeal::String::tryParse(args[2], &y_sens))
                    return true;

                user_sensitivity_x = x_sens;
                user_sensitivity_y = y_sens;
                user_sensitivity_x_3rd = x_sens;
                user_sensitivity_y_3rd = y_sens;
                set_smoothing(true);
                Zeal::EqGame::print_chat("New camera sensitivity [%f] [%f]", user_sensitivity_x, user_sensitivity_y);
            }
            else if (args.size() == 5)
            {
                float x_sens = 0;
                float y_sens = 0;
                float x_sens_3rd = 0;
                float y_sens_3rd = 0;

                if (!Zeal::String::tryParse(args[1], &x_sens))
                    return true;
                if (!Zeal::String::tryParse(args[2], &y_sens))
                    return true;
                if (!Zeal::String::tryParse(args[3], &x_sens_3rd))
                    return true;
                if (!Zeal::String::tryParse(args[4], &y_sens_3rd))
                    return true;


                user_sensitivity_x = x_sens;
                user_sensitivity_y = y_sens;
                user_sensitivity_x_3rd = x_sens_3rd;
                user_sensitivity_y_3rd = y_sens_3rd;
                set_smoothing(true);
                Zeal::EqGame::print_chat("New camera sensitivity FirstPerson: [%f] [%f] ThirdPerson: [%f] [%f]", user_sensitivity_x, user_sensitivity_y, user_sensitivity_x_3rd, user_sensitivity_y_3rd);
            }
            else
            {
                set_smoothing(!ZealService::get_instance()->camera_mods->enabled.get());
                if (ZealService::get_instance()->camera_mods->enabled.get())
                {
                    Zeal::EqGame::print_chat("Zealcam enabled");
                    Zeal::EqGame::print_chat("camera sensitivity FirstPerson : [% f] [% f] ThirdPerson : [% f] [% f] ", user_sensitivity_x, user_sensitivity_y, user_sensitivity_x_3rd, user_sensitivity_y_3rd);
                    if (cycle_to_zeal_cam_enabled.get())
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
    zeal->binds_hook->replace_cmd(15, [this](int state) { handle_camera_motion_binds(15, state); return false; });
    zeal->binds_hook->replace_cmd(115, [this](int state) { handle_camera_motion_binds(15, state); return false; });
    zeal->binds_hook->replace_cmd(111, [this](int state) { handle_camera_motion_binds(15, state); return false; });
    zeal->binds_hook->replace_cmd(16, [this](int state) { handle_camera_motion_binds(16, state); return false; });
    zeal->binds_hook->replace_cmd(116, [this](int state) { handle_camera_motion_binds(16, state); return false; });
    zeal->binds_hook->replace_cmd(112, [this](int state) { handle_camera_motion_binds(16, state); return false; });
    zeal->binds_hook->replace_cmd(18, [this](int state) { handle_camera_motion_binds(18, state); return false; });
    zeal->binds_hook->replace_cmd(19, [this](int state) { handle_camera_motion_binds(19, state); return false; });
    zeal->binds_hook->replace_cmd(20, [this](int state) { handle_cycle_camera_views(20, state); return false; });

}

CameraMods::~CameraMods()
{
    shutting_down = true;
    toggle_zeal_cam(false);
}
