#pragma once
#include "memory.h"
#include <chrono>
#include "vectors.h"
#include "ZealSettings.h"
#include "EqFunctions.h"
class CameraMods
{
public:
	std::chrono::steady_clock::time_point prevTime;



	//cam_lock = ini->getValue<bool>("Zeal", "CamLock");
	//fov = ini->getValue<float>("Zeal", "Fov");

	ZealSetting<bool> enabled = { true, "Zeal", "MouseSmoothing", false };
	ZealSetting<bool> cam_lock = { true, "Zeal", "CamLock", false };
	ZealSetting<bool> use_old_sens = { false, "Zeal", "OldSens", false };
	ZealSetting<bool> camera3_strafe_enabled = { false, "Zeal", "Camera3StrafeEnabled", false };
	ZealSetting<bool> camera4_strafe_enabled = { false, "Zeal", "Camera4StrafeEnabled", false };
	ZealSetting<bool> cycle_to_zeal_cam_enabled = { true, "Zeal", "cycle_to_zeal_cam_enabled", false };
	ZealSetting<float> user_sensitivity_x = { 0.1f, "Zeal", "MouseSensitivityX", false };
	ZealSetting<float> user_sensitivity_y = { 0.1f, "Zeal", "MouseSensitivityY", false };
	ZealSetting<float> user_sensitivity_x_3rd = { 0.1f, "Zeal", "MouseSensitivityX3rd", false };
	ZealSetting<float> user_sensitivity_y_3rd = { 0.1f, "Zeal", "MouseSensitivityY3rd", false };
	ZealSetting<float> fov = { 45.f, "Zeal", "Fov", false, [this](float val) { Zeal::EqStructures::CameraInfo* ci = Zeal::EqGame::get_camera();	if (ci) ci->FieldOfView = val; } };
	ZealSetting<int> pan_delay = { 0, "Zeal", "PanDelay", false };


	const float max_zoom_out = 100;
	const float zoom_speed = 5.f;

	int eq_ptr = 0;
	float height;
	float fps=0;
	float zeal_cam_pitch;
	float zeal_cam_yaw;
	float current_zoom = 0.f;
	float desired_zoom = 0.f;
	void set_smoothing(bool val);
	void callback_main();
	void callback_render();
	void callback_characterselect();
	void callback_endmainloop();
	bool update_cam(); //returns true on collision
	void mouse_wheel(int delta);
	void update_zoom(float zoom);
	void toggle_zeal_cam(bool enabled, bool reset_pitch = true);
	void proc_mouse();
	void handle_camera_motion_binds(int cmd, bool is_down);
	void handle_cycle_camera_views(int cmd, bool is_down);
	void proc_rmousedown(int x, int y);
	CameraMods(class ZealService* pHookWrapper, class IO_ini* ini);
	~CameraMods();
private:
	bool main_loop_ended = false;
	bool check_snap = false;
	ULONGLONG lmouse_time = 0;
	POINT lmouse_cursor_pos;
	bool hide_cursor = false;
	float sensitivity_x = 0.7f;
	float sensitivity_y = 0.4f;
	void interpolate_zoom();
	std::unordered_map<int, bool> cmd_key_map;
	BYTE original_cam[6] = { 0 };
	std::chrono::steady_clock::time_point lastTime;
	Vec2 local_delta = { 0, 0 };
	bool shutting_down = false;
	void tick_key_move();
	void update_fps_sensitivity();
};
	

