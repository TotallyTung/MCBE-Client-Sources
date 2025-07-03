#pragma once

struct AABB{
    union {
        struct {
            Vector3<float> mMin;
            Vector3<float> mMax;
        };
        Vector3<float> mBounds[2];
    };

    AABB(const Vector3<float>& position, const Vector3<float>& size) {
        mMin.x = position.x + size.x;
        mMin.y = position.y + size.y;
        mMin.z = position.z + size.z;
        mMax.x = position.x;
        mMax.y = position.y;
        mMax.z = position.z;
    }

    AABB(const Vector3<float>& min, const Vector3<float>& max, bool) : mMin(min), mMax(max) {}
    AABB() : mMin(0), mMax(0) {}

    Vector3<float> getClosestPoint(const Vector3<float>& point) const {
        Vector3<float> result;
        result.x = point.x < mMin.x ? mMin.x : (point.x > mMax.x ? mMax.x : point.x);
        result.y = point.y < mMin.y ? mMin.y : (point.y > mMax.y ? mMax.y : point.y);
        result.z = point.z < mMin.z ? mMin.z : (point.z > mMax.z ? mMax.z : point.z);
        return result;
    }
};