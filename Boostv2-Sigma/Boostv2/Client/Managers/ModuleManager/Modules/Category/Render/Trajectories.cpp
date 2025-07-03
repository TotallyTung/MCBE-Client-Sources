#include "Trajectories.h"

Trajectories::Trajectories() : Module("Trajectories", "Draws line for useable items", Category::RENDER) {
}

std::unordered_map<Actor*, std::vector<Vec3<float>>> trajectoryMap;

void Trajectories::onLevelRender() {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (!localPlayer) return;

	for (auto& entity : localPlayer->level->getRuntimeActorList()) {
		if (!entity || !entity->getActorTypeComponent()) continue;

		uint32_t entityId = entity->getActorTypeComponent()->id;

		if (entityId == 4194391 || entityId == 4194372 || entityId == 4194385) {
			Vec3<float> currentPos = entity->getPos();

			auto it = trajectoryMap.find(entity);
			if (it == trajectoryMap.end()) {
				trajectoryMap[entity] = std::vector<Vec3<float>>{ currentPos };
				continue;
			}

			auto& path = it->second;
			if (path.empty() || path.back().dist(currentPos) > 0.05f)
				path.push_back(currentPos);

			if (path.size() > 50)
				path.erase(path.begin());

			float t = std::min(1.f, (float)path.size() / 50.f);
			UIColor mainColor = ColorUtil::lerp(UIColor(0, 200, 255), UIColor(255, 0, 255), t);
			mainColor.a = 255;

			for (size_t i = 1; i < path.size(); i++) {
				Vec3<float> start = path[i - 1].sub(MCR::origin);
				Vec3<float> end = path[i].sub(MCR::origin);
				MCR::line3dRenderList.push_back({ start, mainColor });
				MCR::line3dRenderList.push_back({ end, mainColor });
			}

			Vec3<float> playerPos = localPlayer->getEyePos().sub(MCR::origin);
			Vec3<float> entityPos = currentPos.add(Vec3<float>(0.f, 0.25f, 0.f)).sub(MCR::origin);
			MCR::line3dRenderList.push_back({ playerPos, mainColor });
			MCR::line3dRenderList.push_back({ entityPos, mainColor });

			Vec3<float> pos = currentPos;
			Vec3<float> velocity = entity->stateVector->velocity;

			const float gravity = -0.03f;
			const float drag = 0.99f;
			const int maxSteps = 200;
			const float stepSize = 1.0f;

			Vec3<float> hitPos = pos;
			Vec3<float> direction = velocity;

			for (int i = 0; i < maxSteps; ++i) {
				Vec3<float> next = pos.add(velocity.mul(stepSize));
				Block* block = Game.getClientInstance()->getRegion()->getBlock(next.toInt());
				if (block && block->getblockLegacy()->isSolid()) {
					hitPos = next;
					direction = velocity;
					break;
				}
				pos = next;
				velocity = velocity.mul(drag);
				velocity.y += gravity * stepSize;
			}

			Vec3<float> center = hitPos.sub(MCR::origin);
			Vec3<float> p1, p2, p3, p4;

			if (std::abs(direction.y) > std::abs(direction.x) && std::abs(direction.y) > std::abs(direction.z)) {
				p1 = Vec3<float>(-0.5f, 0.f, -0.5f);
				p2 = Vec3<float>(0.5f, 0.f, -0.5f);
				p3 = Vec3<float>(0.5f, 0.f, 0.5f);
				p4 = Vec3<float>(-0.5f, 0.f, 0.5f);
			}
			else if (std::abs(direction.x) > std::abs(direction.z)) {
				p1 = Vec3<float>(0.f, -0.5f, -0.5f);
				p2 = Vec3<float>(0.f, 0.5f, -0.5f);
				p3 = Vec3<float>(0.f, 0.5f, 0.5f);
				p4 = Vec3<float>(0.f, -0.5f, 0.5f);
			}
			else {
				p1 = Vec3<float>(-0.5f, -0.5f, 0.f);
				p2 = Vec3<float>(0.5f, -0.5f, 0.f);
				p3 = Vec3<float>(0.5f, 0.5f, 0.f);
				p4 = Vec3<float>(-0.5f, 0.5f, 0.f);
			}

			p1 = p1.add(center);
			p2 = p2.add(center);
			p3 = p3.add(center);
			p4 = p4.add(center);

			for (float offset = -0.02f; offset <= 0.02f; offset += 0.02f) {
				Vec3<float> off = Vec3<float>(offset, offset, offset);
				MCR::line3dRenderList.push_back({ p1.add(off), mainColor });
				MCR::line3dRenderList.push_back({ p2.add(off), mainColor });
				MCR::line3dRenderList.push_back({ p2.add(off), mainColor });
				MCR::line3dRenderList.push_back({ p3.add(off), mainColor });
				MCR::line3dRenderList.push_back({ p3.add(off), mainColor });
				MCR::line3dRenderList.push_back({ p4.add(off), mainColor });
				MCR::line3dRenderList.push_back({ p4.add(off), mainColor });
				MCR::line3dRenderList.push_back({ p1.add(off), mainColor });
			}
		}
	}
}
