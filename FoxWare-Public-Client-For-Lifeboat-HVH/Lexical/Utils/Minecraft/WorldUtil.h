#pragma once
#include "../../SDK/Game.h"

namespace WorldUtil {
	Block* getBlock(const BlockPos& blockPos);
	float distanceToPoint(const Vec3<float>& pos, const Vec3<float>& point);
	float distanceToEntity(const Vec3<float>& pos, Actor* entity);
	float distanceToBlock(const Vec3<float>& pos, const BlockPos& blockPos);
	float getSeenPercent(const Vec3<float>& pos, const AABB& aabb);
	float getSeenPercent(const Vec3<float>& pos, Actor* actor);
	std::string getEntityNameTags(Actor* entity);
}
