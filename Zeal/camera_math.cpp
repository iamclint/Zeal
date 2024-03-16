#include "camera_math.h"
namespace camera_math
{
    float pitch_to_normal(float game_pitch) {
        float scaledAmplitude = (game_pitch / 128.0f) * 90.0f;
        if (scaledAmplitude > 89.99f) {
            scaledAmplitude = 89.99f;
        }
        else if (scaledAmplitude < -89.99f) {
            scaledAmplitude = -89.99f;
        }

        return scaledAmplitude;
    }
    float pitch_to_game(float zeal_pitch) {
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
    float lerp(float rawDelta, float smoothDelta, float t) {
        return std::lerp(smoothDelta, static_cast<float>(rawDelta), t);
    }
    float angle_difference(float angle1, float angle2) {
        float abs_diff = fabs(angle1 - angle2);
        if (abs_diff > 256.0f) {
            return 512.0f - abs_diff;
        }
        else {
            return abs_diff;
        }
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

    Vec3 get_cam_pos_behind(const Vec3& playerHead, float distance, float playerYaw, float pitch) {
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
}