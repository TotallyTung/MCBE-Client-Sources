#pragma once

#include <string>
#include <math.h>
#include <complex>
#include <vector>
#include <imgui.h>
#include <ext/matrix_float4x4.hpp>

class BlockPos {
public:
    union {
        struct {
            int x, y, z;
        };
        int arr[3]{};
    };

    BlockPos(int x = 0, int y = 0, int z = 0) {
        this->x = x;
        this->y = y;
        this->z = z;
    }


    std::string ToString() {
        std::string rtn;
        rtn += std::to_string(x) + ",";
        rtn += std::to_string(y) + ",";
        rtn += std::to_string(z);
        return rtn;
    }

    // Operators

    bool operator == (BlockPos v) const { return v.x == x && v.y == y && v.z == z; }
    bool operator != (BlockPos v) const { return v.x != x || v.y != y || v.z != z; }

    BlockPos operator + (BlockPos v) const { return { x + v.x, y + v.y, z + v.z }; }
    BlockPos operator - (BlockPos v) const { return { x - v.x, y - v.y, z - v.z }; }

    BlockPos operator + (int v) const { return { x + v, y + v, z + v }; }
    BlockPos operator - (int v) const { return { x - v, y - v, z - v }; }

    BlockPos operator * (int v) const { return { x * v, y * v, z * v }; }
    BlockPos operator / (int v) const { return { x / v, y / v, z / v }; }

    BlockPos operator += (BlockPos v) const { return { x + v.x, y + v.y, z + v.z }; }
    BlockPos operator -= (BlockPos v) const { return { x - v.x, y - v.y, z - v.z }; }

    BlockPos operator += (int v) const { return { x + v, y + v, z + v }; }
    BlockPos operator -= (int v) const { return { x - v, y - v, z - v }; }

    BlockPos operator *= (int v) const { return { x * v, y * v, z * v }; }
    BlockPos operator /= (int v) const { return { x / v, y / v, z / v }; }

    BlockPos operator ++ () const { return { x + 1, y + 1, z + 1 }; }
    BlockPos operator -- () const { return { x - 1, y - 1, z - 1 }; }



    // Type Conversions

    //operator Vector3() const { return { (float)x, (float)y, (float)z }; }


    // Custom Functions

    float Distance(BlockPos v) const {
        float dX = x - v.x;
        float dY = y - v.y;
        float dZ = z - v.z;
        return std::sqrt(dX * dX + dY * dY + dZ * dZ);
    }

    float DistanceNoY(BlockPos v) const {
        float dX = x - v.x;
        float dZ = z - v.z;
        return std::sqrt(dX * dX + dZ * dZ);
    }
};

struct Vector2 {
    union {
        struct {
            float x, y;
        };
        float arr[2]{};
    };

    // Constructors
    Vector2() {
        x = 0;
        y = 0;
    }

    Vector2(int x) {
        x = 0;
        y = 0;
    }

    Vector2(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Vector2(int x, int y) {
        this->x = (float)x;
        this->y = (float)y;
    }

    Vector2(ImVec2 vec) {
        this->x = vec.x;
        this->y = vec.y;
    }

    // Operators

    bool operator == (Vector2 v) const { return v.x == x && v.y == y; }
    bool operator != (Vector2 v) const { return v.x != x || v.y != y; }

    Vector2 operator + (Vector2 v) const { return { x + v.x, y + v.y }; }
    Vector2 operator - (Vector2 v) const { return { x - v.x, y - v.y }; }

    Vector2 operator + (float v) const { return { x + v, y + v }; }
    Vector2 operator - (float v) const { return { x - v, y - v }; }

    Vector2 operator * (float v) const { return { x * v, y * v }; }
    Vector2 operator / (float v) const { return { x / v, y / v }; }

    Vector2 operator += (Vector2 v) const { return { x + v.x, y + v.y }; }
    Vector2 operator -= (Vector2 v) const { return { x - v.x, y - v.y }; }

    Vector2 operator += (float v) const { return { x + v, y + v }; }
    Vector2 operator -= (float v) const { return { x - v, y - v }; }

    Vector2 operator *= (float v) const { return { x * v, y * v }; }
    Vector2 operator /= (float v) const { return { x / v, y / v }; }

    Vector2 operator ++ () const { return { x + 1, y + 1 }; }
    Vector2 operator -- () const { return { x - 1, y - 1 }; }

    // float * Vector2 operat
    friend Vector2 operator*(float lhs, const Vector2& rhs) {
        return Vector2(lhs * rhs.x, lhs * rhs.y);
    }

    // Custom

    float Distance(Vector2 v) {
        return sqrtf(powf(v.x - x, 2) + powf(v.y - y, 2));
    }

    float Distance(float x, float y) {
        return sqrtf(powf(x - this->x, 2) + powf(y - this->y, 2));
    }

    float Distance(int x, int y) {
        return sqrtf(powf((float)x - this->x, 2) + powf((float)y - this->y, 2));
    }

    std::string ToString() {
        std::string rtn;
        rtn += std::to_string(x) + ",";
        rtn += std::to_string(y);
        return rtn;
    }

    // Type Conversions

    operator ImVec2() const { return ImVec2(x, y); }
    float Dot(Vector2 vector2) const { return x * vector2.x + y * vector2.y; }
    float Length() const { return sqrtf(x * x + y * y); }
    Vector2 Normalize() { return Vector2(x / Length(), y / Length()); }


    static Vector2 Lerp(Vector2 a, Vector2 b, float t) {
        return Vector2(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y));
    }
};


struct Vector3 {
    union {
        struct {
            float x, y, z;
        };
        float arr[3]{};
    };

    // Constructors
    Vector3() {
        x = 0;
        y = 0;
        z = 0;
    }

    Vector3(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector3(int x, int y, int z) {
        this->x = (float)x;
        this->y = (float)y;
        this->z = (float)z;
    }

    Vector3(BlockPos pos) {
        this->x = (float)pos.x;
        this->y = (float)pos.y;
        this->z = (float)pos.z;
    }

    Vector3(glm::vec3 vec) {
        this->x = vec.x;
        this->y = vec.y;
        this->z = vec.z;
    }

    // Operators

    bool operator == (Vector3 v) const { return v.x == x && v.y == y && v.z == z; }
    bool operator != (Vector3 v) const { return v.x != x || v.y != y || v.z != z; }

    Vector3 operator + (Vector3 v) const { return { x + v.x, y + v.y, z + v.z }; }
    Vector3 operator - (Vector3 v) const { return { x - v.x, y - v.y, z - v.z }; }

    Vector3 operator + (float v) const { return { x + v, y + v, z + v }; }
    Vector3 operator - (float v) const { return { x - v, y - v, z - v }; }

    Vector3 operator * (float v) const { return { x * v, y * v, z * v }; }
    Vector3 operator / (float v) const { return { x / v, y / v, z / v }; }

    Vector3 operator += (Vector3 v) const { return { x + v.x, y + v.y, z + v.z }; }
    Vector3 operator -= (Vector3 v) const { return { x - v.x, y - v.y, z - v.z }; }

    Vector3 operator += (float v) const { return { x + v, y + v, z + v }; }
    Vector3 operator -= (float v) const { return { x - v, y - v, z - v }; }

    Vector3 operator *= (float v) const { return { x * v, y * v, z * v }; }
    Vector3 operator /= (float v) const { return { x / v, y / v, z / v }; }

    Vector3 operator ++ () const { return { x + 1, y + 1, z + 1 }; }
    Vector3 operator -- () const { return { x - 1, y - 1, z - 1 }; }




    Vector3 Lerp(const Vector3& pos, float x1) {
        return Vector3(x + (pos.x - x) * x1, y + (pos.y - y) * x1, z + (pos.z - z) * x1);
    }

    float Distance(Vector3 v) {
        float dX = x - v.x;
        float dY = y - v.y;
        float dZ = z - v.z;
        return std::sqrt(dX * dX + dY * dY + dZ * dZ);
    }

    float DistanceNoY(Vector3 v) {
        float dX = x - v.x;
        float dZ = z - v.z;
        return std::sqrt(dX * dX + dZ * dZ);
    }

    std::string ToString(bool detailed = false) {

        std::string rtn;
        if (detailed){
            rtn += "X: " + std::to_string(x) + ",";
            rtn += "Y: " + std::to_string(y) + ",";
            rtn += "Z: " + std::to_string(z);
        }
        else {
            rtn += std::to_string(x) + ",";
            rtn += std::to_string(y) + ",";
            rtn += std::to_string(z);
        }
        return rtn;
    }


    [[nodiscard]] Vector3 sub(const Vector3& o) const {
        return Vector3(x - o.x, y - o.y, z - o.z);
    }

    // Type Conversions

    operator BlockPos() const {
        return BlockPos((int)x, (int)y, (int)z);
    }

    operator BlockPos*() const {
        return new BlockPos((int)x, (int)y, (int)z);
    }
};

struct Rect {
    union
    {
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
        float array[4]{};
    };

    Rect()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
        this->w = 0;
    }

    Rect(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    // Type Conversions

    ImVec4 ToImVec4() const {
        return ImVec4(x, y, z, w);
    }

};

struct AABB {
    union {
        struct { Vector3 lower, upper; };
        Vector3 arr[2]{};
    };

    AABB(const Vector3& position, const Vector3& size) {
        this->upper.x = position.x;
        this->lower.x = position.x + size.x;
        this->upper.y = position.y;
        this->lower.y = position.y + size.y;
        this->upper.z = position.z;
        this->lower.z = position.z + size.z;
    }

    AABB(Vector3 min, Vector3 max, bool)
    {
        this->upper = max;
        this->lower = min;
    }

    AABB() {
        this->upper = Vector3();
        this->lower = Vector3();
    }

    // Operators

    bool operator == (AABB v) const { return v.lower == lower && v.upper == upper; }
    bool operator != (AABB v) const { return v.lower != lower || v.upper != upper; }

    // math
    AABB operator + (Vector3 v) const { return { lower + v, upper + v, true }; }
    AABB operator - (Vector3 v) const { return { lower - v, upper - v, true }; }



    Vector3 GetClosestPoint(Vector3 point) {
        Vector3 result = point;
        if (point.x < lower.x) result.x = lower.x;
        if (point.y < lower.y) result.y = lower.y;
        if (point.z < lower.z) result.z = lower.z;
        if (point.x > upper.x) result.x = upper.x;
        if (point.y > upper.y) result.y = upper.y;
        if (point.z > upper.z) result.z = upper.z;
        return result;
    }

    // Custom Functions
private:
    double Abs(double a)
    {
        return std::fabs(a);
    }
public:



    Vector3 GetCenterPos() {
        return Vector3((upper.x + lower.x) / 2, (upper.y + lower.y) / 2, (upper.z + lower.z) / 2);
    }

    bool Intersects(AABB other)
    {
        bool x = Abs(lower.x + upper.x - other.lower.x - other.upper.x) <= Abs(upper.x - lower.x + other.upper.x - other.lower.x);
        bool y = Abs(lower.y + upper.y - other.lower.y - other.upper.y) <= Abs(upper.y - lower.y + other.upper.y - other.lower.y);
        bool z = Abs(lower.z + upper.z - other.lower.z - other.upper.z) <= Abs(upper.z - lower.z + other.upper.z - other.lower.z);

        // Now, lets check each corner of this aabb and check of the other aabb contains it.
        Vector3 corners[8] = {
            Vector3(lower.x, lower.y, lower.z),
            Vector3(lower.x, lower.y, upper.z),
            Vector3(lower.x, upper.y, lower.z),
            Vector3(lower.x, upper.y, upper.z),
            Vector3(upper.x, lower.y, lower.z),
            Vector3(upper.x, lower.y, upper.z),
            Vector3(upper.x, upper.y, lower.z),
            Vector3(upper.x, upper.y, upper.z)
        };

        bool contains = x && y && z;

        for (Vector3 corner : corners) {
            if (other.Contains(corner)) {
                contains = true;
                break;
            }
        }

        return contains;
    }

    // Checks if the provided point is inside the AABB.
    bool Contains(Vector3 point) {
        bool contains = point.x >= lower.x && point.x <= upper.x &&
            point.y >= lower.y && point.y <= upper.y &&
            point.z >= lower.z && point.z <= upper.z;

        // If the point is inside the AABB, return true.
        if (contains) return true;

        // Clamp the point to the AABB and check if it is equal to the original point.

        float newX = std::clamp(point.x, lower.x, upper.x);
        float newY = std::clamp(point.y, lower.y, upper.y);
        float newZ = std::clamp(point.z, lower.z, upper.z);

        return newX == point.x && newY == point.y && newZ == point.z;

    }

    bool IntersectsAny(std::vector<AABB> others) {
        for (auto& other : others) {
            if (Intersects(other)) return true;
        }
        return false;
    }

    std::string ToString() {
        return lower.ToString() + ", " + upper.ToString();
    }
};

struct rgb {
    union {
        struct { float r, g, b, a; };
        float arr[4]{};
    };
    bool shouldDelete = true;

    rgb() {
        this->r = 1;
        this->g = 1;
        this->b = 1;
        this->a = 1;
    }

    rgb(float x) {
        this->r = x;
        this->g = x;
        this->b = x;
        this->a = 1;
    }

    rgb(const rgb& other) {
        this->r = other.r;
        this->g = other.g;
        this->b = other.b;
        this->a = other.a;
        this->shouldDelete = other.shouldDelete;
    }

    rgb(const float r, const float g, const float b, const float a = 255) {
        this->r = r / 255.0f;
        this->g = g / 255.0f;
        this->b = b / 255.0f;
        this->a = a / 255.0f;
    }

    rgb(const int r, const int g, const int b, const int a = 255) {
        this->r = r / 255.0f;
        this->g = g / 255.0f;
        this->b = b / 255.0f;
        this->a = a / 255.0f;
    }

    rgb(const float r, const float g, const float b, const float a, const bool shouldDelete) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
        this->shouldDelete = shouldDelete;
    }

    // Type Conversions

    operator ImVec4() const {
        return ImVec4(r, g, b, a);
    }

    operator ImColor() const {
        return ImColor(r, g, b, a);
    }

    operator ImU32() const {
        return ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a));
    }

    // Custom Functions

    // ToHSV

    Vector3 GetHSV() {
        float h, s, v;
        ImGui::ColorConvertHSVtoRGB(r, g, b,  h, s, v);
        return Vector3(h, s, v);
    }

    std::string ToString() {
        return std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a);
    }
};

struct CaretMeasureData {
    int a{};
    bool b{};

    CaretMeasureData() {
        CaretMeasureData(0xFFFFFFFF, true);  // NOLINT(bugprone-undelegated-constructor)
    }
    CaretMeasureData(int param_1, bool param_2) {
        this->a = param_1;
        this->b = param_2;
    }
};

struct glmatrixf : public glm::mat4
{
    glmatrixf() = default;

    glmatrixf(glm::mat4 mat) : glm::mat4(mat) {}


    bool OWorldToScreen(Vector3 origin, Vector3 pos, Vector2& screen, Vector2 fov, Vector2 displaySize) const
    {
        pos = pos - origin;

        float x = glm::dot((*this)[0], glm::vec4(pos.x, pos.y, pos.z, 1.0f));
        float y = glm::dot((*this)[1], glm::vec4(pos.x, pos.y, pos.z, 1.0f));
        float z = glm::dot((*this)[2], glm::vec4(pos.x, pos.y, pos.z, 1.0f));

        if (z > 0)
            return false;

        float mX = (float)displaySize.x / 2.0F;
        float mY = (float)displaySize.y / 2.0F;

        screen.x = mX + (mX * x / -z * fov.x);
        screen.y = mY - (mY * y / -z * fov.y);

        return true;
    }

    inline void mul(const glmatrixf& x, const glmatrixf& y) {
        mul(x, y);
    }
    ///pos should be the exact center of the enemy model for scaling to work properly
    Vector2 WorldToScreen(Vector3 pos, int width, int height) {
        //Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
        Rect clipCoords;
        clipCoords.x = glm::dot((*this)[0], glm::vec4(pos.x, pos.y, pos.z, 1.0f));
        clipCoords.y = glm::dot((*this)[1], glm::vec4(pos.x, pos.y, pos.z, 1.0f));
        clipCoords.z = glm::dot((*this)[2], glm::vec4(pos.x, pos.y, pos.z, 1.0f));
        clipCoords.w = glm::dot((*this)[3], glm::vec4(pos.x, pos.y, pos.z, 1.0f));

        //perspective division, dividing by clip.W = Normalized Device Coordinates
        Vector3 NDC;
        NDC.x = clipCoords.x / clipCoords.w;
        NDC.y = clipCoords.y / clipCoords.w;
        NDC.z = clipCoords.z / clipCoords.w;

        //viewport tranform to screenCooords

        Vector2 playerscreen;
        playerscreen.x = ((float)width / 2 * NDC.x) + (NDC.x + (float)width / 2);  // NOLINT(bugprone-integer-division)
        playerscreen.y = -((float)height / 2 * NDC.y) + (NDC.y + (float)height / 2);  // NOLINT(bugprone-integer-division)

        return playerscreen;
    }

    ImVec4 getRectForAABB(const AABB& aabb, Vector3 origin, Vector2 fov, Vector2 displaySize) {
        Vector3 worldPoints[8];
        worldPoints[0] = Vector3(aabb.lower.x, aabb.lower.y, aabb.lower.z);
        worldPoints[1] = Vector3(aabb.lower.x, aabb.lower.y, aabb.upper.z);
        worldPoints[2] = Vector3(aabb.upper.x, aabb.lower.y, aabb.lower.z);
        worldPoints[3] = Vector3(aabb.upper.x, aabb.lower.y, aabb.upper.z);
        worldPoints[4] = Vector3(aabb.lower.x, aabb.upper.y, aabb.lower.z);
        worldPoints[5] = Vector3(aabb.lower.x, aabb.upper.y, aabb.upper.z);
        worldPoints[6] = Vector3(aabb.upper.x, aabb.upper.y, aabb.lower.z);
        worldPoints[7] = Vector3(aabb.upper.x, aabb.upper.y, aabb.upper.z);

        std::vector<Vector2> points;
        for (int i = 0; i < 8; i++) {
            Vector2 result;
            if (OWorldToScreen(origin, worldPoints[i], result, fov, displaySize))
                points.emplace_back(result);
        }
        if (points.size() < 2) return { 0.0f, 0.0f, 0.0f, 0.0f };

        ImVec4 resultRect = { points[0].x, points[0].y, points[0].x, points[0].y };
        for (const auto& point : points) {
            if (point.x < resultRect.x) resultRect.x = point.x;
            if (point.y < resultRect.y) resultRect.y = point.y;
            if (point.x > resultRect.z) resultRect.z = point.x;
            if (point.y > resultRect.w) resultRect.w = point.y;
        }
        return resultRect;
    }
};

struct FrameTransform
{
    glmatrixf modelView {};
    Vector3 origin {};
    Vector3 playerPos {};
};
