#pragma once

enum class ActorDataIDs : unsigned short {
    Reserved0                        = 0x0,
    StructuralIntegrity              = 0x1,
    Variant                          = 0x2,
    ColorIndex                       = 0x3,
    Name                             = 0x4,
    Owner                            = 0x5,
    Target                           = 0x6,
    AirSupply                        = 0x7,
    EffectColor                      = 0x8,
    Reservec009                      = 0x9,
    Reserved010                      = 0xA,
    Hurt                             = 0xB,
    HurtDir                          = 0xC,
    RowTimeLeft                      = 0xD,
    RowTimeRight                     = 0xE,
    Value                            = 0xF,
    DisplayTileRuntimeId             = 0x10,
    DisplayOffset                    = 0x11,
    CustomDisplay                    = 0x12,
    Swell                            = 0x13,
    OldSwell                         = 0x14,
    SwellDir                         = 0x15,
    ChargeAmount                     = 0x16,
    CarryBlockRuntimeId              = 0x17,
    ClientEvent                      = 0x18,
    UsingItem                        = 0x19,
    PlayerFlags                      = 0x1A,
    PlayerIndex                      = 0x1B,
    BedPosition                      = 0x1C,
    XPower                           = 0x1D,
    YPower                           = 0x1E,
    ZPower                           = 0x1F,
    AuxPower                         = 0x20,
    FishX                            = 0x21,
    FishZ                            = 0x22,
    Fishangle                        = 0x23,
    AuxValueData                     = 0x24,
    LeashHolder                      = 0x25,
    Reserved038                      = 0x26,
    HasNpc                           = 0x27,
    NpcData                          = 0x28,
    Actions                          = 0x29,
    AirSupplyMax                     = 0x2A,
    MarkVariant                      = 0x2B,
    ContainerType                    = 0x2C,
    ContainerSize                    = 0x2D,
    ContainerStrengthModifier        = 0x2E,
    BlockTarget                      = 0x2F,
    Inv                              = 0x30,
    TargetA                          = 0x31,
    TargetB                          = 0x32,
    TargetC                          = 0x33,
    AerialAttack                     = 0x34,
    Reserved053                      = 0x35,
    Reserved054                      = 0x36,
    FuseTime                         = 0x37,
    Reservec056                      = 0x38,
    SeatLockPassengerRotation        = 0x39,
    SeatLockPassengerRotationDegrees = 0x3A,
    SeatRotationOffset               = 0x3B,
    SeatRotationOffsetDegrees        = 0x3C,
    DataRadius                       = 0x3D,
    DataWaiting                      = 0x3E,
    DataParticle                     = 0x3F,
    PeekId                           = 0x40,
    AttachFace                       = 0x41,
    Attached                         = 0x42,
    AttachPos                        = 0x43,
    TradeTarget                      = 0x44,
    Career                           = 0x45,
    HasCommandBlock                  = 0x46,
    CommandName                      = 0x47,
    LastCommandOutput                = 0x48,
    TrackCommandOutput               = 0x49,
    Reserved074                      = 0x4A,
    Strength                         = 0x4B,
    StrengthMax                      = 0x4C,
    DataSpellCastingColor            = 0x4D,
    DataLifetimeTicks                = 0x4E,
    PoseIndex                        = 0x4F,
    DataTickOffset                   = 0x50,
    NametagAlwaysShow                = 0x51,
    Color2Index                      = 0x52,
    NameAuthor                       = 0x53,
    Score                            = 0x54,
    BalloonAnchor                    = 0x55,
    PuffedState                      = 0x56,
    BubbleTime                       = 0x57,
    Agent                            = 0x58,
    SittingAmount                    = 0x59,
    SittingAmountPrevious            = 0x5A,
    EatingCounter                    = 0x5B,
    Reserved092                      = 0x5C,
    LayingAmount                     = 0x5D,
    LayingAmountPrevious             = 0x5E,
    DataDuration                     = 0x5F,
    DataSpawnTimeDeprecated          = 0x60,
    DataChangeRate                   = 0x61,
    DataChangeOnPickup               = 0x62,
    DataPickupCount                  = 0x63,
    InteractText                     = 0x64,
    TradeTier                        = 0x65,
    MaxTradeTier                     = 0x66,
    TradeExperience                  = 0x67,
    SkinId                           = 0x68,
    SpawningFrames                   = 0x69,
    CommandBlockTickDelay            = 0x6A,
    CommandBlockExecuteOnFirstTick   = 0x6B,
    AmbientSoundInterval             = 0x6C,
    AmbientSoundIntervalRange        = 0x6D,
    AmbientSoundEventName            = 0x6E,
    FallDamageMultiplier             = 0x6F,
    NameRawText                      = 0x70,
    CanRideTarget                    = 0x71,
    LowTierCuredTradeDiscount        = 0x72,
    HighTierCuredTradeDiscount       = 0x73,
    NearbyCuredTradeDiscount         = 0x74,
    NearbyCuredDiscountTimeStamp     = 0x75,
    Hitbox                           = 0x76,
    IsBuoyant                        = 0x77,
    FreezingEffectStrength           = 0x78,
    BuoyancyData                     = 0x79,
    GoatHornCount                    = 0x7A,
    BaseRuntimeId                    = 0x7B,
    MovementSoundDistanceOffset      = 0x7C,
    HeartbeatIntervalTicks           = 0x7D,
    HeartbeatSoundEvent              = 0x7E,
    PlayerLastDeathPos               = 0x7F,
    PlayerLastDeathDimension         = 0x80,
    PlayerHasDied                    = 0x81,
    CollisionBox                     = 0x82,
    VisibleMobEffects                = 0x83,
    Count                            = 0x84,
};