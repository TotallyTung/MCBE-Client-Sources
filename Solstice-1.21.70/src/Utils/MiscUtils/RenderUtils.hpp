#pragma once
#include <Utils/Structs.hpp>
//
// Created by vastrakai on 7/7/2024.
//



struct ImTexture {
    ID3D11ShaderResourceView* texture;
    int width;
    int height;
};

class ImTextureCache
{
public:
    // path -> texture
    static inline std::unordered_map<std::string, ImTexture> textures;
    static inline std::mutex textureMutex;

    static ImTexture* tryGet(const std::string& path);
    static ImTexture* cacheOrGet(const std::string& path);
    static void freeCachedTextures();
};

class RenderUtils {
public:
    static inline FrameTransform transform;

    static void drawOutlinedAABB(const AABB& aabb, bool filled, const ImColor& color = ImColor(255, 255, 255, 255));

    static bool worldToScreen(glm::vec3 pos, ImVec2& screenPos);
};