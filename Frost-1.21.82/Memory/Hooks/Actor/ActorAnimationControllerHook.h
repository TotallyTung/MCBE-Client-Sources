#pragma once

void* onApplyToPose;

void ApplyToPoseDetour(ActorAnimationControllerPlayer* _this, RenderParams* renderParams, std::unordered_map<::SkeletalHierarchyIndex, std::vector<class BoneOrientation>>& destBoneOrientationsMap, float blendWeight, int mUnknown) {
	Memory::CallFunc<void, ActorAnimationControllerPlayer*, RenderParams*, std::unordered_map<::SkeletalHierarchyIndex, std::vector<class BoneOrientation>>&, float, int>( // CallFunc to call the original.
		onApplyToPose, _this, renderParams, destBoneOrientationsMap, blendWeight, mUnknown);

	InstanceManager::set<ActorAnimationControllerPlayer>(_this);

	auto mEntity = renderParams->mActor;

	if (InstanceManager::get<ClientInstance>() != nullptr && InstanceManager::isAllowedToUseKeys()) {
		Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
		if (!mPlayer)
			return;

		std::string mName = BoneOrientation::getName(destBoneOrientationsMap);
		BoneType mBoneType = BoneOrientation::getBoneType(destBoneOrientationsMap);
		ActorPartModel* mPart = BoneOrientation::getActorModel(destBoneOrientationsMap);

		if (mEntity == InstanceManager::get<ClientInstance>()->getLocalPlayer()) {
			ActorWalkAnimationComponent* mWalkAnimationComp = mEntity->getComponent<ActorWalkAnimationComponent>();

			mWalkAnimationComp->mWalkAnimSpeed = 1.5;
		}
	}
}

class ActorAnimationControllerHook : public FuncHook 
{
public:
	bool Initialize() override
	{
		Memory::HookFunction(SigManager::ActorAnimationControllerPlayer_applyToPose, (void*)&ApplyToPoseDetour, &onApplyToPose, "ActorAnimationControllerPlayer::applyToPose");

		return true;
	}
};