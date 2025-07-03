#pragma once

enum RenderType
{
    Multi,
    OneCall // our new renderer (epic !)
};

enum IrisRenderFace // for mesh faces
{
    Left, Right
};

class RenderUtil3D
{
public:
    static inline Vector3<int> mCubeVertices[8] = { // cube vertices
        { 0, 0, 0 },
        { 0, 0, 1 },
        { 0, 1, 0 },
        { 0, 1, 1 },
        { 1, 0, 0 },
        { 1, 0, 1 },
        { 1, 1, 0 },
        { 1, 1, 1 }
    };

    static inline int mOneCallsRendered;
    static inline int mMultisRendered;

    static Vector2<float> WorldToScreen(const Vector3<float>& world) { // A world screen that returns the value
        if (ImGui::GetCurrentContext()) {
            Vector2<float> ret;
            InstanceManager::get<ClientInstance>()->WorldToScreen(world, ret, InstanceManager::get<ClientInstance>()->getFov(), FrameUtil::mTransform.mOrigin, FrameUtil::mTransform.mMatrix);
            return ret;
        }
    }
};