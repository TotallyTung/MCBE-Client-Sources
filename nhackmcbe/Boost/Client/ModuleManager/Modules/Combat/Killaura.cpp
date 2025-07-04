#include "Killaura.h"
#include <cmath> // for std::atan2()
bool packetattack = false;
bool antitotem = false; //not done
Killaura::Killaura() : Module("Killaura", "Auto attack players / mobs arround u.", Category::COMBAT) {
	addSlider<float>("TargetR", "Players/Mobs have range lower than this will be targeted", ValueType::FLOAT_T, &targetRange, 3.f, 100.f);
	addSlider<float>("AttackR", "NULL", ValueType::FLOAT_T, &ARange, 0.f, 100.f);
	addSlider<float>("Max Strafe", "NULL", ValueType::FLOAT_T, &MaxSrange, 0.f, 100.f);
	addSlider<float>("Min Strafe", "NULL", ValueType::FLOAT_T, &MinSrange, 0.f, 100.f);
	addEnumSetting("Mode", "NULL", { "Single", "Multi" }, &Mode);
	addEnumSetting("Rotation", "NULL", { "None", "Silent" ,"Strafe" }, &rotMode);
	addEnumSetting("Switch", "NULL", { "None", "Full", "Silent" }, &switchMode);
	addBoolCheck("Attack Mob", "If u want attack mob or not", &attackMob);
	addBoolCheck("Hurttime check", "NULL", &hurttime);
	addSlider<int>("Attack delay", "NULL", ValueType::INT_T, &attackDelay, 0, 20);
	addSlider<int>("Multiplier", "NULL", ValueType::INT_T, &mult, 0, 100);
	addSlider<int>("CPS", "NULL", ValueType::INT_T, &APS, 0, 100);
	//addBoolCheck("Visual AttackRange", "NULL", &visualRange);
	addBoolCheck("PacketAttack", "Breaks armor faster but is weaker", &packetattack);
	addBoolCheck("NoSwing", "NULL", &noSwing);
	addBoolCheck("AntiTotem", "NULL", &antitotem); //using surround in the air with killaura makes you take less damage for whatever reason. we can just remove surround rendering and placements and add it under on tick. not done yet.
	addSlider<int>("TPS", "TimerSpeed", ValueType::INT_T, &timer, 0, 100);
}
std::string Killaura::getModName() {
	static char textStr[15];
	if (Mode == 0) {
		sprintf_s(textStr, 15, "Single");
	}
	else if (Mode == 1) {
		sprintf_s(textStr, 15, "Multi");
	}
	return std::string(textStr);
}
int Killaura::getBestWeaponSlot() {
	auto localPlayer = mc.getLocalPlayer();
	if (!localPlayer)
		return -1;

	auto plrInv = localPlayer->getPlayerInventory();
	if (!plrInv)
		return -1;

	auto inv = plrInv->inventory;
	if (!inv)
		return -1;

	float damage = 0.f;
	int slot = plrInv->selectedSlot;

	for (int i = 0; i < 9; i++) {
		auto itemStack = inv->getItemStack(i);
		if (itemStack && itemStack->isValid()) { // or you can do itemStack->getItemPtr()->hasRecipeTag("minecraft:is_sword") for sword find only 
			/*
			* here some step that you can get pick and things
			* item->getItemPtr()->hasRecipeTag("minecraft:digger") <- this is how you get like a things that can digs like shovel and picks and axe maybe
			* if (item->hasRecipeTag("minecraft:is_sword"))
			* item->hasRecipeTag("minecraft:is_pickaxe") <- get pickaxe
			* item->hasRecipeTag("minecraft:is_axe") <- get axe
			* item->hasRecipeTag("minecraft:is_shovel") <- get some shovel
			* item->hasRecipeTag("minecraft:is_hoe")<- get hoe like a riel bitch
			* item->hasRecipeTag("minecraft:is_food") <- get food
			*/
			float currentDamage = itemStack->getItemPtr()->getAttackDamage() + (1.25f * itemStack->getEnchantLevel(EnchantID::sharpness));
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = i;
			}
		}
	}
	return slot;
}
# define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811174502841027019385211055596446229489549303819644288109756659334461284756482337867831652712019091456485669234603486104543266482133936072602491412737245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094330572703657595919530921861173819326117931051185480744623799627495673518857527248912279381830119491298336733624406566430860213949463952247371907021798609437027705392171762931767523846748184676694051320005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235420199561121290219608640344181598136297747713099605187072113499999983729780499510597317328160963185950244594553469083026425223082533446850352619311881710100031378387528865875332083814206171776691473035982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989 /* 1000 digits of pi, perfect strafe */

Vec2<float> GetRotations(Vec3<float> playerEyePos, Vec3<float> targetPos) {
	Vec3<float> delta = targetPos.sub(playerEyePos);
	float yaw = atan2(delta.z, delta.x) * 180.f / M_PI;
	float pitch = atan2(delta.y, std::sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
	return { -pitch, yaw - 90 };
}
decltype(GetTickCount64()) startx;  //????
bool Counterx(double a1) {
	if ((GetTickCount64() - startx) >= a1) {
		startx = GetTickCount64();
		return true;
	}
	return false;
}

void Killaura::onNormalTick(Actor* actor) {
	auto localPlayer = mc.getLocalPlayer();
	auto localpos = mc.getLocalPlayer()->getEyePos();
	auto plrInv = localPlayer->getPlayerInventory();
	auto inv = plrInv->inventory;
	auto gm = mc.getGameMode();
	auto region = localPlayer->dimension->blockSource;
	auto level = localPlayer->getLevel();

	targetList.clear();
	if (!level) return;
	for (auto& target : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(target, attackMob)) {
			float dist = target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
			float rangeCheck = targetRange;
			if (dist < rangeCheck)
				targetList.push_back(target);
		}
	}
	mc.getClientInstance()->minecraft->mctimer->tps = (float)timer;

	if (!targetList.empty()) {
		std::sort(targetList.begin(), targetList.end(), TargetUtils::sortByDist);
		TargetDis = targetList[0]->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos);
		isInRange = true;
		if (TargetDis < MaxSrange) {

			Vec2  rotAngle = GetRotations(*mc.getLocalPlayer()->getPosition(), *targetList[0]->getPosition());
			//mc.DisplayClientMessage("KA = %f", rotAngle.y);
			if (rotMode == 2){
				localPlayer->rotationComponent->rotation = rotAngle;
			}
		}

		if (attackDelayTick >= attackDelay) {
			int bestSlot = getBestWeaponSlot();
			int oldSlot = plrInv->selectedSlot;
			bool shouldSwitch = (bestSlot != plrInv->selectedSlot);

			if (shouldSwitch && (switchMode == 1 || switchMode == 2)) {
				plrInv->selectedSlot = bestSlot;
				if (switchMode == 2) {
					/*auto packet = MinecraftPackets::createPacket(PacketID::MobEquipmentPacket);
					auto packetcasted = std::reinterpret_pointer_cast<MobEquipmentPacket>(packet);
					packetcasted->ActorRuntimeID = localPlayer->getRuntimeID();
					packetcasted->ContainerID = 0;
					packetcasted->ContainerID2 = 0;
					packetcasted->InventorySlot = bestSlot;
					packetcasted->InventorySlot2 = bestSlot;
					packetcasted->HotbarSlot = bestSlot;
					packetcasted->HotbarSlot2 = bestSlot;
					NetworkItemStackDescriptor itemstackdescriptor(*inv->getItemStack(bestSlot));
					packetcasted->ItemStackDescriptor = itemstackdescriptor;
					mc.getClientInstance()->loopbackPacketSender->send(packetcasted.get());*/ // <- this hehh tao lay tu nuvola tai vi no ngon hon
					MobEquipmentPacket pk(localPlayer->getRuntimeID(), inv->getItemStack(bestSlot), bestSlot, bestSlot);
					mc.getLocalPlayer()->sendNetworkPacket(pk);
					mc.getClientInstance()->loopbackPacketSender->send(&pk);
				}
			}
			if (Mode == 1 && packetattack)
			{
				for (auto& target : targetList) {
					if (target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos) < ARange) {
						if (!(hurttime && target->hurtTime > 0)) {
							InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), target->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));

							mc.getClientInstance()->loopbackPacketSender->send(&inter);
							mc.getClientInstance()->loopbackPacketSender->send(&inter);
							gm->attack(target);
							gm->attack(target);
							localPlayer->swing();
						}
					}
				}
			}
			if (Mode == 0 && packetattack)
			{
				if (TargetDis < ARange) {
					if (!(hurttime && targetList[0]->hurtTime > 0)) {
						InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), targetList[0]->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));

						mc.getClientInstance()->loopbackPacketSender->send(&inter);
						mc.getClientInstance()->loopbackPacketSender->send(&inter);
						gm->attack(targetList[0]);
						gm->attack(targetList[0]);
						localPlayer->swing();
					}
				}
			}
			for (int target = 0; target < mult; target++) {
				if (Mode == 1 && !packetattack)
				{
					if (Counterx((double)1000.f / (float)APS)) {
						for (auto& target : targetList) {
							if (target->stateVectorComponent->pos.dist(localPlayer->stateVectorComponent->pos) < ARange) {
								if (!(hurttime && target->hurtTime > 0)) {
									//InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), target->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));

									//mc.getClientInstance()->loopbackPacketSender->send(&inter);
									gm->attack(target); //we use three to do more damage
									if (noSwing)
										return;
									if (!targetList.empty() && mc.getLocalPlayer() != nullptr && !noSwing)
										localPlayer->swing();
								}
							}
						}
					}
				}
				if (Mode == 0 && !packetattack)
				{
					if (Counterx((double)1000.f / (float)APS)) {
						if (TargetDis < ARange) {
							if (!(hurttime && targetList[0]->hurtTime > 0)) {
								//InteractPacket inter(InteractAction::LEFT_CLICK, mc.getLocalPlayer()->getRuntimeID(), targetList[0]->stateVectorComponent->pos.sub(Vec3<int>(0.f, 0.2f, 0.f)));

								//mc.getClientInstance()->loopbackPacketSender->send(&inter);
								gm->attack(targetList[0]); //we do 3 to do more damage.
								if (noSwing)
									return;
								if (!targetList.empty() && mc.getLocalPlayer() != nullptr && !noSwing)
									localPlayer->swing();
							}
						}
					}
				}


				if (shouldSwitch && switchMode == 2) {
					plrInv->selectedSlot = oldSlot;
				}

				attackDelayTick = 0;
			}
			}
			else {
				attackDelayTick++;
			}
		}
		else {
			isInRange = false;
			targetList.clear();
			TargetDis = 0;
		}
	}
void Killaura::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (!targetList.empty() && rotMode == 1 && packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->rotation = rotAngle;
		authPacket->headYaw = rotAngle.y;
	}

	if (!targetList.empty() && rotMode == 1 && packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->rotation = rotAngle;
		movepacket->headYaw = rotAngle.y;
	}
}

void Killaura::onDisable()
{
	mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
}