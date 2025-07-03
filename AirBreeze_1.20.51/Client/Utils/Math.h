#pragma once
//#include <max>
constexpr float PI = (3.1415927f);
class Math
{
public:
	static const float& fdmin(const float& a, const float& b)
	{
		return (a < b) ? a : b;
	}

	inline static float animate(float endPoint, float current, float speed) {
		if (speed < 0.0)
			speed = 0.0;
		else if (speed > 1.0)
			speed = 1.0;

		float dif = std::fmax(endPoint, current) - std::fmin(endPoint, current);
		float factor = dif * speed;
		return current + (endPoint > current ? factor : -factor);
	}

	inline static float lerp(float a, float b, float c)
	{
		return a + c * (b - a);
	}

	__forceinline static float wrap(float value, float min, float max) {
		return fmod(fmod(value - min, max - min) + (max - min), max - min) + min;
	}

	inline static int lerpInt(int a1, int a2, double a3)
	{
		return (int)lerp((float)a1, (float)a2, (float)a3);
	}

	static inline float clamp(float value, float min_val, float max_val) {
		return value < min_val ? min_val : (value > max_val ? max_val : value);
	}


	static double pow(double a, double b)
	{
		double c = 1;
		for (int i = 0; i < b; i++)
		{
			c *= a;
		}
		return c;
	}

	static double fact(double x)
	{
		double ret = 1;
		for (int i = 1; i <= x; i++)
		{
			ret *= i;
		}
		return ret;
	}

	static float sin(float x)
	{
		float res = 0, pow = x, fact = 1;
		for (int i = 0; i < 5; i++)
		{
			res += pow / fact;
			pow *= -1 * x * x;
			fact *= (2 * (i + 1)) * (2 * (i + 1) + 1);
		}
		return res;
	}

	static double cos(double x)
	{
		double y = 1;
		double s = -1;
		for (int i = 2; i <= 100; i += 2)
		{
			y += s * (pow(x, i) / fact(i));
			s *= -1;
		}
		return y;
	}
};

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
	Vec3<T> sub(T v) const {
		return Vec3<T>(x - v, y - v, z - v);
	};
	Vec3<T> sub(T vX, T vY, T vZ) const {
		return Vec3<T>(x - vX, y - vY, z - vZ);
	};
	Vec3<T> sub(const Vec3<T>& v) const {
		return Vec3<T>(x - v.x, y - v.y, z - v.z);
	};
public: //IPasted from da horion
	float squaredlen() const { return x * x + y * y + z * z; }
	float squaredxzlen() const { return x * x + z * z; }

	float magnitude() const { return sqrtf(squaredlen()); }

	float dist(const Vec3<T>& e) const {
		return sub(e).magnitude();
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
};

class Rect {
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;
public:
	Rect(float top = 0.f, float left = 0.f, float right = 0.f, float bottom = 0.f) {
		this->x = top;
		this->y = left;
		this->z = right;
		this->w = bottom;
	};
public:
	auto get(void) -> float* {
		float v[] = { this->x, this->y, this->z, this->w };
		return v;
	};
};

class Color {
public:
	float r = 255.f;
	float g = 255.f;
	float b = 255.f;
	float a = 1.f;
public:
	Color(float r = 255.f, float g = 255.f, float b = 255.f, float a = 1.f) {
		this->r = r / 255.f;
		this->g = g / 255.f;
		this->b = b / 255.f;
		this->a = a;
	};
public:
	auto get(void) -> float* {
		float v[] = { this->r, this->g, this->b, this->a };
		return v;
	};
};

struct AABB {
	Vec3<float> lower;
	Vec3<float> upper;
	Vec2<float> size;
	AABB() {}
	AABB(Vec3<float> l, Vec3<float> h) : lower(l), upper(h) {};
	AABB(const AABB& aabb) {
		lower = Vec3<float>(aabb.lower);
		upper = Vec3<float>(aabb.upper);
	}



};

struct glmatrixf { //thank you horion
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
		Vec3<float> n(dir);
		dir.x = n.x * v[0] + n.y * v[1] + n.z * v[2];
		dir.y = n.x * v[4] + n.y * v[5] + n.z * v[6];
		dir.z = n.x * v[8] + n.y * v[9] + n.z * v[10];
	}

	inline void invertvertex(Vec3<float>& pos) const {
		Vec3<float> p(pos);
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