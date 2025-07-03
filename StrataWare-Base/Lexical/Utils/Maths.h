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

	Vec3<T> operator*(const T scalar) const {
		return Vec3<T>(x * scalar, y * scalar, z * scalar);
	}

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
	template<typename T>
	inline T min(T a, T b) {
		return (a < b) ? a : b;
	}

	template<typename T>
	inline T clamp(T value, T min, T max) {
		if (value < min) return min;
		if (value > max) return max;
		return value;
	}

	inline float clamp(float value, float min, float max, float epsilon = FLT_EPSILON) {
		if (value - min < -epsilon) return min;
		if (value - max > epsilon) return max;
		return value;
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

	inline Vec2<float> smoothRotation(const Vec2<float>& from, const Vec2<float>& to, float t) {
		Vec2<float> diff;
		diff.x = to.x - from.x;
		diff.y = to.y - from.y;
		if (diff.x > 180) diff.x -= 360;
		if (diff.x < -180) diff.x += 360;
		if (diff.y > 180) diff.y -= 360;
		if (diff.y < -180) diff.y += 360;
		float easedT = std::sin(t * 3.14159f / 2);
		Vec2<float> result;
		result.x = from.x + diff.x * easedT;
		result.y = from.y + diff.y * easedT;
		return result;
	}
}
template<typename T>
struct Vec5 {
	union {
		struct {
			T x, y, z, w, v;
		};
		T arr[5];
	};

	// Constructor
	Vec5(T x = 0, T y = 0, T z = 0, T w = 0, T v = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
		this->v = v;
	}
	Vec5<T> add(const Vec5<T>& o) const {
		return Vec5<T>(x + o.x, y + o.y, z + o.z, w + o.w, v + o.v);
	}
	Vec5<T> sub(const Vec5<T>& o) const {
		return Vec5<T>(x - o.x, y - o.y, z - o.z, w - o.w, v - o.v);
	}
	Vec5<T> mul(const T scalar) const {
		return Vec5<T>(x * scalar, y * scalar, z * scalar, w * scalar, v * scalar);
	}
	Vec5<T> div(const T scalar) const {
		return Vec5<T>(x / scalar, y / scalar, z / scalar, w / scalar, v / scalar);
	}
	float magnitude() const {
		return std::sqrt(x * x + y * y + z * z + w * w + v * v);
	}
	Vec5<T> normalize() const {
		float mag = magnitude();
		return Vec5<T>(x / mag, y / mag, z / mag, w / mag, v / mag);
	}
	T dot(const Vec5<T>& o) const {
		return x * o.x + y * o.y + z * o.z + w * o.w + v * o.v;
	}
	template<typename Y>
	Vec5<Y> CastTo() const {
		return Vec5<Y>((Y)x, (Y)y, (Y)z, (Y)w, (Y)v);
	}
	Vec5<T> floor() const {
		return Vec5<T>(std::floor(x), std::floor(y), std::floor(z), std::floor(w), std::floor(v));
	}
};
template<typename T>
struct Vec1 {
	T x;
	Vec1(T x = 0) {
		this->x = x;
	}
	Vec1<T> add(const Vec1<T>& o) const {
		return Vec1<T>(x + o.x);
	}
	Vec1<T> sub(const Vec1<T>& o) const {
		return Vec1<T>(x - o.x);
	}
	Vec1<T> mul(const T scalar) const {
		return Vec1<T>(x * scalar);
	}
	Vec1<T> div(const T scalar) const {
		return Vec1<T>(x / scalar);
	}
	float magnitude() const {
		return std::fabs(x);
	}
	Vec1<T> normalize() const {
		if (x != 0) {
			return Vec1<T>(x / std::fabs(x));
		}
		return *this;
	}
	template<typename Y>
	Vec1<Y> CastTo() const {
		return Vec1<Y>((Y)x);
	}
};
template<typename T>
struct Mat3 {
	T m[3][3];
	Mat3() {
		std::memset(m, 0, sizeof(m));
	}
	Mat3(std::initializer_list<T> values) {
		auto it = values.begin();
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				m[i][j] = *it++;
			}
		}
	}
	Vec3<T> multiply(const Vec3<T>& v) const {
		return Vec3<T>(
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
		);
	}
	Mat3<T> multiply(const Mat3<T>& other) const {
		Mat3<T> result;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.m[i][j] = m[i][0] * other.m[0][j] +
					m[i][1] * other.m[1][j] +
					m[i][2] * other.m[2][j];
			}
		}
		return result;
	}
	T determinant() const {
		return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
			m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
			m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
	}
	Mat3<T> transpose() const {
		Mat3<T> result;
		for (int i = 0; i < 3; ++i) {
			for (int j = 0; j < 3; ++j) {
				result.m[i][j] = m[j][i];
			}
		}
		return result;
	}
};
template<typename T>
struct Mat4 {
	T m[4][4];
	Mat4() {
		std::memset(m, 0, sizeof(m));
	}
	Mat4(std::initializer_list<T> values) {
		auto it = values.begin();
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				m[i][j] = *it++;
			}
		}
	}
	Vec4<T> multiply(const Vec4<T>& v) const {
		return Vec4<T>(
			m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3] * v.w,
			m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3] * v.w,
			m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3] * v.w,
			m[3][0] * v.x + m[3][1] * v.y + m[3][2] * v.z + m[3][3] * v.w
		);
	}
	Mat4<T> multiply(const Mat4<T>& other) const {
		Mat4<T> result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m[i][0] * other.m[0][j] +
					m[i][1] * other.m[1][j] +
					m[i][2] * other.m[2][j] +
					m[i][3] * other.m[3][j];
			}
		}
		return result;
	}
	T determinant() const {
		return m[0][0] * m[1][1] * m[2][2] * m[3][3];
	}
	Mat4<T> transpose() const {
		Mat4<T> result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result.m[i][j] = m[j][i];
			}
		}
		return result;
	}
};
template<typename T>
struct Quaternion {
	T x, y, z, w;
	Quaternion(T x = 0, T y = 0, T z = 0, T w = 1) : x(x), y(y), z(z), w(w) {}
	Quaternion<T> multiply(const Quaternion<T>& other) const {
		return Quaternion<T>(
			w * other.x + x * other.w + y * other.z - z * other.y,
			w * other.y - x * other.z + y * other.w + z * other.x,
			w * other.z + x * other.y - y * other.x + z * other.w,
			w * other.w - x * other.x - y * other.y - z * other.z
		);
	}
	Quaternion<T> conjugate() const {
		return Quaternion<T>(-x, -y, -z, w);
	}
	Quaternion<T> normalize() const {
		T len = std::sqrt(x * x + y * y + z * z + w * w);
		return Quaternion<T>(x / len, y / len, z / len, w / len);
	}
	Mat3<T> toMatrix() const {
		Mat3<T> result;
		result.m[0][0] = 1 - 2 * (y * y + z * z);
		result.m[0][1] = 2 * (x * y - w * z);
		result.m[0][2] = 2 * (x * z + w * y);
		result.m[1][0] = 2 * (x * y + w * z);
		result.m[1][1] = 1 - 2 * (x * x + z * z);
		result.m[1][2] = 2 * (y * z - w * x);
		result.m[2][0] = 2 * (x * z - w * y);
		result.m[2][1] = 2 * (y * z + w * x);
		result.m[2][2] = 1 - 2 * (x * x + y * y);
		return result;
	}
};
template<typename T>
struct LineSegment {
	Vec3<T> start, end;
	LineSegment(const Vec3<T>& start = Vec3<T>(), const Vec3<T>& end = Vec3<T>())
		: start(start), end(end) {
	}
	float length() const {
		return start.dist(end);
	}
	bool contains(const Vec3<T>& point) const {
		return (point.x >= std::min(start.x, end.x) && point.x <= std::max(start.x, end.x)) &&
			(point.y >= std::min(start.y, end.y) && point.y <= std::max(start.y, end.y)) &&
			(point.z >= std::min(start.z, end.z) && point.z <= std::max(start.z, end.z));
	}
};
template<typename T>
struct BoundingSphere {
	Vec3<T> center;
	T radius;
	BoundingSphere(const Vec3<T>& center = Vec3<T>(), T radius = 0)
		: center(center), radius(radius) {
	}
	bool intersects(const BoundingSphere<T>& other) const {
		T distance = center.dist(other.center);
		return distance <= (radius + other.radius);
	}
	bool contains(const Vec3<T>& point) const {
		return center.dist(point) <= radius;
	}
};
