#pragma once

class ActorUtils {
public:
    static inline std::vector<Actor*> getActorList(float mRange = 50, bool mIncludePlayer = true, bool mSort = true) {
        Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();
        auto mList = mPlayer->getLevel()->getRuntimeActorList();

        std::vector<Actor*> mTargetList;

        mTargetList.clear();
        mTargetList.reserve(mList.size());

        std::for_each(mList.begin(), mList.end(), [&](Actor* actor) {
            if (!mIncludePlayer) {
                if (isValidTarget(actor) && actor != mPlayer) {
                    float mDistance = mPlayer->getPosition().distance(actor->getPosition());
                    if (mDistance <= mRange) {
                        mTargetList.push_back(actor);
                    }
                }
            }
            else {
                if (isValidTarget(actor)) {
                    float mDistance = mPlayer->getPosition().distance(actor->getPosition());
                    if (mDistance <= mRange) {
                        mTargetList.push_back(actor);
                    }
                }
            }
            });

        if (mSort) {
            std::sort(mTargetList.begin(), mTargetList.end(), [&](Actor* a, Actor* b) {
                return mPlayer->getPosition().distance(a->getPosition()) <
                    mPlayer->getPosition().distance(b->getPosition());
                });
        }

        return mTargetList;
    }

    static inline Actor* getObstructingActor(Actor* mPlayer, Actor* mTarget, float mRange, int mBypassMode = 0) {
        if (mBypassMode == 0) // None
            return mTarget;

        auto mActors = mPlayer->getLevel()->getRuntimeActorList();

        std::ranges::sort(mActors, [&](Actor* a, Actor* b) -> bool
            {
                return a->getPosition().distance(mPlayer->getPosition()) < b->getPosition().distance(mPlayer->getPosition());
            });

        if (mBypassMode == 1) // Raycast
        {
            std::map<Actor*, AABBShapeComponent> mActorHitboxes;
            Vector3<float> mRayStart = mPlayer->getPosition();
            Vector3<float> mRayEnd = mTarget->getPosition();
            Vector3<float> mCurrentRayPos = mRayStart;

            for (auto actor : mActors)
            {
                if (actor == mPlayer || actor == mTarget) continue;
                if (actor->getPosition().distance(mPlayer->getPosition()) > mRange) continue;

                auto mHitbox = *actor->getAABBShapeComponent();
                mActorHitboxes[actor] = mHitbox;

                if (MathUtil::rayIntersectsAABB(mCurrentRayPos, mRayEnd, mHitbox.mPosLower, mHitbox.mPosUpper))
                {
                    /*if (mDebug.mValue)
                    {
                        spdlog::info("Found obstructing actor: {}", actor->mEntityIdentifier);
                        ChatUtils::displayClientMessage("Attacking obstructing actor: " + actor->mEntityIdentifier);
                    }*/
                    mTarget = actor;
                    break;
                }
            }

            return mTarget;
        }

        return mTarget;
    }

    static inline std::vector<Actor*> getHiveBot() {
        Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();
        auto mList = mPlayer->getLevel()->getRuntimeActorList();

        std::vector<Actor*> mTargetList;

        mTargetList.clear();
        mTargetList.reserve(mList.size());

        std::for_each(mList.begin(), mList.end(), [&](Actor* actor) {
            Vector2<float> mHitbox = actor->getAABBShapeComponent()->mHitbox;
            bool mHitboxIsShadow = mHitbox.x == 0.86f && mHitbox.y == 2.32;
            bool mIsHiveEntity = actor->getEntityTypeId() == ActorType::HiveEntity;

            if (mIsHiveEntity && mHitboxIsShadow && actor != mPlayer) {
                float mDistance = mPlayer->getPosition().distance(actor->getPosition());
                if (mDistance <= 3) {
                    mTargetList.push_back(actor);
                }
            }
            });

        // Optionally sort by distance (or another criterion)
        std::sort(mTargetList.begin(), mTargetList.end(), [&](Actor* a, Actor* b) {
            return mPlayer->getPosition().distance(a->getPosition()) <
                mPlayer->getPosition().distance(b->getPosition());
            });

        return mTargetList;
    }

    static bool isValidTarget(Actor* actor) {
        Player* player = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();

        if (!player || !actor || actor->getEntityTypeId() != ActorType::Player || !actor->hasComponent<PlayerComponent>())
            return false; // Skip if actor is null

        return true;
    }
};