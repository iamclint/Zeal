#pragma once
#include <cmath>
#include "vectors.h"
namespace camera_math
{
	float pitch_to_normal(float game_pitch);
	float pitch_to_game(float zeal_pitch);
	float lerp(float rawDelta, float smoothDelta, float t);
	float angle_difference(float angle1, float angle2);
	Vec3 get_cam_pos_behind(const Vec3& playerHead, float distance, float playerYaw, float pitch);
	float get_pitch(Vec3 cameraPos, Vec3 targetPos);
}
