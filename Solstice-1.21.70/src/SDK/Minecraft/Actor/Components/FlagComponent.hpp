//
// Created by vastrakai on 7/10/2024.
//

#pragma once
//#define DEFINE_FLAG(name) struct name {}; // this is stupid lmao

struct TargetCameraSetInitialOrientationComponent {};
struct CameraAlignWithTargetForwardComponent {};
struct ExtendPlayerRenderingComponent {};
struct ActiveCameraComponent {};
struct CurrentInputCameraComponent {};
struct CameraFirstPersonComponent {};
struct CameraBobComponent {};
struct CameraThirdPersonComponent {};
struct PlayerStateAffectsRenderingComponent {};

struct SetMovingFlagRequestComponent {};
struct CameraRenderFirstPersonObjectsComponent {};
struct CameraRenderPlayerModelComponent {};
struct RedirectCameraInputComponent {};
struct RenderCameraComponent {};
struct GameCameraComponent {};
struct OnFireComponent {};
struct MoveRequestComponent {};
struct DebugCameraIsActiveComponent
{
    PAD(0x10);
};
struct AllowInsideBlockRenderComponent {};
class NameableComponent
{
    PAD(0x8);
};

struct InteractPreventDefaultFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct InteractPreventDefaultFlagComponent>(void) noexcept
struct OnGroundFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct OnGroundFlagComponent>(void) noexcept
struct HasTeleportedFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct HasTeleportedFlagComponent>(void) noexcept
struct AntiCheatRewindFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct AntiCheatRewindFlagComponent>(void) noexcept
struct PermissionFlyFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PermissionFlyFlagComponent>(void) noexcept
struct MobFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct MobFlagComponent>(void) noexcept
struct ExitFromPassengerFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ExitFromPassengerFlagComponent>(void) noexcept
struct CanVehicleSprintFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct CanVehicleSprintFlagComponent>(void) noexcept
struct ActorIsBeingDestroyedFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorIsBeingDestroyedFlagComponent>(void) noexcept
struct HasLightweightFamilyFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct HasLightweightFamilyFlagComponent>(void) noexcept
struct CanStandOnSnowFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct CanStandOnSnowFlagComponent>(void) noexcept
struct PrevPosRotSetThisTickFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PrevPosRotSetThisTickFlagComponent>(void) noexcept
struct IsHorizontalPoseFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IsHorizontalPoseFlagComponent>(void) noexcept
struct HorizontalCollisionFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct HorizontalCollisionFlagComponent>(void) noexcept
struct ActorRemovedFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorRemovedFlagComponent>(void) noexcept
struct ActorHeadInWaterFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorHeadInWaterFlagComponent>(void) noexcept
struct ActorHeadWasInWaterFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorHeadWasInWaterFlagComponent>(void) noexcept
struct AirTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct AirTravelFlagComponent>(void) noexcept
struct PowerJumpFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PowerJumpFlagComponent>(void) noexcept
struct DashJumpFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct DashJumpFlagComponent>(void) noexcept
struct LiquidTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct LiquidTravelFlagComponent>(void) noexcept
struct LavaTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct LavaTravelFlagComponent>(void) noexcept
struct GlidingTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct GlidingTravelFlagComponent>(void) noexcept
struct GroundTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct GroundTravelFlagComponent>(void) noexcept
struct ArmorFlyEnabledFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ArmorFlyEnabledFlagComponent>(void) noexcept
struct WaterTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WaterTravelFlagComponent>(void) noexcept
struct SkeletonFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SkeletonFlagComponent>(void) noexcept
struct WasOnGroundFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WasOnGroundFlagComponent>(void) noexcept
struct PlayerIsSleepingFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PlayerIsSleepingFlagComponent>(void) noexcept
struct PassengersChangedFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PassengersChangedFlagComponent>(void) noexcept
struct WasInLavaFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WasInLavaFlagComponent>(void) noexcept
struct MinecartFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct MinecartFlagComponent>(void) noexcept
struct NeverChangesSizeFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct NeverChangesSizeFlagComponent>(void) noexcept
struct BoatFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct BoatFlagComponent>(void) noexcept
struct ThrownTridentFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ThrownTridentFlagComponent>(void) noexcept
struct WasInWaterFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WasInWaterFlagComponent>(void) noexcept
struct MobIsJumpingFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct MobIsJumpingFlagComponent>(void) noexcept
struct SquidFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SquidFlagComponent>(void) noexcept
struct HorseFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct HorseFlagComponent>(void) noexcept
struct CamelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct CamelFlagComponent>(void) noexcept
struct SlimeFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SlimeFlagComponent>(void) noexcept
struct ParrotFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ParrotFlagComponent>(void) noexcept
struct SheepFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SheepFlagComponent>(void) noexcept
struct ShulkerFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ShulkerFlagComponent>(void) noexcept
struct SwitchingVehiclesFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SwitchingVehiclesFlagComponent>(void) noexcept
struct NeedSetPreviousPositionFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct NeedSetPreviousPositionFlagComponent>(void) noexcept
struct IsDeadFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IsDeadFlagComponent>(void) noexcept
struct SimulatedPlayerFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SimulatedPlayerFlagComponent>(void) noexcept
struct MobAllowStandSlidingFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct MobAllowStandSlidingFlagComponent>(void) noexcept
struct VerticalCollisionFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct VerticalCollisionFlagComponent>(void) noexcept
struct WaterAnimalFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WaterAnimalFlagComponent>(void) noexcept
struct VillagerV2FlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct VillagerV2FlagComponent>(void) noexcept
struct VexFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct VexFlagComponent>(void) noexcept
struct SoulSpeedEnchantFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SoulSpeedEnchantFlagComponent>(void) noexcept
struct FreezeImmuneFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct FreezeImmuneFlagComponent>(void) noexcept
struct ActorIsImmobileFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorIsImmobileFlagComponent>(void) noexcept
struct ActorIsKnockedBackOnDeathFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorIsKnockedBackOnDeathFlagComponent>(void) noexcept
struct ShouldAwardWhoNeedsRocketsAchievementFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ShouldAwardWhoNeedsRocketsAchievementFlagComponent>(void) noexcept
struct DiscardFrictionFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct DiscardFrictionFlagComponent>(void) noexcept
struct AutoClimbTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct AutoClimbTravelFlagComponent>(void) noexcept
struct WitherBossFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WitherBossFlagComponent>(void) noexcept
struct FireworksRocketFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct FireworksRocketFlagComponent>(void) noexcept
struct PandaFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PandaFlagComponent>(void) noexcept
struct ItemActorFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ItemActorFlagComponent>(void) noexcept
struct TropicalFishFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct TropicalFishFlagComponent>(void) noexcept
struct HorseLandedOnGroundFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct HorseLandedOnGroundFlagComponent>(void) noexcept
struct WolfFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WolfFlagComponent>(void) noexcept
struct CollidableMobNearFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct CollidableMobNearFlagComponent>(void) noexcept
struct ShulkerBulletFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ShulkerBulletFlagComponent>(void) noexcept
struct IsNearDolphinsFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IsNearDolphinsFlagComponent>(void) noexcept
struct AutoStepRequestFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct AutoStepRequestFlagComponent>(void) noexcept
struct WitherSkullFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WitherSkullFlagComponent>(void) noexcept
struct WitchFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WitchFlagComponent>(void) noexcept
struct StandOnHoneyOrSlimeBlockFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct StandOnHoneyOrSlimeBlockFlagComponent>(void) noexcept
struct BlazeFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct BlazeFlagComponent>(void) noexcept
struct StandOnOtherBlockFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct StandOnOtherBlockFlagComponent>(void) noexcept
struct EjectedByActivatorRailFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct EjectedByActivatorRailFlagComponent>(void) noexcept
struct IgnoresEntityInsideFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IgnoresEntityInsideFlagComponent>(void) noexcept
struct ScanForDolphinFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ScanForDolphinFlagComponent>(void) noexcept
struct LavaSlimeFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct LavaSlimeFlagComponent>(void) noexcept
struct IsPanickingFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IsPanickingFlagComponent>(void) noexcept
struct AgentFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct AgentFlagComponent>(void) noexcept
struct PaintingFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PaintingFlagComponent>(void) noexcept
struct SkipBodySlotUpgradeFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SkipBodySlotUpgradeFlagComponent>(void) noexcept
struct SkipPlayerTickSystemFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SkipPlayerTickSystemFlagComponent>(void) noexcept
struct HangingActorFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct HangingActorFlagComponent>(void) noexcept
struct FishingHookFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct FishingHookFlagComponent>(void) noexcept
struct MobIsImmobileFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct MobIsImmobileFlagComponent>(void) noexcept
struct ActorIsFirstTickFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorIsFirstTickFlagComponent>(void) noexcept
struct RemoveInPeacefulFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct RemoveInPeacefulFlagComponent>(void) noexcept
struct IllagerBeastFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IllagerBeastFlagComponent>(void) noexcept
struct EyeOfEnderFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct EyeOfEnderFlagComponent>(void) noexcept
struct ExperienceOrbFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ExperienceOrbFlagComponent>(void) noexcept
struct EnderDragonFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct EnderDragonFlagComponent>(void) noexcept
struct PrimedTntFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct PrimedTntFlagComponent>(void) noexcept
struct IsFishableFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IsFishableFlagComponent>(void) noexcept
struct ChickenFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ChickenFlagComponent>(void) noexcept
struct BatFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct BatFlagComponent>(void) noexcept
struct JoinRaidQueuedFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct JoinRaidQueuedFlagComponent>(void) noexcept
struct WasHandledBySculkCatalystFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct WasHandledBySculkCatalystFlagComponent>(void) noexcept
struct IsChasingDuringPlayFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct IsChasingDuringPlayFlagComponent>(void) noexcept
struct EntityNeedsInitializeFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct EntityNeedsInitializeFlagComponent>(void) noexcept
struct NeedsUpgradeToBodySlotFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct NeedsUpgradeToBodySlotFlagComponent>(void) noexcept
struct CanSeeInvisibleFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct CanSeeInvisibleFlagComponent>(void) noexcept
struct ActorAddedFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorAddedFlagComponent>(void) noexcept
struct ActorDataHorseFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorDataHorseFlagComponent>(void) noexcept
struct GuardianFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct GuardianFlagComponent>(void) noexcept
struct EnderManFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct EnderManFlagComponent>(void) noexcept
struct MonsterFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct MonsterFlagComponent>(void) noexcept
struct SpiderFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct SpiderFlagComponent>(void) noexcept
struct MoveTowardsClosestSpaceFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct MoveTowardsClosestSpaceFlagComponent>(void) noexcept
struct ActorLocalPlayerEntityMovedFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorLocalPlayerEntityMovedFlagComponent>(void) noexcept
struct DolphinFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct DolphinFlagComponent>(void) noexcept
struct CollisionFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct CollisionFlagComponent>(void) noexcept
struct BreaksFallingBlocksFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct BreaksFallingBlocksFlagComponent>(void) noexcept
struct FallingBlockFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct FallingBlockFlagComponent>(void) noexcept
struct ProjectileFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ProjectileFlagComponent>(void) noexcept
struct CollidableMobFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct CollidableMobFlagComponent>(void) noexcept
struct BeeFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct BeeFlagComponent>(void) noexcept
struct LevitateTravelFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct LevitateTravelFlagComponent>(void) noexcept
struct KeepRidingEvenIfTooLargeForVehicleFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct KeepRidingEvenIfTooLargeForVehicleFlagComponent>(void) noexcept
struct ReplayStateLenderFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ReplayStateLenderFlagComponent>(void) noexcept
struct FishFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct FishFlagComponent>(void) noexcept
struct ActorChunkMoveFlagComponent {}; // string: auto __cdecl entt::internal::stripped_type_name<struct ActorChunkMoveFlagComponent>(void) noexcep