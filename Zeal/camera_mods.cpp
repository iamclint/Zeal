#include "camera_mods.h"
#include "EqFunctions.h"
#include "EqStructures.h"
#include "EqAddresses.h"
#include "Zeal.h"
#include <thread>
int zoom_speed = 3;
void reset_zoom()
{
    Zeal::EqGame::camera[*Zeal::EqGame::camera_view].zoom = Zeal::EqGame::get_self()->ModelHeightOffset+ zoom_speed+5;
}

static int desired_zoom = 0;
static float desired_height = 0;
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


float get_pitch(Vec3 cameraPos, Vec3 targetPos)
{

    // Calculate the direction vector from the camera to the target
    Vec3 direction = targetPos - cameraPos;

    // Calculate the horizontal distance
    float horizontalDistance = direction.Length2D();//  glm::length(glm::vec2(direction.x, direction.y));

    // Calculate the vertical distance
    float verticalDistance = direction.z;

    // Calculate the pitch angle in radians
    float pitch = std::atan2(verticalDistance, horizontalDistance);

    // Map the pitch to the desired range of -120 to 120 degrees
    float pitchDegrees = pitch * (180.0 / M_PI);

    float n = 120 * (fabs(pitchDegrees) / 90.f);
    if (pitchDegrees < 0)
        pitchDegrees = -n;
    else
        pitchDegrees = n;

    return pitchDegrees;
}

void adjust_camera_height(float previous_height, float current_height) {
    Vec3 pos2 = Zeal::EqGame::get_self()->Position;
    pos2.z += Zeal::EqGame::get_self()->ModelHeightOffset;
    float orig_height = previous_height - pos2.z;
    float new_height = current_height - pos2.z;

    Zeal::EqGame::camera[zeal_cam].height = orig_height+(new_height-orig_height);
}


void fix_pitch()
{
    Zeal::EqGame::get_camera_location();
    Vec3 pos1 = *(Vec3*)((*(int*)Zeal::EqGame::Display) + 0x1C);
    Vec3 pos2 = Zeal::EqGame::get_self()->Position;
    pos2.z += Zeal::EqGame::get_self()->ModelHeightOffset;
    Zeal::EqGame::camera[zeal_cam].pitch = get_pitch(pos1, pos2);
}

void handle_zoom(int delta)
{

    DWORD camera_view = *Zeal::EqGame::camera_view;
    FLOAT cameraThirdPersonZoomMax = 200;
    DWORD *zoomAddress = NULL;
            
    if (delta == 120)
    {
        if (camera_view != Zeal::EqEnums::CameraView::FirstPerson)
        {
            if (Zeal::EqGame::camera[zeal_cam].zoom <= Zeal::EqGame::get_self()->ModelHeightOffset+5)
            {
                if (camera_view == zeal_cam)
                {
                    *Zeal::EqGame::camera_view = Zeal::EqEnums::CameraView::FirstPerson;
                    reset_zoom();
                }
                        
            }
            else
            {
                if (camera_view != Zeal::EqEnums::CameraView::FirstPerson)
                {
                    desired_zoom -= zoom_speed;
                               
                }
            }
        }
    }
    else if (delta == -120)
    {
        if (camera_view == Zeal::EqEnums::CameraView::FirstPerson)
        {
            *Zeal::EqGame::camera_view = zeal_cam;
            reset_zoom();
            Zeal::EqGame::camera[zeal_cam].heading = Zeal::EqGame::get_self()->Heading-260; //put the camera directly behind your head
            Zeal::EqGame::camera[zeal_cam].height = Zeal::EqGame::get_self()->ModelHeightOffset; //put the camera level flat out
        }
        else 
        {
            desired_zoom += zoom_speed;
        }
                
    }
}



int handle_mouse_wheel(int delta)
{
    if (!Zeal::EqGame::is_mouse_hovering_window() && ZealService::get_instance()->camera_mods->smoothing)
    {
        handle_zoom(delta);
        return 0;
    }
    else
        return ZealService::get_instance()->hooks->hook_map["HandleMouseWheel"]->original(handle_mouse_wheel)(delta);
}

float Smooth(short rawDelta, float smoothDelta) {
    return std::lerp(smoothDelta, static_cast<float>(rawDelta), 1.0f / 1.5f);
}



void __fastcall procMouse(int eq, int unused, int a1)
{

    ZealService* zeal = ZealService::get_instance();
    if (eq)
        zeal->camera_mods->eq_ptr = eq;
    if (zeal->camera_mods->smoothing) //maybe add some setting to toggle this feature on and off
    {
        static float smoothMouseDeltaX = 0;
        static float smoothMouseDeltaY = 0;
        DWORD camera_view = *Zeal::EqGame::camera_view;
        Zeal::EqStructures::MouseDelta* delta = (Zeal::EqStructures::MouseDelta*)0x798586;
        Zeal::EqStructures::MouseDelta local_delta = *delta;
        Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
        if (!self)
            return;
        if (can_move() && *Zeal::EqGame::is_right_mouse_down)
        {
            smoothMouseDeltaX = Smooth(delta->x * zeal->camera_mods->sensitivity_x, smoothMouseDeltaX);
            smoothMouseDeltaY = Smooth(delta->y * zeal->camera_mods->sensitivity_y, smoothMouseDeltaY);
            delta->y = 0;
            delta->x = 0;
            if (camera_view != Zeal::EqEnums::CameraView::FirstPerson)
            {
                Zeal::EqGame::camera[camera_view].heading -= smoothMouseDeltaX;
                self->Heading = Zeal::EqGame::get_camera()->Heading;
            }
            if (fabs(smoothMouseDeltaY) > 0)
            {
                if (camera_view == Zeal::EqEnums::CameraView::ThirdPerson3)
                    Zeal::EqGame::camera[camera_view].height += smoothMouseDeltaY / 2;
                else
                    self->Pitch -= smoothMouseDeltaY;

                if (self->Pitch > 128)
                    self->Pitch = 128;
                if (self->Pitch < -128)
                    self->Pitch = -128;
            }

            if (fabs(smoothMouseDeltaX) > 0.1)
            {
                self->MovementSpeedHeading = -smoothMouseDeltaX;
            }
            else
                self->MovementSpeedHeading = 0;
        }

        //if (camera_view >= 3)
        //{
        //    Zeal::EqGame::camera[camera_view].heading = Zeal::EqGame::EqGameInternal::fix_heading(Zeal::EqGame::camera[camera_view].heading);
        //    self->Heading = Zeal::EqGame::camera[camera_view].heading;
        //}
    }
    /*else
    {
        if (uk!=123344)
           zeal->hooks->hook_map["procMouse"]->original(procMouse)(uk, unused, a1);
        return;
    }*/
    zeal->hooks->hook_map["procMouse"]->original(procMouse)(eq, unused, a1);
}

void __fastcall ProcessControls(int t, int unused)
{
    if (!ZealService::get_instance()->camera_mods->smoothing)
    {
        ZealService::get_instance()->hooks->hook_map["ProcessControls"]->original(ProcessControls)(t, unused);
        return;
    }

    DWORD camera_view = *Zeal::EqGame::camera_view;
    ZealService::get_instance()->hooks->hook_map["ProcessControls"]->original(ProcessControls)(t,unused);

    if (!(*Zeal::EqGame::is_right_mouse_down) && camera_view == Zeal::EqEnums::CameraView::ThirdPerson3)
    {
        Zeal::EqStructures::Entity* self = Zeal::EqGame::get_self();
        if (can_move() && (Zeal::EqGame::KeyInput->left || Zeal::EqGame::KeyInput->right))
            Zeal::EqGame::camera[camera_view].heading = Zeal::EqGame::get_self()->Heading - 260;
    }
    if (camera_view == Zeal::EqEnums::CameraView::ThirdPerson3)
    {
        static float prev_z;
        fix_pitch();
        if (desired_zoom!=0)
        {
            if (prev_z)
                adjust_camera_height(prev_z, ((Vec3*)((*(int*)Zeal::EqGame::Display) + 0x1C))->z);
            prev_z = ((Vec3*)((*(int*)Zeal::EqGame::Display) + 0x1C))->z;

        //   
        }
        if (desired_zoom > 0)
        {
            Zeal::EqGame::camera[camera_view].zoom += 1;
            desired_zoom--;

            Zeal::EqGame::camera[zeal_cam].height += .5;
        }
        else if (desired_zoom < 0)
        {
            Zeal::EqGame::camera[camera_view].zoom -= 1;
            desired_zoom++;
            if (Zeal::EqGame::camera[zeal_cam].height)
                Zeal::EqGame::camera[zeal_cam].height -= .5;
        }


               
        //Zeal::EqGame::camera[camera_view].height += (float)(local_delta.y) * .5f;
        if (Zeal::EqGame::camera[camera_view].zoom > 200)
            Zeal::EqGame::camera[camera_view].zoom = 200;
    }

}

CameraMods::~CameraMods()
{
    mem::write<float>(Zeal::EqGame::max_pitch, 30.0f);
    mem::write<byte>(0x53fa50, 02);
    mem::write<byte>(0x53f648, 02);
}

void CameraMods::set_smoothing(bool val)
{
    smoothing = val;
    ZealService::get_instance()->ini->setValue<bool>("Zeal", "MouseSmoothing", smoothing);
}
void CameraMods::callback_main()
{

}
CameraMods::CameraMods(ZealService* zeal)
{
    height = 0;
    static float max_pitch = 90.f;
    mem::write<int>(0x4db6e3, (int)&max_pitch); //the maximum pitch when zoomed out
    mem::write<float>(Zeal::EqGame::max_pitch, 90.0f);
    mem::write<byte>(0x53fa50, 03); //change strafe 
    mem::write<byte>(0x53f648, 03);
    zeal->main_loop_hook->add_callback([this]() { callback_main();  });
	zeal->hooks->Add("HandleMouseWheel", Zeal::EqGame::EqGameInternal::fn_handle_mouseweheel, handle_mouse_wheel, hook_type_detour);
    zeal->hooks->Add("procMouse", 0x537707, procMouse, hook_type_detour);
    zeal->hooks->Add("ProcessControls", 0x53f337, ProcessControls, hook_type_detour);
    //std::thread test([]() {
    //    mem::mem_set(0x5243D5, 0x90, 5);//processmousevent
    //    mem::mem_set(0x524392, 0x90, 5); //processmousevent
    //    mem::mem_set(0x53fc9d, 0x90, 5); //proc mouse
    //    while (!ZealService::get_instance()->exit)
    //    {
    //        //static POINT prev_mouse_pos;
    //        //static bool was_mouse_down = false;
    //      //  if (*Zeal::EqGame::is_right_mouse_down)
    //       // {
    //        if (*(int*)0x809478)
    //            Zeal::EqGame::EqGameInternal::ProcessMouseEvent();
    //            procMouse(0, 0, 0);
    //        //}
    //        Sleep(5);
    //    }

    //   });
    //test.detach();
//      Hooks->Add("HandleLook", 0x53f337, handle_key_move, hook_type_detour);
}
