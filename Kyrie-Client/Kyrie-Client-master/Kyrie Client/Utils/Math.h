#pragma once
#include <math.h>
#include <stdlib.h>
#include "../ImGui/imgui.h"

template<typename T>
class Vec2 {
public:
	T x = 0;
	T y = 0;
public:
	Vec2(T x = 0, T y = 0) {
		this->x = x;
		this->y = y;
	};
public:
	Vec2<T>& operator = (const Vec2<T>& v) {
		x = v.x;
		y = v.y;
		return *this;
	};
public:
	bool operator == (const Vec2<T>& v) {
		return (
			this->x == v.x &&
			this->y == v.y
			);
	};
	bool operator != (const Vec2<T>& v) {
		return (
			this->x != v.x ||
			this->y != v.y
			);
	};

	/* Multiply */

	Vec2<T> operator * (Vec2<T> mult) {
		return Vec2<T>(this->x * mult.x, this->y * mult.y);
	}

	Vec2<T> operator *= (Vec2<T> mult) {
		return Vec2<T>(this->x * mult.x, this->y * mult.y);
	}

	Vec2<T> operator * (float mult) {
		return Vec2<T>(this->x * mult, this->y * mult);
	}

	Vec2<T> operator *= (float mult) {
		return Vec2<T>(this->x * mult, this->y * mult);
	}


	/* Divide */

	Vec2<T> operator / (Vec2<T> div) {
		return Vec2<T>(this->x / div.x, this->y / div.y);
	}

	Vec2<T> operator /= (Vec2<T> div) {
		return Vec2<T>(this->x / div.x, this->y / div.y);
	}

	Vec2<T> operator / (float div) {
		return Vec2<T>(this->x / div, this->y / div);
	}

	Vec2<T> operator /= (float div) {
		return Vec2<T>(this->x / div, this->y / div);
	}

	/* Add */

	Vec2<T> operator + (Vec2<T> add) {
		return Vec2<T>(this->x + add.x, this->y + add.y);
	}

	Vec2<T> operator += (Vec2<T> add) {
		return Vec2<T>(this->x + add.x, this->y + add.y);
	}

	/* Subtract */

	Vec2<T> operator - (Vec2<T> sub) {
		return Vec2<T>(this->x - sub.x, this->y - sub.y);
	}

	Vec2<T> operator -= (Vec2<T> sub) {
		return Vec2<T>(this->x - sub.x, this->y - sub.y);
	}

public:
	Vec2<T> div(T v) {
		return Vec2<T>(x / v, y / v);
	};
	Vec2<T> div(T vX, T vY) {
		return Vec2<T>(x / vX, y / vY);
	};
	Vec2<T> div(Vec2<T>& v) {
		return Vec2<T>(x / v.x, y / v.y);
	};
public:
	Vec2<T> mul(T v) {
		return Vec2<T>(x * v, y * v);
	};
	Vec2<T> mul(T vX, T vY) {
		return Vec2<T>(x * vX, y * vY);
	};
	Vec2<T> mul(Vec2<T>& v) {
		return Vec2<T>(x * v.x, y * v.y);
	};
public:
	Vec2<T> add(T v) {
		return Vec2<T>(x + v, y + v);
	};
	Vec2<T> add(T vX, T vY) {
		return Vec2<T>(x + vX, y + vY);
	};
	Vec2<T> add(Vec2<T>& v) {
		return Vec2<T>(x + v.x, y + v.y);
	};
public:
	Vec2<T> sub(T v) {
		return Vec2<T>(x - v, y - v);
	};
	Vec2<T> sub(T vX, T vY) {
		return Vec2<T>(x - vX, y - vY);
	};
	Vec2<T> sub(Vec2<T>& v) {
		return Vec2<T>(x - v.x, y - v.y);
	};
};

template<typename T>
class Vec3 {
public:
	T x = 0;
	T y = 0;
	T z = 0;
public:
	Vec3(T x = 0, T y = 0, T z = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
	};
public:
	Vec3<T>& operator = (const Vec3<T>& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	};
public:
	bool operator == (const Vec3<T>& v) {
		return (
			this->x == v.x &&
			this->y == v.y &&
			this->z == v.z
			);
	};
	bool operator != (const Vec3<T>& v) {
		return (
			this->x != v.x ||
			this->y != v.y ||
			this->z != v.z
			);
	};
public:
	Vec3<T> div(T v) {
		return Vec3<T>(x / v, y / v, z / v);
	};
	Vec3<T> div(T vX, T vY, T vZ) {
		return Vec3<T>(x / vX, y / vY, z / vZ);
	};
	Vec3<T> div(Vec3<T>& v) {
		return Vec3<T>(x / v.x, y / v.y, z / v.z);
	};
public:
	Vec3<T> mul(T v) {
		return Vec3<T>(x * v, y * v, z * v);
	};
	Vec3<T> mul(T vX, T vY, T vZ) {
		return Vec3<T>(x * vX, y * vY, z * vZ);
	};
	Vec3<T> mul(Vec3<T>& v) {
		return Vec3<T>(x * v.x, y * v.y, z * v.z);
	};
public:
	Vec3<T> add(T v) {
		return Vec3<T>(x + v, y + v, z + v);
	};
	Vec3<T> add(T vX, T vY, T vZ) {
		return Vec3<T>(x + vX, y + vY, z + vZ);
	};
	Vec3<T> add(Vec3<T>& v) {
		return Vec3<T>(x + v.x, y + v.y, z + v.z);
	};
public:
	Vec3<T> sub(T v) {
		return Vec3<T>(x - v, y - v, z - v);
	};
	Vec3<T> sub(T vX, T vY, T vZ) {
		return Vec3<T>(x - vX, y - vY, z - vZ);
	};
	Vec3<T> sub(const Vec3<T>& v) {
		return Vec3<T>(x - v.x, y - v.y, z - v.z);
	};
	/*
	Vec3<T> sub(Vec3<float>& v) {
		return Vec3<T>(x - v.x, y - v.y, z - v.z);
	};
	*/

	float squaredlen() const { return x * x + y * y + z * z; }

	float magnitude() const { return sqrtf(squaredlen()); }

	float dist(Vec3<T>& v) {
		return this->sub(v).magnitude();
	}
};

template<typename T>
class Vec4 {
public:
	T x = 0;
	T y = 0;
	T z = 0;
	T w = 0;
public:
	Vec4(T x = 0, T y = 0, T z = 0, T w = 0) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};
public:
	Vec4<T>& operator = (const Vec4<T>& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
		return *this;
	};
public:
	bool operator == (const Vec4<T>& v) {
		return (
			this->x == v.x &&
			this->y == v.y &&
			this->z == v.z &&
			this->w == v.w
			);
	};
	bool operator != (const Vec4<T>& v) {
		return (
			this->x != v.x ||
			this->y != v.y ||
			this->z != v.z ||
			this->w != v.w
			);
	};
public:
	Vec4<T> div(T v) {
		return Vec4<T>(x / v, y / v, z / v, w / v);
	};
	Vec4<T> div(T vX, T vY, T vZ, T vW) {
		return Vec4<T>(x / vX, y / vY, z / vZ, w / vW);
	};
	Vec4<T> div(Vec4<T>& v) {
		return Vec4<T>(x / v.x, y / v.y, z / v.z, w / v.w);
	};
public:
	Vec4<T> mul(T v) {
		return Vec4<T>(x * v, y * v, z * v, w * v);
	};
	Vec4<T> mul(T vX, T vY, T vZ, T vW) {
		return Vec4<T>(x * vX, y * vY, z * vZ, w * vW);
	};
	Vec4<T> mul(Vec4<T>& v) {
		return Vec4<T>(x * v.x, y * v.y, z * v.z, w * v.w);
	};
public:
	Vec4<T> add(T v) {
		return Vec4<T>(x + v, y + v, z + v, w + v);
	};
	Vec4<T> add(T vX, T vY, T vZ, T vW) {
		return Vec4<T>(x + vX, y + vY, z + vZ, w + vW);
	};
	Vec4<T> add(Vec4<T>& v) {
		return Vec4<T>(x + v.x, y + v.y, z + v.z, w + v.w);
	};
public:
	Vec4<T> sub(T v) {
		return Vec4<T>(x - v, y - v, z - v, w - v);
	};
	Vec4<T> sub(T vX, T vY, T vZ, T vW) {
		return Vec4<T>(x - vX, y - vY, z - vZ, w - vW);
	};
	Vec4<T> sub(Vec4<T>& v) {
		return Vec4<T>(x - v.x, y - v.y, z - v.z, w - v.z);
	};

	__forceinline bool contains(const Vec2<float>& point) {
		if (point.x <= x || point.y <= y)
			return false;

		if (point.x >= z || point.y >= w)
			return false;
		return true;
	};
};

inline float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

inline float lerpSync(const float& a, const float& b, const float& duration) {
	const float result = (float)(a + (b - a) * duration * (60.f / ImGui::GetIO().Framerate));
	if (a < b && result > b) return b;
	else if (a > b && result < b) return b;
	return result;
}

struct glmatrixf {
	union {
		float v[16];
		float v_nested[4][4];
	};

	__forceinline float operator[](int i) const { return v[i]; }
	__forceinline float& operator[](int i) { return v[i]; }

#define MULMAT(row, col) v[col + row] = x[row] * y[col] + x[row + 4] * y[col + 1] + x[row + 8] * y[col + 2] + x[row + 12] * y[col + 3];

	template <class XT, class YT>
	void mul(const XT x[16], const YT y[16]) {
		MULMAT(0, 0);
		MULMAT(1, 0);
		MULMAT(2, 0);
		MULMAT(3, 0);
		MULMAT(0, 4);
		MULMAT(1, 4);
		MULMAT(2, 4);
		MULMAT(3, 4);
		MULMAT(0, 8);
		MULMAT(1, 8);
		MULMAT(2, 8);
		MULMAT(3, 8);
		MULMAT(0, 12);
		MULMAT(1, 12);
		MULMAT(2, 12);
		MULMAT(3, 12);
	}

#undef MULMAT

	glmatrixf* correct() {
		glmatrixf* newMatPtr = new glmatrixf;

		for (int i = 0; i < 4; i++) {
			newMatPtr->v[i * 4 + 0] = v[0 + i];
			newMatPtr->v[i * 4 + 1] = v[4 + i];
			newMatPtr->v[i * 4 + 2] = v[8 + i];
			newMatPtr->v[i * 4 + 3] = v[12 + i];
		}
		return newMatPtr;
	};

	inline bool OWorldToScreen(Vec3<float> origin, Vec3<float> pos, Vec2<float>& screen, Vec2<float> fov, Vec2<float> displaySize) {
		pos = pos.sub(origin);

		float x = transformx(pos);
		float y = transformy(pos);
		float z = transformz(pos);

		if (z > 0)
			return false;

		float mX = (float)displaySize.x / 2.0F;
		float mY = (float)displaySize.y / 2.0F;

		screen.x = mX + (mX * x / -z * fov.x);
		screen.y = mY - (mY * y / -z * fov.y);

		return true;
	}

	inline void mul(const glmatrixf& x, const glmatrixf& y) {
		mul(x.v, y.v);
	}

	inline void translate(float x, float y, float z) {
		v[12] += x;
		v[13] += y;
		v[14] += z;
	}

	inline void translate(const Vec3<float>& o) {
		translate(o.x, o.y, o.z);
	}

	inline void scale(float x, float y, float z) {
		v[0] *= x;
		v[1] *= x;
		v[2] *= x;
		v[3] *= x;
		v[4] *= y;
		v[5] *= y;
		v[6] *= y;
		v[7] *= y;
		v[8] *= z;
		v[9] *= z;
		v[10] *= z;
		v[11] *= z;
	}

	inline void invertnormal(Vec3<float>& dir) const {
		Vec3 n(dir);
		dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
		dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
		dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
	}

	inline void invertvertex(Vec3<float>& pos) const {
		Vec3 p(pos);
		p.x -= v[12];
		p.y -= v[13];
		p.z -= v[14];
		pos.x = p.x * v[0] + p.y * v[1] + p.z * v[2];
		pos.y = p.x * v[4] + p.y * v[5] + p.z * v[6];
		pos.z = p.x * v[8] + p.y * v[9] + p.z * v[10];
	}

	inline void transform(const Vec3<float>& in, Vec4<float>& out) const {
		out.x = transformx(in);
		out.y = transformy(in);
		out.z = transformz(in);
		out.w = transformw(in);
	}

	__forceinline float transformx(const Vec3<float>& p) const {
		return p.x * v[0] + p.y * v[4] + p.z * v[8] + v[12];
	}

	__forceinline float transformy(const Vec3<float>& p) const {
		return p.x * v[1] + p.y * v[5] + p.z * v[9] + v[13];
	}

	__forceinline float transformz(const Vec3<float>& p) const {
		return p.x * v[2] + p.y * v[6] + p.z * v[10] + v[14];
	}

	__forceinline float transformw(const Vec3<float>& p) const {
		return p.x * v[3] + p.y * v[7] + p.z * v[11] + v[15];
	}

	__forceinline Vec3<float> gettranslation() const {
		return Vec3<float>(v[12], v[13], v[14]);
	}

	//assault cube world2screen
	Vec3<float> transform(glmatrixf* matrix, Vec3<float>& totransform) {
		return Vec3<float>(matrix->transformx(totransform),
			matrix->transformy(totransform),
			matrix->transformz(totransform))
			.div(matrix->transformw(totransform));
	}

	///pos should be the exact center of the enemy model for scaling to work properly
	Vec3<float> WorldToScreen(Vec3<float> pos, int width, int height) {
		//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
		Vec4<float> clipCoords;
		clipCoords.x = pos.x * v[0] + pos.y * v[4] + pos.z * v[8] + v[12];
		clipCoords.y = pos.x * v[1] + pos.y * v[5] + pos.z * v[9] + v[13];
		clipCoords.z = pos.x * v[2] + pos.y * v[6] + pos.z * v[10] + v[14];
		clipCoords.w = pos.x * v[3] + pos.y * v[7] + pos.z * v[11] + v[15];

		//perspective division, dividing by clip.W = Normalized Device Coordinates
		Vec3<float> NDC;
		NDC.x = clipCoords.x / clipCoords.w;
		NDC.y = clipCoords.y / clipCoords.w;
		NDC.z = clipCoords.z / clipCoords.w;

		//viewport tranform to screenCooords

		Vec3<float> playerscreen;
		playerscreen.x = (width / 2 * NDC.x) + (NDC.x + width / 2);
		playerscreen.y = -(height / 2 * NDC.y) + (NDC.y + height / 2);

		return playerscreen;
	}
};