#pragma once
//#include "../../../EnTT/EnTT.hpp" // Including entt here to avoid overriding.
#include "../../../../Libs/entt/entt/entt.hpp"
//#include "../../../../Libs/entt/entt/fwd.hpp"

#include "../Attribute/Attribute.h"
#include "../Attribute/BaseAttributeMap.h"
#include "../GameMode/GameType.h"
#include "ActorDefinitionIdentifier.h"
//#include "../Attribute/AttributeInstance.h"

#include "../../Components/AttributesComponent.h"
#include "../../Components/ActorDefinitionIdentifierComponent.h"
#include "../../Components/ActorTypeComponent.h"
#include "../../Components/ActorGameTypeComponent.h"
#include "../../Components/ActorUniqueIDComponent.h"
#include "../../Components/JumpComponent.h"
#include "../../Components/RuntimeIDComponent.h"
#include "../../Components/DepenetrationComponent.h"
#include "../../Components/StateVectorComponent.h"
#include "../../Components/AABBShapeComponent.h"
#include "../../Components/ActorEquipmentComponent.h"
#include "../../Components/ActorHeadRotationComponent.h"
#include "../../Components/ActorRotationComponent.h"
#include "../../Components/BlockMovementSlowdownMultiplierComponent.h"
#include "../../Components/FallDistanceComponent.h"
#include "../../Components/MaxAutoStepComponent.h"
#include "../../Components/MobHurtTimeComponent.h"
#include "../../Components/MobBodyRotationComponent.h"
#include "../../Components/RenderPositionComponent.h"
#include "../../Components/MoveInputComponent.h"
#include "../../Components/MovementInterpolatorComponent.h"
#include "../../Components/FlagComponent.h"
#include "../../Components/ActorDataFlagComponent.h"

#include "ActorFlags.h"
#include "SerializedSkin.h"

#include "EntityIdTraits.h"
#include "EntityID.h"
#include "EntityContext.h"

#pragma region Classes & Structs

//class EntityRegistry;
class ActorLocation;
class ActorDamageSource;
class NewInteractionModel;
class UIProfanityContext;
class ActorUniqueID;
class ActorDamageCause;
class ItemStack;
class ActorEvent;
class EquipmentSlot;
class IContainerManager;
class DataLoadHelper;
class ActorLink;
class LevelSoundEvent;
class AnimationComponent;
class RenderParams;
class HandSlot;
class EquipmentTableDefinition;
class Options;
class ActorInteraction;
class FrameUpdateContextBase;
class ItemStackBase;
class MobEffectInstance;
class Attribute;
class AnimationComponentGroupType;
class ItemUseMethod;
class ResolvedTextObject;
class INpcDialogueData;
class IConstBlockSource;
class ChalkboardBlockPlayer;
class BlockPlayer;
class Block;
class Tick;
class ChunkSource;
class LayeredAbilities;
class ChunkPos;
class MovementEventType;
class PlayerMovementSettings;
class Item;
class Container;
class EventPacket;
class ContainerManagerModel;

class ActorInitializationMethod;
class InitializationMethod;
class VariantParameterList;

class ChalkboardBlockActor;
class ResolvedTextObject;
struct TextObjectRoot;
class SubChunkPos;
class ChunkSource;
class LayeredAbilities;

class ContainerContentChangeListener;

// packets
class ChangeDimensionPacket;

#pragma endregion

struct PlayerStatus { Actor* player = nullptr; float value = 0; };

enum ArmorSlot {
	Helmet = 0,
	Chestplate = 1,
	Leggings = 2,
	Boots = 3
};

// Actor VTable
class Actor { // 1.21.73
public:
	void setSneaking(bool value) {
		Memory::CallVFunc<44, void, bool>(this, value);
	}

	bool isAlive() {
		return Memory::CallVFunc<47, bool>(this);
	}

	bool isOnFire() {
		return Memory::CallVFunc<48, bool>(this);
	}

	void stopSwimming() {
		Memory::CallVFunc<101, void>(this);
	}

	void swing() {
		Memory::CallVFunc<111, void>(this);
	}

public:
	template <typename T>
	T* getComponent() {
		return const_cast<T*>(getEntityContext()->getRegistry().try_get<T>(getEntityContext()->mEntity));
	}

	template <typename T>
    bool hasComponent() {
		return getEntityContext()->getRegistry().all_of<T>(getEntityContext()->mEntity);
	}

	template <typename T>
	void getOrEmplaceComponent() {
		return getEntityContext()->getRegistry().get_or_emplace<T>(getEntityContext()->mEntity);
	}

	template <typename T>
	void removeComponent() {
		getEntityContext()->getRegistry().remove<T>(getEntityContext()->mEntity);
	}

	PlayerInventory* getSupplies() {
		uintptr_t address = reinterpret_cast<uintptr_t>(this);
		return *reinterpret_cast<PlayerInventory**>(address + 0x5C0); // 1.21.73
	}

	ContainerManagerModel* getContainerManagerModel() {
		uintptr_t address = reinterpret_cast<uintptr_t>(this);
		return *reinterpret_cast<ContainerManagerModel**>(address + 0x5A8); // 1.21.73
	}

	ItemStack* getSelectedItem() {
		auto supplies = getSupplies();
		return supplies->inventory->getItem(supplies->hotbarSlot);
	}

	std::string* getNametag() {
		static void* signature;

		if (signature == NULL) {
			signature = Memory::findSig(xorstr_("48 83 EC 28 48 8B 81 28 01 00 00 48 85 C0 74 4F"));
		}

		auto getNameTag = reinterpret_cast<std::string*(__thiscall*)(Actor*)>(signature); // NameTags signature address
		return getNameTag(this); // Return FunctionCall as Actor
	}

	void setNametag(std::string* name) {
		static void* sig;

		if (sig == NULL) {
			sig = Memory::findSig("48 89 5C 24 18 48 89 74 24 20 57 48 83 EC 60 48 8B F2 48 8B F9 48 8B");
		}

		auto fn = reinterpret_cast<void(__thiscall*)(Actor*, std::string*)>(sig);
		return fn(this, name);
	}

	AttributesComponent* getAttributesComponent() {
		return getComponent<AttributesComponent>();
	}

	AttributeInstance* getAttribute(int id) {
		auto& map = getAttributesComponent()->baseAttributeMap.attributes;
		auto it = map.find(id);
		if (it != map.end()) {
			return &it->second;
		}
		return nullptr;
	}

	void setInvisible(bool invisible) {
		static void* SetInvisableFunc = Memory::findSig("48 89 74 24 ? 57 48 83 EC 50 48 8B 01 0F B6 F2 48 8B F9 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 40 3A C6");

		auto setInvis = reinterpret_cast<void(__thiscall*)(Actor*, bool)>(SetInvisableFunc); // setInvisible signature address
		return setInvis(this, invisible); // Return FunctionCall as Actor
	}

	void setPos(Vector3<float> pos)
	{
		AABBShapeComponent* shape = getAABBShape();

		if (!shape)
			return;

		shape->PosLower = pos;
		shape->PosUpper = Vector3<float>(pos.x + shape->Hitbox.x, pos.y + shape->Hitbox.y, pos.z);
	}

	

	bool isBot()
	{
		AABBShapeComponent* shape = getAABBShape();
		MovementInterpolatorComponent* movement = getMovementInterpolator();

		if (!shape || !movement)
			return true;

		//if (shape->Hitbox.x != 0.6f || shape->Hitbox.y != 1.8f)
			//return true;

		if (shape->Hitbox.y < 1.5f || shape->Hitbox.x < 0.49f || shape->Hitbox.y > 2.1f || shape->Hitbox.x > 0.9f)
			return true;

		//if (IsBadReadPtr(shape, sizeof(AABBShapeComponent)))
			//return true;

		if (isImmobile() || getNametag()->length() <= 1 || getNametag()->find("game selector") != std::string::npos)
			return true;

		//if (movement->Rotations.x == 0 || movement->Rotations.y == 0)
			//return true;

		return false;
	}

	bool isTeammate(Actor* localPlayer) 
	{
		std::string localName = localPlayer->getNametag()->c_str();
		std::string targetName = this->getNametag()->c_str();
		std::vector < std::string > colorList;
		colorList.push_back(BLACK);
		colorList.push_back(DARK_BLUE);
		colorList.push_back(DARK_GREEN);
		colorList.push_back(DARK_AQUA);
		colorList.push_back(DARK_RED);
		colorList.push_back(DARK_PURPLE);
		colorList.push_back(GOLD);
		colorList.push_back(GRAY);
		colorList.push_back(DARK_GRAY);
		colorList.push_back(BLUE);
		colorList.push_back(GREEN);
		colorList.push_back(AQUA);
		colorList.push_back(RED);
		colorList.push_back(LIGHT_PURPLE);
		colorList.push_back(YELLOW);
		colorList.push_back(WHITE);
		for (auto& i : colorList) {
			if (localName.find(i) != std::string::npos && targetName.find(i) != std::string::npos) return true;
		}

		return false;
	}

	float getHorizontalSpeed(float timer = 20.f)
	{
		StateVectorComponent* stateVec = getComponent<StateVectorComponent>();

		if (!stateVec)
			return 0;

		Vector3<float> currentPosition = stateVec->Position;
		currentPosition.y = 0;

		Vector3<float> lastPosition = stateVec->PrevPosition;
		lastPosition.y = 0;

		return (currentPosition.distance(lastPosition)) * timer;
	}

	MoveInputComponent* getMoveInputHandler() {
		return getComponent<MoveInputComponent>();
	}
	
	JumpControlComponent* getJumpControl() {
		//return getComponent<JumpControlComponent>();
		static auto func = reinterpret_cast<void*>(Memory::getXref((uintptr_t)Memory::findSig(xorstr_("E8 ? ? ? ? 48 85 C0 74 ? C6 40 ? ? 48 83 C4 ? 5B"))));
		auto registryBase = reinterpret_cast<void*>(getEntityContext()->mRegistry);
		auto id = getEntityContext()->mEntity;
		return Memory::CallFunc<JumpControlComponent*>(func, registryBase, &id);
	}

	int32_t getHurtTime() {
		return getComponent<MobHurtTimeComponent>()->hurtTime;
	}

	float getPitch() {
		return getComponent<ActorRotationComponent>()->rotation.x;
	}

	void setPitch(float rotation) {
		getComponent<ActorRotationComponent>()->rotation.x = rotation;
	}

	float getHeadYaw() {
		return getComponent<ActorHeadRotationComponent>()->rotation.x;
	}

	void setHeadYaw(float rotation) {
		getComponent<ActorHeadRotationComponent>()->rotation.x = rotation;
	}

	float getBodyRotation() {
		return getComponent<MobBodyRotationComponent>()->bodyRot;
	}

	void setBodyRotation(float rotation) {
		getComponent<MobBodyRotationComponent>()->bodyRot = rotation;
	}

	AttributeInstance* getAttribute(AttributeId id) {
		return getAttribute((int)id);
	}

	float getAbsorption() {
		return getAttribute(AttributeId::Absorption)->currentValue;
	}

	float getHealth() {
		return getAttribute(AttributeId::Health)->currentValue;
	}

	int64_t getRuntimeID() {
		return getComponent<RuntimeIDComponent>()->runtimeID;
	}

	int64_t getUniqueID() {
		return getComponent<ActorUniqueIDComponent>()->uniqueID;
	}

	Vector3<float> getRenderPosition() {


		return getComponent<RenderPositionComponent>()->renderPos;
	}

	bool isPlayer() {
		return hasComponent<FlagComponent<PlayerComponentFlag>>();
	}

	bool isLocalPlayer() {
		return hasComponent<FlagComponent<LocalPlayerComponentFlag>>();
	}

	float getFallDistance() {
		return getComponent<FallDistanceComponent>()->fallDistance;
	}

	void setFallDistance(float distance) {
		getComponent<FallDistanceComponent>()->fallDistance = distance;
	}

	SimpleContainer* getArmorContainer() {
		return getComponent<ActorEquipmentComponent>()->mArmorContainer;
	}

	class ItemStack* getArmor(ArmorSlot slot) {
		return getArmorContainer()->getItem(slot);
	}

	int32_t getEntityTypeId() {
		return getComponent<ActorTypeComponent>()->type;
	}

	bool isStuckInCollider() {
		return getComponent<DepenetrationComponent>()->isPenetrating();
	}

	bool isCollidingHorizontal() {
		return hasComponent<FlagComponent<HorizontalCollisionFlag>>();
	}

	bool isInLava() {
		return hasComponent<FlagComponent<InLavaFlag>>();
	}

	bool isInWater() {
		return hasComponent<FlagComponent<InWaterFlag>>();
	}

	bool isImmobile() {
		return hasComponent<FlagComponent<ActorIsImmobileFlag>>();
	}

	void setImmobile(bool state) {
		if (state) { getOrEmplaceComponent<FlagComponent<ActorIsImmobileFlag>>(); }
		else { removeComponent<FlagComponent<ActorIsImmobileFlag>>(); }
	}

	

	bool isOnGround() {
		return hasComponent<OnGroundFlagComponent>();
	}

	void setIsOnGround(bool state) {
		if (state) { getOrEmplaceComponent<OnGroundFlagComponent>(); }
		else { removeComponent<OnGroundFlagComponent>(); }
	};

	void jumpFromGround() {
		getMoveInputHandler()->mJumping = true;
		getMoveInputHandler()->mInputState.mJumpDown = true;
		getMoveInputHandler()->mRawInputState.mJumpDown = true;
		Auau = true;

		//getOrEmplaceComponent<FlagComponent<JumpFromGroundRequestFlag>>();
	}

	bool getStatusFlag(ActorFlags flag) {
		return getComponent<ActorDataFlagComponent>()->flags[flag];
	}

	void setStatusFlag(ActorFlags flag, bool value) {
		getComponent<ActorDataFlagComponent>()->flags[flag] = value;
	}
	
	EntityContext* getEntityContext()
	{
		uintptr_t address = reinterpret_cast<uintptr_t>(this);
		return reinterpret_cast<EntityContext*>((uintptr_t)this + 0x8); // 1.21.73
	}
public:
	// Level Actor::getLevel(void)
	BUILD_ACCESS(class Level*, Level, 0x1D8); // 1.21.73
	// StateVectorComponent Actor::getPosition(void)
	BUILD_ACCESS(StateVectorComponent*, StateVector, 0x218); // 1.21.73
	// AABBShapeComponent Actor::getAABB(void) or StateVector + 8
	BUILD_ACCESS(AABBShapeComponent*, AABBShape, 0x220); // 1.21.73
	// MovementInterpolatorComponent Actor::getRotation(void) or StateVector + 16
	BUILD_ACCESS(MovementInterpolatorComponent*, MovementInterpolator, 0x228); // 1.21.73
};
