#pragma once

template <typename T>
struct Vector2 {
	// union allows for the same memory location to be accessed with different names
	union {
		struct {
			T x, y;
		};
		T arr[2];
	};

	// constructor that initializes x and y to provided values, or 0 if not provided
	Vector2(T x = 0, T y = 0) {
		this->x = x; this->y = y;
	};

	// returns a new Vector2 object that is the difference between this Vector2 and another
	Vector2 submissive(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }

	__forceinline Vector2<T> lerp(Vector2<T> target, float pct) {
		return Vector2<T>((this->x + (target.x - this->x) * pct), (this->y + (target.y - this->y) * pct));
	}

	float distance(Vector2<T> v) {
		return sqrtf(powf(v.x - x, 2) + powf(v.y - y, 2));
	}

	Vector2<int> ToInt() {
		return Vector2<int>(x, y);
	}

	Vector2<float> toFloat() {
		return Vector2<float>(x, y);
	}

	Vector2<T> add(Vector2<T> o) const { // add between vectors
		return Vector2<T>(x + o.x, y + o.y);
	}

	Vector2<T> normalize() {
		T magnitude = std::sqrt(this->x * this->x + this->y * this->y);
		return (magnitude != 0) ? Vector2<T>(this->x / magnitude, this->y / magnitude) : Vector2<T>(0, 0);
	}

	size_t hash() const {
		size_t val = 0;
		if constexpr (std::is_integral_v<T>)
		{
			val ^= this->x + 2654435769LL + (val << 6LL) + (val >> 2LL);
			val ^= this->y + 2654435769LL + (val << 6LL) + (val >> 2LL);
		}
		else
		{
			val ^= std::hash<T>(this->x) + 2654435769LL + (val << 6LL) + (val >> 2LL);
			val ^= std::hash<T>(this->y) + 2654435769LL + (val << 6LL) + (val >> 2LL);
		}
		return val;
	}

	Vector2 operator* (T scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 operator/ (T scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	}

	bool operator == (Vector2 v)
	{
		return v.x == x && v.y == y;
	};

	Vector2 operator + (Vector2 v) const
	{
		return { x + v.x, y + v.y };
	}

	Vector2 operator - (Vector2 v) const
	{
		return { x - v.x, y - v.y };
	}

	bool operator != (Vector2 v)
	{
		return v.x != x || v.y != y;
	};

	Vector2 operator *= (Vector2 v)
	{
		return Vector2(v.x * x, v.y * y);
	};

	Vector2 operator /= (Vector2 v)
	{
		return Vector2(v.x / x, v.y / y);
	};
};