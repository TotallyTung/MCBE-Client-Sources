#pragma once

struct IEntityComponent {};

struct HorizontalCollisionFlagComponent {};
struct VerticalCollisionFlagComponent {};
struct ItemUseSlowdownModifierComponent {};
struct PlayerComponent {};
struct LocalPlayerComponent {};
struct JumpFromGroundRequestComponent {};  
struct OnFireComponent {};
struct ActorIsImmobileFlagComponent {};
struct WasInLavaFlagComponent {};
struct WasInWaterFlagComponent {};
struct GameCameraComponent {};
struct CameraRenderFirstPersonObjectsComponent {};
struct CameraRenderPlayerModelComponent {};
struct RenderCameraComponent {};
struct RedirectCameraInputComponent {};
struct MoveRequestComponent {};
struct PlayerIsSleepingFlagComponent {};
struct SendPacketsComponent {};
struct RecalculateControlledByLocalInstanceRequestComponent {};
struct PermissionFlyFlagComponent {};
struct CanSeeInvisibleFlagComponent {};
struct IsDeadFlagComponent {};
struct MobIsJumpingFlagComponent {};
struct TriggerJumpRequestComponent {};
struct MobOnPlayerJumpRequestComponent {};
struct MobJumpComponent {};

struct WasOnGroundFlagComponent{};
struct OnGroundFlagComponent {};
struct DebugCameraIsActiveComponent {};
struct PigFlag {};