#pragma once
#include "memory.h"
#include <chrono>
#include "vectors.h"

class CameraMods
{
public:
	std::chrono::steady_clock::time_point prevTime;
	const float max_zoom_out = 100;
	float user_sensitivity_x = 0.7f;
	float user_sensitivity_y = 0.3f;
	float user_sensitivity_x_3rd = 0.7f;
	float user_sensitivity_y_3rd = 0.3f;
	bool camera3_strafe_enabled = true;
	bool camera4_strafe_enabled = true;
	bool cycle_to_zeal_cam_enabled = true;
	int eq_ptr = 0;
	bool enabled;
	float height;
	float fps;
	float zeal_cam_pitch;
	float zeal_cam_yaw;
	float current_zoom = 0.f;
	float zoom_speed = 5.f;
	float desired_zoom = 0.f;
	void set_smoothing(bool val);
	void callback_main();
	bool update_cam(); //returns true on collision
	void mouse_wheel(int delta);
	void update_zoom(float zoom);
	void toggle_zeal_cam(bool enabled);
	void proc_mouse();
	void handle_camera_motion_binds(int cmd, bool is_down);
	void handle_cycle_camera_views(int cmd, bool is_down);
	CameraMods(class ZealService* pHookWrapper, class IO_ini* ini);
	~CameraMods();
private:
	bool check_snap = false;
	ULONGLONG lmouse_time = 0;
	POINT lmouse_cursor_pos;
	bool hide_cursor = false;
	float sensitivity_x = 0.7f;
	float sensitivity_y = 0.4f;
	void load_settings(class IO_ini* ini);
	void interpolate_zoom();
	int current_key_cmd = 0;
	Vec2 previous_mouse_pos;
	Vec2 mouse_delta;
	BYTE original_cam[6] = { 0 };
	std::chrono::steady_clock::time_point lastTime;
	void tick_key_move();
	void update_fps_sensitivity();
};
	

