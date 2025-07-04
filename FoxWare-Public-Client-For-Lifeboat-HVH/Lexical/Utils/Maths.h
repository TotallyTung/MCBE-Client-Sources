#pragma once
#define PI (3.1415927f)
#include <algorithm>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>

static constexpr float DEG_RAD2 = PI / 360.0f;
static constexpr float DEG_RAD = 180.0f / PI;
static constexpr float RAD_DEG = PI / 180.f;

template<typename T>
struct Vec2 {
	union {
		struct {
			T x, y;
		};
		T arr[2];
	};

	Vec2(T x = 0, T y = 0) {
		this->x = x;
		this->y = y;
	}

	Vec2<T> normAngles() {
		float x = this->x;
		float y = this->y;
		while (x > 90.f)
			x -= 180.f;
		while (x < -90.f)
			x += 180.f;

		while (y > 180.0f)
			y -= 360.0f;
		while (y < -180.0f)
			y += 360.0f;
		return Vec2(x, y);
	}

	Vec2<T> add(const Vec2<T>& o) const {
		return Vec2<T>(x + o.x, y + o.y);
	}

	Vec2<T> sub(const Vec2<T>& o) const {
		return Vec2<T>(x - o.x, y - o.y);
	}

	Vec2<T> mul(const Vec2<T>& o) const {
		return Vec2<T>(x * o.x, y * o.y);
	}

	Vec2<T> div(const Vec2<T>& o) const {
		return Vec2<T>(x / o.x, y / o.y);
	}

	template<typename Y>
	Vec2<Y> CastTo() const {
		return Vec2<Y>((Y)x, (Y)y);
	}
};

template<typename T>
struct Vec3 {

	union {
		struct {
			T x, y, z;
		};
		T arr[3];
	};

	Vec3(T x = 0, T y = 0, T z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	bool operator==(const Vec3<T>& other) const {
		return (x == other.x && y == other.y && z == other.z);
	}

	Vec3<T> add(const Vec3<T>& o) const {
		return Vec3<T>(x + o.x, y + o.y, z + o.z);
	}

	Vec3<T> add2(float a, float b, float c) {
		return Vec3<T>(x + a, y + b, z + c);
	}

	Vec3<T> sub(const Vec3<T>& o) const {
		return Vec3<T>(x - o.x, y - o.y, z - o.z);
	}

	Vec3<T> mul(const Vec3<T>& o) const {
		return Vec3<T>(x * o.x, y * o.y, z * o.z);
	}

	Vec3<T> div(const Vec3<T>& o) const {
		return Vec3<T>(x / o.x, y / o.y, z / o.z);
	}

	Vec3<T> floor() const {
		return Vec3<T>(floorf(x), floorf(y), floorf(z));
	};
	Vec3<int> toInt() {
		return Vec3<int>((int)x, (int)y, (int)z);
	}

	Vec3<float> toFloat() {
		return Vec3<float>((float)x, (float)y, (float)z);
	}

	float squaredlen() const {
		return (float)(x * x + y * y + z * z);
	}

	float squaredxzlen() const {
		return (float)(x * x + z * z);
	}

	float magnitude() const {
		return std::sqrtf(squaredlen());
	}

	float dist(const Vec3<float>& v3) const {
		return this->sub(v3).magnitude();
	}
	Vec3<T> lerpTo(const Vec3<T>& other, float lerpFactor) const {
		Vec3<T> result;
		result.x = x + lerpFactor * (other.x - x);
		result.y = y + lerpFactor * (other.y - y);
		result.z = z + lerpFactor * (other.z - z);
		return result;
	}
	Vec3<T> lerp(const Vec3<T>& other, float tx, float ty, float tz) const {
		Vec3<T> result;
		result.x = x + tx * (other.x - x);
		result.y = y + ty * (other.y - y);
		result.z = z + tz * (other.z - z);
		return result;
	}

	Vec2<float> CalcAngle(const Vec3<float>& dst) {
		Vec3<float> diff = dst.sub(*this);

		diff.y = diff.y / diff.magnitude();
		Vec2<float> angles;
		angles.x = asinf(diff.y) * -DEG_RAD;
		angles.y = (float)-atan2f(diff.x, diff.z) * DEG_RAD;

		return angles;
	}

	template<typename Y>
	Vec3<Y> CastTo() const {
		return Vec3<Y>((Y)x, (Y)y, (Y)z);
	}

	Vec3<T> normalize() const {
		T length = std::sqrt(x * x + y * y + z * z);
		return Vec3<T>(x / length, y / length, z / length);
	}

	Vec3<T> operator-(const Vec3<T>& other) const {
		return Vec3<T>(x - other.x, y - other.y, z - other.z);
	}

	T dot(const Vec3<T>& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	Vec3<T> cross(const Vec3<T>& other) const {
		return Vec3<T>(
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		);
	}
};

namespace std {
	template <>
	struct hash<Vec3<int>> {
		size_t operator()(const Vec3<int>& key) const {
			return std::hash<int>()(key.x) ^ std::hash<int>()(key.y) ^ std::hash<int>()(key.z);
		}
	};
}

template<typename T>
struct Vec4 {

	union {
		struct {
			T x, y, z, w;
		};
		T arr[4];
	};

	Vec4(T x = 0, T y = 0, T z = 0, T w = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	inline bool contains(const Vec2<float>& point) {
		if (point.x < x || point.y < y)
			return false;

		if (point.x > z || point.y > w)
			return false;
		return true;
	}
};

struct AABB {
	Vec3<float> lower;
	Vec3<float> upper;
	Vec2<float> size;

	Vec3<float> getCenter() {
		return (lower.add(upper)).div(Vec3<float>(2.f, 2.f, 2.f));
	}

	bool myball(AABB aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.y > lower.y && upper.y > aabb.lower.y &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}

	bool intersects(const AABB& aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.y > lower.y && upper.y > aabb.lower.y &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}

	bool intersectsXZ(const AABB& aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}
	AABB expand(const Vec3<float>& amount) const {
		AABB expanded = *this; // Kendi kopyas�n?olu�tur
		expanded.lower.x -= amount.x;
		expanded.lower.y -= amount.y;
		expanded.lower.z -= amount.z;
		expanded.upper.x += amount.x;
		expanded.upper.y += amount.y;
		expanded.upper.z += amount.z;

		// Yeni boyutu hesapla
		expanded.size.x = expanded.upper.x - expanded.lower.x;
		expanded.size.y = expanded.upper.y - expanded.lower.y;

		return expanded; // Geni�letilmi?AABB'yi d�nd�r
	}
};
typedef Vec3<int> BlockPos;

namespace Math {
	inline float lerp(float start, float end, float t) {
		return start + ((end - start) * t);
	}
	inline float moveTowards(float current, float target, float maxDelta) {
		float delta = target - current;
		if (fabs(delta) <= maxDelta) {
			return target; // Se o restante for menor que o incremento, vai direto para o target
		}
		return current + (delta > 0 ? maxDelta : -maxDelta);
	}



	inline float wrap(float value, float min, float max) {
		return fmod(fmod(value - min, max - min) + (max - min), max - min) + min;
	}

	inline int randomInt(int start, int end) {
		return rand() % (end - start + 1) + start;
	}

	inline static float calculateDistance(const Vec3<float>& vec) {
		return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	}

	inline float randomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}
}
