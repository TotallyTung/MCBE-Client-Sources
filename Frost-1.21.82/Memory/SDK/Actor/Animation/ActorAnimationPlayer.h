#pragma once

class Matrix {};

enum class SkeletalHierarchyIndex : int32_t {
    Unknown = 0xff,
};

enum class ActorAnimationType : int32_t{
    Empty                    = 0xff,
    SkeletalAnimation        = 0x0,
    AnimationController      = 0x1,
    AnimationControllerState = 0x2,
};

enum class BoneType : int {
    Unknown  = -1,
    Head     = 0,
    LeftArm  = 1,
    RightArm = 2,
    LeftLeg  = 3,
    RightLeg = 4,
    Body     = 5
};

class ActorAnimationPlayer {
public:
	virtual ~ActorAnimationPlayer();
    virtual void applyToPose(class RenderParams& renderParams, std::unordered_map<::SkeletalHierarchyIndex, std::vector<class BoneOrientation>>& destBoneOrientationsMap, float blendWeight) = 0;
    virtual void resetAnimation() = 0;
    virtual void buildBoneToPartMapping(class AnimationComponent&);
    virtual void bindParticleEffects(std::unordered_map<class HashedString, class HashedString> const&);
    virtual void bindSoundEffects(std::unordered_map<class HashedString, std::string> const&);
    virtual bool hasAnimationFinished() const = 0;
    virtual std::shared_ptr<class ActorAnimationPlayer> findAnimation(class HashedString const& friendlyName) = 0;
    virtual ::ActorAnimationType getAnimationType() const = 0;
    virtual class HashedString const& getRawName() const = 0;
};

class ActorAnimationControllerPlayer : public ::ActorAnimationPlayer {};

class ActorAnimationControllerState {};

class ActorAnimationControllerStatePlayer : public ::ActorAnimationPlayer {};

class ActorAnimationGroup : public ::Bedrock::EnableNonOwnerReferences {
public:
    /*
    std::string const&      fileData;
    class Core::Path const& filenameWithExtension;
    class PackStats&        stats;
    class SemVersion const& engineVersion;
    ::CurrentCmdVersion     packCommandVersion;
    bool                    isPersonaPack;
    std::string const&      personaPieceId;
    */
};

class BoneOrientation {
public:
    bool            mUseMatrixFromOwningActor; // this+0x0
    int             mParentBoneIndex;          // this+0x4
    HashedString    mName;                     // this+0x10
    HashedString    mParentName;               // this+0x40
    Matrix          mLocalPreTransformMatrix;  // this+0x70
    Matrix          mMatrix;                   // this+0xB0
    float           mRotationRelativeMode;     // this+0xF0
    int             mTransform;                // this+0xF4
    char            padding[0x20];             // Padding to align with offset 0x118
    int             mDefaultTransform;         // this+0x118
    char            padding2[0x20];            // Padding to align with offset 0x13C
    Vector3<float>  mPivot;                    // this+0x13C
    bool            mOverrideStackMatrix;      // Offset: 0x148
public:
    static std::string getName(const std::unordered_map<SkeletalHierarchyIndex, std::vector<BoneOrientation>>& destBoneOrientationsMap) {
        const char* mName = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(&destBoneOrientationsMap) + 0x10);
        std::string boneName(mName);
        return boneName;
    }

    static Vector3<float> getPivot(const std::unordered_map<SkeletalHierarchyIndex, std::vector<BoneOrientation>>& destBoneOrientationsMap) {
        ActorPartModel* mPart = reinterpret_cast<ActorPartModel*>(reinterpret_cast<uintptr_t>(&destBoneOrientationsMap) + 0x78);
        return mPart->mPos;
    }

    static Vector3<float> getRotation(const std::unordered_map<SkeletalHierarchyIndex, std::vector<BoneOrientation>>& destBoneOrientationsMap) {
        ActorPartModel* mPart = reinterpret_cast<ActorPartModel*>(reinterpret_cast<uintptr_t>(&destBoneOrientationsMap) + 0x78);
        return mPart->mRot;
    }

    static Vector3<float> getSize(const std::unordered_map<SkeletalHierarchyIndex, std::vector<BoneOrientation>>& destBoneOrientationsMap) {
        ActorPartModel* mPart = reinterpret_cast<ActorPartModel*>(reinterpret_cast<uintptr_t>(&destBoneOrientationsMap) + 0x78);
        return mPart->mSize;
    }

    static ActorPartModel* getActorModel(const std::unordered_map<SkeletalHierarchyIndex, std::vector<BoneOrientation>>& destBoneOrientationsMap) {
        ActorPartModel* mPart = reinterpret_cast<ActorPartModel*>(reinterpret_cast<uintptr_t>(&destBoneOrientationsMap) + 0x78);
        return mPart;
    }

    static BoneType getBoneType(const std::unordered_map<SkeletalHierarchyIndex, std::vector<BoneOrientation>>& destBoneOrientationsMap) {
        const char* mName = reinterpret_cast<const char*>(reinterpret_cast<uintptr_t>(&destBoneOrientationsMap) + 0x10);
        std::string boneName(mName);

        std::transform(boneName.begin(), boneName.end(), boneName.begin(), ::tolower);

        static const std::unordered_map<std::string, BoneType> boneTypeMap = {
            { "head", BoneType::Head },
            { "leftarm", BoneType::LeftArm },
            { "rightarm", BoneType::RightArm },
            { "leftleg", BoneType::LeftLeg },
            { "rightleg", BoneType::RightLeg },
            { "body", BoneType::Body }
        };

        if (auto it = boneTypeMap.find(boneName); it != boneTypeMap.end()) {
            return it->second;
        }

        return BoneType::Unknown;
    }
};