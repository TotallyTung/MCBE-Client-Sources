#pragma once
//
// Created by vastrakai on 10/1/2024.
//

#define DEG_RAD 0.017453292519943295769236907684886f

struct Vec2 {
	float x, y;
	Vec2() { x = y = 0; }
	Vec2(const float a, const float b) : x(a), y(b) {}
	Vec2(const int a, const int b) : x(static_cast<float>(a)), y(static_cast<float>(b)) {}

	static Vec2 fromAngle(const float angle) {
		return {-sin(angle), cos(angle)};
	}

	bool operator==(const Vec2 &o) const { return x == o.x && y == o.y; }
	bool operator!=(const Vec2 &o) const { return x != o.x || y != o.y; }
	Vec2 operator-(const Vec2& o) const { return {x - o.x, y - o.y}; }

	[[nodiscard]] Vec2 sub(const float f) const {
		return {x - f, y - f};
	}
	[[nodiscard]] Vec2 sub(const float ox, const float oy) const {
		return {x - ox, y - oy};
	}
	[[nodiscard]] Vec2 div(const float f) const {
		return {x / f, y / f};
	}
	[[nodiscard]] Vec2 div(const Vec2 &o) const {
		return {x / o.x, y / o.y};
	}
	[[nodiscard]] Vec2 mul(const Vec2 &o) const {
		return {x * o.x, y * o.y};
	}
	[[nodiscard]] Vec2 mul(const float f) const {
		return {x * f, y * f};
	}
	[[nodiscard]] Vec2 sub(const Vec2 &o) const {
		return {x - o.x, y - o.y};
	}
	[[nodiscard]] Vec2 add(const Vec2 &o) const {
		return {x + o.x, y + o.y};
	}
	[[nodiscard]] Vec2 add(const float o) const {
		return {x + o, y + o};
	}
	[[nodiscard]] Vec2 add(const float ox, const float oy) const {
		return {x + ox, y + oy};
	}

	[[nodiscard]] Vec2 normalized() const {
		return div(magnitude());
	}

	[[nodiscard]] Vec2 cross() const {
		return {-y, x};
	}

	[[nodiscard]] Vec2 clamp() const {
		return {x > 90 ? 90 : (x < -90 ? -90 : x), y > 180 ? 180 : (x < -180 ? -180 : y)};
	}

	[[nodiscard]] float dot(const float ox, const float oy) const { return x * ox + y * oy; }

	[[nodiscard]] float dot(const Vec2 &o) const { return x * o.x + y * o.y; }

	[[nodiscard]] Vec2 normAngles() const {
		float x = this->x;
		float y = this->y;
		while (x > 90.f)
			x -= 180.0f;
		while (x < -90.f)
			x += 180.0f;

		while (y > 180.0f)
			y -= 360.0f;
		while (y < -180.0f)
			y += 360.0f;
		return {x, y};
	}

	[[nodiscard]] float squaredlen() const { return x * x + y * y; }
	[[nodiscard]] float magnitude() const { return sqrtf(squaredlen()); }
};

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(const int a, const int b, const int c) : x(static_cast<float>(a)), y(static_cast<float>(b)), z(static_cast<float>(c)) {}
    Vec3(const double a, const double b, const double c) : x(static_cast<float>(a)), y(static_cast<float>(b)), z(static_cast<float>(c)) {}
    Vec3(const float a, const float b, const float c) : x(a), y(b), z(c) {}
    Vec3(const float a, const float b) : x(a), y(b), z(0) {}
    Vec3(const Vec2 &copy, const float zP) : x(copy.x), y(copy.y), z(zP) {}

    bool operator==(const Vec3 &o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator!=(const Vec3 &o) const { return x != o.x || y != o.y || z != o.z; }
    Vec3 operator-() const { return {-x, -y, -z}; }

    Vec3 &operator+=(const Vec3 &o) noexcept {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    Vec3 &operator-=(const Vec3 &o) noexcept {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }
    Vec3 &operator*=(const float f) noexcept {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }
    Vec3 &operator/=(const float f) noexcept {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    Vec3 operator+(const Vec3 &o) const {
        return {x + o.x, y + o.y, z + o.z};
    }

    Vec3 operator-(const Vec3 &o) const {
        return {x - o.x, y - o.y, z - o.z};
    }

    [[nodiscard]] bool iszero() const { return x == 0 && y == 0 && z == 0; }

    [[nodiscard]] Vec3 mul(const Vec3 &o) const {
        return {x * o.x, y * o.y, z * o.z};
    }
    [[nodiscard]] Vec3 mul(const float f) const {
        return {x * f, y * f, z * f};
    }
    [[nodiscard]] Vec3 mul(const float x1, const float y1, const float z1) const {
        return {x * x1, y * y1, z * z1};
    }
    [[nodiscard]] Vec3 div(const float f) const {
        return {x / f, y / f, z / f};
    }
    [[nodiscard]] Vec3 div(const float x1, const float y1, const float z1) const {
        return {x / x1, y / y1, z / z1};
    }
    [[nodiscard]] Vec3 div(const Vec3 &o) const {
        return {x / o.x, y / o.y, z / o.z};
    }
    [[nodiscard]] Vec3 add(const float f) const {
        return {x + f, y + f, z + f};
    }
    [[nodiscard]] Vec3 add(const float x1, const float y1, const float z1) const {
        return {x + x1, y + y1, z + z1};
    }
    [[nodiscard]] Vec3 sub(const float f) const {
        return {x - f, y - f, z - f};
    }
    [[nodiscard]] Vec3 sub(const float x1, const float y1, const float z1) const {
        return {x - x1, y - y1, z - z1};
    }

    [[nodiscard]] Vec3 floor() const {
        return {std::floor(x), std::floor(y), std::floor(z)};
    }

    [[nodiscard]] Vec3 ceil() const {
        return {std::ceil(x), std::ceil(y), std::ceil(z)};
    }

    [[nodiscard]] Vec3 add(const Vec3 &o) const {
        return {x + o.x, y + o.y, z + o.z};
    }
    [[nodiscard]] Vec3 sub(const Vec3 &o) const {
        return {x - o.x, y - o.y, z - o.z};
    }

    [[nodiscard]] float squaredlen() const { return x * x + y * y + z * z; }
    [[nodiscard]] float squaredxzlen() const { return x * x + z * z; }

    [[nodiscard]] Vec3 lerp(const Vec3& other, const float tx, const float ty, const float tz) const {
        Vec3 ne;
        ne.x = x + tx * (other.x - x);
        ne.y = y + ty * (other.y - y);
        ne.z = z + tz * (other.z - z);
        return ne;
    }

    [[nodiscard]] Vec3 lerp(const Vec3 other, const float val) const {
        Vec3 ne;
        ne.x = x + val * (other.x - x);
        ne.y = y + val * (other.y - y);
        ne.z = z + val * (other.z - z);
        return ne;
    }

    Vec3 lerp(const Vec3 *other, const float val) const {
        Vec3 ne;
        ne.x = x + val * (other->x - x);
        ne.y = y + val * (other->y - y);
        ne.z = z + val * (other->z - z);
        return ne;
    }

    [[nodiscard]] float sqrxy() const { return x * x + y * y; }

    [[nodiscard]] float dot(const Vec3 &o) const { return x * o.x + y * o.y + z * o.z; }
    [[nodiscard]] float dotxy(const Vec3 &o) const { return x * o.x + y * o.y; }

    [[nodiscard]] float magnitude() const { return std::sqrt(squaredlen()); }

    [[nodiscard]] Vec3 normalize() const {
        return div(magnitude());
    }

    [[nodiscard]] float dist(const Vec3 &e) const {
        return sub(e).magnitude();
    }

    [[nodiscard]] float Get2DDist(const Vec3 &e) const {
        const float dx = e.x - x;
        const float dy = e.y - y;
        return std::sqrt(dx * dx + dy * dy);
    }

    [[nodiscard]] float magnitudexy() const { return std::sqrt(x * x + y * y); }
    [[nodiscard]] float magnitudexz() const { return std::sqrt(x * x + z * z); }

    [[nodiscard]] Vec3 cross(const Vec3 &b) const {
        return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x};
    }
    [[nodiscard]] float cxy(const Vec3 &a) const { return x * a.y - y * a.x; }

    [[nodiscard]] Vec2 CalcAngle(const Vec3& dst) const {
        Vec3 diff = dst.sub(*this);

        diff.y = diff.y / diff.magnitude();
        Vec2 angles;
        angles.x = std::asin(diff.y) * -DEG_RAD;
        angles.y = -std::atan2(diff.x, diff.z) * DEG_RAD;
        return angles;
    }

    static const Vec3 ZERO;

	[[nodiscard]] glm::vec3 toGlm() const {
		return {x, y, z};
	}
};


inline const Vec3 Vec3::ZERO(0, 0, 0);

struct Vec3i {
	int x, y, z;

	Vec3i(glm::ivec3 vec) : x(vec.x), y(vec.y), z(vec.z) {}
	Vec3i() { x = y = z = 0; }
	Vec3i(const int a, const int b, const int c) : x(a), y(b), z(c) {}
	Vec3i(const int a, const int b) : x(a), y(b), z(0) {}
	Vec3i(const Vec3 &copy) : x(static_cast<int>(copy.x)), y(static_cast<int>(copy.y)), z(static_cast<int>(copy.z)) {}

	Vec3i(const int *v) : x(v[0]), y(v[1]), z(v[2]) {}

	[[nodiscard]] Vec3 toVec3t() const {
		return {x, y, z};
	}

	[[nodiscard]] bool iszero() const { return x == 0 && y == 0 && z == 0; }

	bool operator==(const Vec3i &o) const { return x == o.x && y == o.y && z == o.z; }
	bool operator!=(const Vec3i &o) const { return x != o.x || y != o.y || z != o.z; }
    bool operator<(const Vec3i& o) const {
        if (x != o.x) return x < o.x;
        if (y != o.y) return y < o.y;
        return z < o.z;
    }

	[[nodiscard]] Vec3i add(const Vec3i& o) const {
		return {x + o.x, y + o.y, z + o.z};
	}

	[[nodiscard]] Vec3i add(const int f) const {
		return {x + f, y + f, z + f};
	}

	[[nodiscard]] Vec3i add(const int a, const int b, const int c) const {
		return {x + a, y + b, z + c};
	}

	[[nodiscard]] Vec3i sub(const int ox, const int oy, const int oz) const {
		return {x - ox, y - oy, z - oz};
	}

	[[nodiscard]] Vec3i sub(const Vec3i& o) const {
		return {x - o.x, y - o.y, z - o.z};
	}

    [[nodiscard]] float squaredlen() const {
		return x * x + y * y + z * z;
	}

	[[nodiscard]] float magnitude() const {
		return std::sqrt(squaredlen());
	}

	[[nodiscard]] float dist(const Vec3i& e) const {
		return sub(e).magnitude();
	}

	void set(const Vec3i* o) {
		x = o->x;
		y = o->y;
		z = o->z;
	}

	[[nodiscard]] Vec3 toFloatVector() const {
		Vec3 vec;
		vec.x = static_cast<float>(x);
		vec.y = static_cast<float>(y);
		vec.z = static_cast<float>(z);
		return vec;
	}

	[[nodiscard]] glm::ivec3 toGlm() const
	{
		return {x, y, z};
	}
};

struct Vec4 {
	float x, y, z, w;
	Vec4() { x = 0, y = 0, z = 0, w = 0; }
	Vec4(const Vec3 &p, const float w = 0) : x(p.x), y(p.y), z(p.z), w(w){}
	Vec4(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w){}

	__forceinline bool contains(const Vec2* point) const {
		if (point->x <= x || point->y <= y)
			return false;

		if (point->x >= z || point->y >= w)
			return false;
		return true;
	}

	[[nodiscard]] __forceinline Vec4 shrink(const float amount) const {
		return {this->x + amount, this->y + amount, this->z - amount, this->w - amount};
	}
};

class AutoPath : public ModuleBase<AutoPath>
{
    std::vector<glm::vec3> mPosList{};
    int mTicks = 0;
    std::mutex mMutex;

public:
    NumberSetting mHowClose = NumberSetting("How Close", "deborg", 1.f, 0.f, 10.f, 0.01f);
	BoolSetting mDebug = BoolSetting("Debug", "deborg", false);
	NumberSetting mPathTimeout = NumberSetting("Path Timeout", "The millisecond timeout for pathfinding", 0.2f, 0, 2.f, 0.01f);

    AutoPath() : ModuleBase("AutoPath", "Automatically pathfinds to a specified location", ModuleCategory::Movement, 0, false) {
        addSettings(&mHowClose);
    	addSettings(&mDebug);
    	addSettings(&mPathTimeout);

        mNames = {
            {Lowercase, "autopath"},
            {LowercaseSpaced, "auto path"},
            {Normal, "AutoPath"},
            {NormalSpaced, "Auto Path"}
        };
    }

    static BlockSource* cachedSrc;
    static const float SQRT_2;
    static std::vector<Vec3i> sideAdj;
    static std::vector<Vec3i> directAdj;
    static std::vector<Vec3i> diagAdd;
    static float heuristicEstimation(const Vec3i& node, const Vec3i& target);
    static bool isCompletelyObstructed(const Vec3i& pos);
    static std::vector<std::pair<Vec3i, float>> getAirAdjacentNodes(const Vec3i& node, const Vec3i& start, Vec3i& goal);
    static std::vector<Vec3> findFlightPath(Vec3i start, Vec3i goal, BlockSource* src, float howClose, bool optimizePath, int64_t timeout, bool debugMsgs);
	static std::vector<glm::vec3> findFlightPathGlm(glm::vec3 start, glm::vec3 goal, BlockSource* src, float howClose, bool optimizePath, int64_t timeout, bool debugMsgs);
    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onRenderEvent(class RenderEvent& event);
};