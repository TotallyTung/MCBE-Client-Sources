#pragma once

constexpr float PI = (3.1415927f);

class MathUtil
{
public:
	inline static float animate(float endPoint, float current, float speed) 
	{
		if (speed < 0.0) speed = 0.0;
		else if (speed > 1.0) speed = 1.0; 

		float dif = std::fmax(endPoint, current) - std::fmin(endPoint, current);
		float factor = dif * speed;
		return current + (endPoint > current ? factor : -factor);
	}

	inline static float lerp(float a, float b, float c) 
	{
		return a + c * (b - a);
	}

	inline static int lerpInt(int a1, int a2, double a3) 
	{
		return (int)MathUtil::lerp((float)a1, (float)a2, (float)a3);
	}

	__forceinline static float wrap(float value, float min, float max) 
	{
		return std::fmod(std::fmod(value - min, max - min) + (max - min), max - min) + min;
	}

	static inline float clamp(float value, float min_val, float max_val) 
	{
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}

	static inline float round(float num, float increment) 
	{
		if (increment == 0.0f) {
			return num;
		}
		else if (increment == 1.0f) {
			return std::round(num);
		}
		else {
			float halfOfInc = increment / 2.0f;
			float floored = std::floorf(num / increment) * increment;
			if (num >= floored + halfOfInc) {
				return std::ceilf(num / increment) * increment;
			}
			else {
				return floored;
			}
		}
	}

	static Vector2<float> getRots(const Vector3<float>& ths, const Vector3<float>& dst)
	{
		float deltaX = dst.x - ths.x;
		float deltaZ = dst.z - ths.z;
		float deltaY = dst.y - ths.y;
		float deltaXZ = hypot(deltaX, deltaZ);

		float yaw = atan2(-deltaX, deltaZ);

		float yawDegrees = yaw * (180 / PI);
		float pitch = atan2(deltaY, deltaXZ) * (180 / PI);

		return Vector2<float>(-pitch, yawDegrees);
	}

	static bool rayIntersectsAABB(Vector3<float> mRayPos, Vector3<float> mRayEnd, Vector3<float> mHitboxMin, Vector3<float> mHitboxMax)
	{
		Vector3<float> t0 = (mHitboxMin - mRayPos) / (mRayEnd - mRayPos);
		Vector3<float> t1 = (mHitboxMax - mRayPos) / (mRayEnd - mRayPos);

		Vector3<float> tmin(std::min(t0.x, t1.x), std::min(t0.y, t1.y), std::min(t0.z, t1.z));
		Vector3<float> tmax(std::max(t0.x, t1.x), std::max(t0.y, t1.y), std::max(t0.z, t1.z));

		float tminmax = std::max(tmin.x, std::max(tmin.y, tmin.z));
		float tmaxmin = std::min(tmax.x, std::min(tmax.y, tmax.z));

		return tminmax <= tmaxmin;
	}
};