#include "WorldUtil.h"

static bool invalidChar(char c) {
	return !(c >= 0 && *reinterpret_cast<unsigned char*>(&c) < 128);
}

static std::string sanitize(const std::string& text) {
	std::string out;
	bool wasValid = true;
	for (char c : text) {
		bool isValid = !invalidChar(c);
		if (wasValid) {
			if (!isValid) {
				wasValid = false;
			}
			else {
				out += c;
			}
		}
		else {
			wasValid = isValid;
		}
	}
	return out;
}

Block* WorldUtil::getBlock(const BlockPos& blockPos) {
	return Game::clientInstance->getRegion()->getBlock(blockPos);
}

float WorldUtil::distanceToPoint(const Vec3<float>& pos, const Vec3<float>& point) {
	return pos.dist(point);
}

float WorldUtil::distanceToEntity(const Vec3<float>& pos, Actor* entity) {
	return distanceToPoint(pos, entity->getPos());
}

float WorldUtil::distanceToBlock(const Vec3<float>& pos, const BlockPos& blockPos) {
	return distanceToPoint(pos, blockPos.CastTo<float>().add(Vec3<float>(0.5f, 0.5f, 0.5f)));
}

float WorldUtil::getSeenPercent(const Vec3<float>& pos, const AABB& aabb) {
	return Game::clientInstance->getRegion()->getSeenPercent(pos, aabb);
}

float WorldUtil::getSeenPercent(const Vec3<float>& pos, Actor* actor) {
	return getSeenPercent(pos, actor->aabbShape->aabb);
}

std::string WorldUtil::getEntityNameTags(Actor* entity) {
	uint32_t entityId = entity->getActorTypeComponent()->id;

	switch (entityId) {
	case 319: {
		// Player
		return sanitize(*entity->getNameTag());
		break;
	}
	case 64: {
		// Item
		ItemStack* stack = (ItemStack*)((uintptr_t)entity + 0x448);

		if (!stack->isValid())
			return std::string("No item");

		std::stringstream textbuild;

		if (stack->mCount > 1)
			textbuild << std::to_string(stack->mCount) << "x ";

		bool wasSpace = true;
		std::string name = *(std::string*)(stack->item.get() + 0xD0);
		for (int i = 0; i < name.size(); i++) {
			if (wasSpace) {
				name[i] = toupper(name[i]);
				wasSpace = false;
			}

			if (name[i] == '_') {
				wasSpace = true;
				name[i] = ' ';
			}
		}

		textbuild << name;

		return textbuild.str();
		break;
	}
	}

	return "NULL";
}