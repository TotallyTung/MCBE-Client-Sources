#pragma once

struct FrameTransform {
    GLMatrix       mMatrix    {};
    Vector3<float> mOrigin    {};
    Vector3<float> mPlayerPos {};
    Vector2<float> mFov       {};
};

class FrameUtil {
public:
    static inline std::queue<FrameTransform> mFrameTransforms = {};
    static inline int mTransformDelay = 3;

    static inline FrameTransform mTransform;
};

class Frame {
public:
    static inline Vector2<float> mFov = { 0, 0 };
    static inline Vector2<float> mDisplaySize = { 0, 0 };
    static inline Vector3<float> mOrigin = { 0, 0, 0 };
};