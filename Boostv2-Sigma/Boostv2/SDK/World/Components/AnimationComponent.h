#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>
#include <string>
#include <string_view>
#include <cstdint>
#include "../../Core/HashedString.h"

struct BoneOrientation {};
#pragma once

#include <array>

struct Quaternion {
    float x, y, z, w;
};

struct ModelPartLocator {
    float weight = 0.f;
    float scale = 1.f;

    std::array<double, 6> transformMatrixA{};
    std::array<double, 6> transformMatrixB{}; 

    Vec3<float> offset1 = { 0.f, 0.f, 0.f };
    Vec3<float> offset2 = { 0.f, 0.f, 0.f };
    Vec3<float> offset3 = { 0.f, 0.f, 0.f };

    bool active = true;

    std::array<float, 16> customMatrix{}; 

    ModelPartLocator() = default;
};
struct ActorAnimationPlayer {};
struct ActorAnimationControllerInfo {};
struct ActorAnimationControllerStatePlayer {};
struct RenderParams {};
struct AnimationComponentID {};
struct DataDrivenModel {};
struct CommonResourceDefinitionMap {};
enum class AnimationComponentGroupType {};

struct AnimationComponent {
    std::shared_ptr<ActorAnimationControllerStatePlayer> currentStatePlayer;
    std::vector<std::shared_ptr<ActorAnimationPlayer>> animationPlayers;
    std::vector<std::shared_ptr<ActorAnimationControllerInfo>> animationControllers;
    std::unordered_map<HashedString, ModelPartLocator> modelPartLocators;
    RenderParams renderParams;
    std::shared_ptr<ActorAnimationPlayer> playSingleAnimation;
    void* renderControllerCache = nullptr;
    std::weak_ptr<CommonResourceDefinitionMap> resourceMap;
    std::unordered_map<HashedString, HashedString> const* particleEffectMap = nullptr;
    std::function<void(ActorAnimationPlayer&)> initFunction;

    struct ChildAnimationComponentInfo {
        // Bu iç yapýnýn detaylarý bilinmediði için boþ býrakýyoruz
    };
    std::vector<ChildAnimationComponentInfo> childComponents;

    std::unordered_map<int, std::vector<BoneOrientation>> boneOrientations; // SkeletalHierarchyIndex yerine int

    bool initialized = false;
    AnimationComponentGroupType groupType;
    AnimationComponentID ownerUUID;
    int64_t lastUpdateFrame = 0;
    uint64_t lastReloadTimestamp = 0;
    std::weak_ptr<const DataDrivenModel> lastModel;
    bool applyAnimations = true;
    bool updateQueryableBones = false;
};
