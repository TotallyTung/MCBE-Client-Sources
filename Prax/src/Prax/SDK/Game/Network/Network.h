#pragma once


#include <Prax/SDK/Game/World/Block.h>
#include <Prax/Utils/System/Internet.h>
#include <Prax/Utils/System/Memory.h>
#include <xorstr.hpp>

class RakNetPacket;

enum class PacketID : int
{
    Login = 0x1,
    PlayStatus = 0x2,
    ServerToClientHandshake = 0x3,
    ClientToServerHandshake = 0x4,
    Disconnect = 0x5,
    ResourcePacksInfo = 0x6,
    ResourcePackStack = 0x7,
    ResourcePackResponse = 0x8,
    Text = 0x9,
    SetTime = 0xA,
    StartGame = 0xB,
    AddPlayer = 0xC,
    AddEntity = 0xD,
    RemoveEntity = 0xE,
    AddItemEntity = 0xF,
    TakeItemEntity = 0x11,
    MoveEntityAbsolute = 0x12,
    MovePlayer = 0x13,
    RiderJump = 0x14,
    UpdateBlock = 0x15,
    AddPainting = 0x16,
    TickSync = 0x17,
    LevelSoundEvent = 0x18,
    LevelEvent = 0x19,
    BlockEvent = 0x1A,
    EntityEvent = 0x1B,
    MobEffect = 0x1C,
    UpdateAttributes = 0x1D,
    InventoryTransaction = 0x1E,
    MobEquipment = 0x1F,
    MobArmorEquipment = 0x20,
    Interact = 0x21,
    BlockPickRequest = 0x22,
    EntityPickRequest = 0x23,
    PlayerAction = 0x24,
    EntityFall = 0x25,
    HurtArmor = 0x26,
    SetEntityData = 0x27,
    SetEntityMotion = 0x28,
    SetEntityLink = 0x29,
    SetHealth = 0x2A,
    SetSpawnPosition = 0x2B,
    Animate = 0x2C,
    Respawn = 0x2D,
    ContainerOpen = 0x2E,
    ContainerClose = 0x2F,
    PlayerHotbar = 0x30,
    InventoryContent = 0x31,
    InventorySlot = 0x32,
    ContainerSetData = 0x33,
    CraftingData = 0x34,
    CraftingEvent = 0x35,
    GUIDataPickItem = 0x36,
    AdventureSettings = 0x37,
    BlockEntityData = 0x38,
    PlayerInput = 0x39,
    LevelChunk = 0x3A,
    SetCommandsEnabled = 0x3B,
    SetDifficulty = 0x3C,
    ChangeDimension = 0x3D,
    SetPlayerGameType = 0x3E,
    PlayerList = 0x3F,
    SimpleEvent = 0x40,
    Event = 0x41,
    SpawnExperienceOrb = 0x42,
    MapItemData = 0x43,
    MapInfoRequest = 0x44,
    RequestChunkRadius = 0x45,
    ChunkRadiusUpdated = 0x46,
    ItemFrameDropItem = 0x47,
    GameRulesChanged = 0x48,
    Camera = 0x49,
    BossEvent = 0x4A,
    ShowCredits = 0x4B,
    AvailableCommands = 0x4C,
    CommandRequest = 0x4D,
    CommandBlockUpdate = 0x4E,
    CommandOutput = 0x4F,
    UpdateTrade = 0x50,
    UpdateEquip = 0x51,
    ResourcePackDataInfo = 0x52,
    ResourcePackChunkData = 0x53,
    ResourcePackChunkRequest = 0x54,
    Transfer = 0x55,
    PlaySound = 0x56,
    StopSound = 0x57,
    SetTitle = 0x58,
    AddBehaviorTree = 0x59,
    StructureBlockUpdate = 0x5A,
    ShowStoreOffer = 0x5B,
    PurchaseReceipt = 0x5C,
    PlayerSkin = 0x5D,
    SubClientLogin = 0x5E,
    AutomationClientConnect = 0x5F,
    SetLastHurtBy = 0x60,
    BookEdit = 0x61,
    NPCRequest = 0x62,
    PhotoTransfer = 0x63,
    ModelFormRequest = 0x64,
    ModelFormResponse = 0x65,
    ServerSettingsRequest = 0x66,
    ServerSettingsResponse = 0x67,
    ShowProfile = 0x68,
    SetDefaultGameType = 0x69,
    RemoveObjective = 0x6A,
    SetDisplayObjective = 0x6B,
    SetScore = 0x6C,
    LabTable = 0x6D,
    UpdateBlockSynced = 0x6E,
    MoveEntityDelta = 0x6F,
    SetScoreboardIdentity = 0x70,
    SetLocalPlayerAsInitialized = 0x71,
    UpdateSoftEnum = 0x72,
    NetworkStackLatency = 0x73,
    ScriptCustomEvent = 0x75,
    SpawnParticleEffect = 0x76,
    AvailableEntityIdentifiers = 0x77,
    LevelSoundEvent2 = 0x78,
    NetworkChunkPublisherUpdate = 0x79,
    BiomeDefinitionList = 0x7A,
    LevelSoundEvent3 = 0x7B,
    LevelEventGeneric = 0x7C,
    LecternUpdate = 0x7D,
    VideoStreamConnect = 0x7E,
    ClientCacheStatus = 0x81,
    OnScreenTextureAnimation = 0x82,
    MapCreateLockedCopy = 0x83,
    StructureTemplateDataExportRequest = 0x84,
    StructureTemplateDataExportResponse = 0x85,
    UpdateBlockProperties = 0x86,
    ClientCacheBlobStatus = 0x87,
    ClientCacheMissResponse = 0x88,
    CompletedUsingItem = 0x8E,
    NetworkSettings = 0x8F,
    PlayerAuthInput = 0x90,
    CreativeContent = 0x91,
    PlayerEnchantOptions = 0x92,
    ItemStackRequest = 0x93,
    ItemStackResponse = 0x94,
    PlayerArmorDamage = 0x95,
    CodeBuilder = 0x96,
    UpdatePlayerGameType = 0x97,
    EmoteList = 0x98,
    PositionTrackingDBServerBroadcast = 0x99,
    PositionTrackingDBClientRequest = 0x9A,
    DebugInfo = 0x9B,
    PacketViolationWarning = 0x9C,
    AnimateEntity = 0x9D,
    ItemComponent = 0xA2,
    FilterText = 0xA3,
    CameraShake = 0x9F,
    PlayerFogSetting = 0xA0,
    CorrectPlayerMovePredictionPacket = 0xA1,
    ClientBoundDebugRendererPacket = 0xA4,
    SyncActorProperty = 0xA5,
    AddVolumeEntityPacket = 0xA6,
    RemoveVolumeEntityPacket = 0xA7,
    SimulationTypePacket = 0xA8,
    NpcDialoguePacket = 0xA9,
    EduUriResourcePacket = 0xAA,
    CreatePhotoPacket = 0xAB,
    UpdateSubChunkBlocks = 0xAC,
    PhotoInfoRequest_DEPRECATED = 0xAD,
    SubChunkPacket = 0xAE,
    SubChunkRequestPacket = 0xAF,
    PlayerStartItemCooldown = 0xB0,
    ScriptMessagePacket = 0xB1,
    CodeBuilderSourcePacket = 0xB2,
    TickingAreasLoadStatus = 0xB3,
    DimensionDataPacket = 0xB4,
    AgentAction = 0xB5,
    ChangeMobProperty = 0xB6,
    LessonProgressPacket = 0xB7,
    RequestAbilityPacket = 0xB8,
    RequestPermissionsPacket = 0xB9,
    ToastRequest = 0xBA,
    UpdateAbilitiesPacket = 0xBB,
    UpdateAdventureSettingsPacket = 0xBC,
    DeathInfo = 0xBD,
    EditorNetworkPacket = 0xBE,
    FeatureRegistryPacket = 0xBF,
    ServerStats = 0xC0,
    RequestNetworkSettings = 0xC1,
    GameTestRequestPacket = 0xC2,
    GameTestResultsPacket = 0xC3,
    PlayerClientInputPermissions = 0xC4,
    ClientCheatAbilityPacket = 0xC5,
    CameraPresets = 0xC6,
    UnlockedRecipes = 0xC7,
    CameraInstruction = 0x12C,
    CompressedBiomeDefinitionList = 0x12D,
    TrimData = 0x12E,
    OpenSign = 0x12F,
    AgentAnimation = 0x130,
    RefreshEntitlements = 0x131,
    EndId,
};

enum class LevelSoundEvent
{
    ITEM_USE_ON = 0,
    HIT = 1,
    STEP = 2,
    FLY = 3,
    JUMP = 4,
    BREAK = 5,
    PLACE = 6,
    HEAVY_STEP = 7,
    GALLOP = 8,
    FALL = 9,
    AMBIENT = 10,
    AMBIENT_BABY = 11,
    AMBIENT_IN_WATER = 12,
    BREATHE = 13,
    DEATH = 14,
    DEATH_IN_WATER = 15,
    DEATH_TO_ZOMBIE = 16,
    HURT = 17,
    HURT_IN_WATER = 18,
    MAD = 19,
    BOOST = 20,
    BOW = 21,
    SQUISH_BIG = 22,
    SQUISH_SMALL = 23,
    FALL_BIG = 24,
    FALL_SMALL = 25,
    SPLASH = 26,
    FIZZ = 27,
    FLAP = 28,
    SWIM = 29,
    DRINK = 30,
    EAT = 31,
    TAKEOFF = 32,
    SHAKE = 33,
    PLOP = 34,
    LAND = 35,
    SADDLE = 36,
    ARMOR = 37,
    MOB_ARMOR_STAND_PLACE = 38,
    ADD_CHEST = 39,
    THROW = 40,
    ATTACK = 41,
    ATTACK_NODAMAGE = 42,
    ATTACK_STRONG = 43,
    WARN = 44,
    SHEAR = 45,
    MILK = 46,
    THUNDER = 47,
    EXPLODE = 48,
    FIRE = 49,
    IGNITE = 50,
    FUSE = 51,
    STARE = 52,
    SPAWN = 53,
    SHOOT = 54,
    BREAK_BLOCK = 55,
    LAUNCH = 56,
    BLAST = 57,
    LARGE_BLAST = 58,
    TWINKLE = 59,
    REMEDY = 60,
    UNFECT = 61,
    LEVELUP = 62,
    BOW_HIT = 63,
    BULLET_HIT = 64,
    EXTINGUISH_FIRE = 65,
    ITEM_FIZZ = 66,
    CHEST_OPEN = 67,
    CHEST_CLOSED = 68,
    SHULKERBOX_OPEN = 69,
    SHULKERBOX_CLOSED = 70,
    ENDERCHEST_OPEN = 71,
    ENDERCHEST_CLOSED = 72,
    POWER_ON = 73,
    POWER_OFF = 74,
    ATTACH = 75,
    DETACH = 76,
    DENY = 77,
    TRIPOD = 78,
    POP = 79,
    DROP_SLOT = 80,
    NOTE = 81,
    THORNS = 82,
    PISTON_IN = 83,
    PISTON_OUT = 84,
    PORTAL = 85,
    WATER = 86,
    LAVA_POP = 87,
    LAVA = 88,
    BURP = 89,
    BUCKET_FILL_WATER = 90,
    BUCKET_FILL_LAVA = 91,
    BUCKET_EMPTY_WATER = 92,
    BUCKET_EMPTY_LAVA = 93,
    ARMOR_EQUIP_CHAIN = 94,
    ARMOR_EQUIP_DIAMOND = 95,
    ARMOR_EQUIP_GENERIC = 96,
    ARMOR_EQUIP_GOLD = 97,
    ARMOR_EQUIP_IRON = 98,
    ARMOR_EQUIP_LEATHER = 99,
    ARMOR_EQUIP_ELYTRA = 100,
    RECORD_13 = 101,
    RECORD_CAT = 102,
    RECORD_BLOCKS = 103,
    RECORD_CHIRP = 104,
    RECORD_FAR = 105,
    RECORD_MALL = 106,
    RECORD_MELLOHI = 107,
    RECORD_STAL = 108,
    RECORD_STRAD = 109,
    RECORD_WARD = 110,
    RECORD_11 = 111,
    RECORD_WAIT = 112,
    STOP_RECORD = 113, // empty sound :o
    FLOP = 114,
    ELDERGUARDIAN_CURSE = 115,
    MOB_WARNING = 116,
    MOB_WARNING_BABY = 117,
    TELEPORT = 118,
    SHULKER_OPEN = 119,
    SHULKER_CLOSE = 120,
    HAGGLE = 121,
    HAGGLE_YES = 122,
    HAGGLE_NO = 123,
    HAGGLE_IDLE = 124,
    CHORUSGROW = 125,
    CHORUSDEATH = 126,
    GLASS = 127,
    POTION_BREWED = 128,
    CAST_SPELL = 129,
    PREPARE_ATTACK = 130,
    PREPARE_SUMMON = 131,
    PREPARE_WOLOLO = 132,
    FANG = 133,
    CHARGE = 134,
    CAMERA_TAKE_PICTURE = 135,
    LEASHKNOT_PLACE = 136,
    LEASHKNOT_BREAK = 137,
    GROWL = 138,
    WHINE = 139,
    PANT = 140,
    PURR = 141,
    PURREOW = 142,
    DEATH_MIN_VOLUME = 143,
    DEATH_MID_VOLUME = 144,
    IMITATE_BLAZE = 145,
    IMITATE_CAVE_SPIDER = 146,
    IMITATE_CREEPER = 147,
    IMITATE_ELDER_GUARDIAN = 148,
    IMITATE_ENDER_DRAGON = 149,
    IMITATE_ENDERMAN = 150,
    IMITATE_ENDERMITE = 151,
    IMITATE_EVOCATION_ILLAGER = 152,
    IMITATE_GHAST = 153,
    IMITATE_HUSK = 154,
    IMITATE_ILLUSION_ILLAGER = 155,
    IMITATE_MAGMA_CUBE = 156,
    IMITATE_POLAR_BEAR = 157,
    IMITATE_SHULKER = 158,
    IMITATE_SILVERFISH = 159,
    IMITATE_SKELETON = 160,
    IMITATE_SLIME = 161,
    IMITATE_SPIDER = 162,
    IMITATE_STRAY = 163,
    IMITATE_VEX = 164,
    IMITATE_VINDICATION_ILLAGER = 165,
    IMITATE_WITCH = 166,
    IMITATE_WITHER = 167,
    IMITATE_WITHER_SKELETON = 168,
    IMITATE_WOLF = 169,
    IMITATE_ZOMBIE = 170,
    IMITATE_ZOMBIE_PIGMAN = 171,
    IMITATE_ZOMBIE_VILLAGER = 172,
    BLOCK_END_PORTAL_FRAME_FILL = 173,
    BLOCK_END_PORTAL_SPAWN = 174,
    RANDOM_ANVIL_USE = 175,
    BOTTLE_DRAGONBREATH = 176,
    PORTAL_TRAVEL = 177,
    ITEM_TRIDENT_HIT = 178,
    ITEM_TRIDENT_RETURN = 179,
    ITEM_TRIDENT_RIPTIDE_1 = 180,
    ITEM_TRIDENT_RIPTIDE_2 = 181,
    ITEM_TRIDENT_RIPTIDE_3 = 182,
    ITEM_TRIDENT_THROW = 183,
    ITEM_TRIDENT_THUNDER = 184,
    ITEM_TRIDENT_HIT_GROUND = 185,
    DEFAULT = 186,
    BLOCK_FLETCHING_TABLE_USE = 187,
    ELEMCONSTRUCT_OPEN = 188,
    ICEBOMB_HIT = 189,
    BALLOONPOP = 190,
    LT_REACTION_ICEBOMB = 191,
    LT_REACTION_BLEACH = 192,
    LT_REACTION_EPASTE = 193,
    LT_REACTION_EPASTE2 = 194,
    LT_REACTION_GLOW_STICK = 195,
    LT_REACTION_GLOW_STICK_2 = 196,
    LT_REACTION_LUMINOL = 197,
    LT_REACTION_SALT = 198,
    LT_REACTION_FERTILIZER = 199,
    LT_REACTION_FIREBALL = 200,
    LT_REACTION_MGSALT = 201,
    LT_REACTION_MISCFIRE = 202,
    LT_REACTION_FIRE = 203,
    LT_REACTION_MISCEXPLOSION = 204,
    LT_REACTION_MISCMYSTICAL = 205,
    LT_REACTION_MISCMYSTICAL2 = 206,
    LT_REACTION_PRODUCT = 207,
    SPARKLER_USE = 208,
    GLOWSTICK_USE = 209,
    SPARKLER_ACTIVE = 210,
    CONVERT_TO_DROWNED = 211,
    BUCKET_FILL_FISH = 212,
    BUCKET_EMPTY_FISH = 213,
    BUBBLE_UP = 214,
    BUBBLE_DOWN = 215,
    BUBBLE_POP = 216,
    BUBBLE_UPINSIDE = 217,
    BUBBLE_DOWNINSIDE = 218,
    HURT_BABY = 219,
    DEATH_BABY = 220,
    STEP_BABY = 221,
    SPAWN_BABY = 222,
    BORN = 223,
    BLOCK_TURTLE_EGG_BREAK = 224,
    BLOCK_TURTLE_EGG_CRACK = 225,
    BLOCK_TURTLE_EGG_HATCH = 226,
    LAY_EGG = 227,
    BLOCK_TURTLE_EGG_ATTACK = 228,
    BEACON_ACTIVATE = 229,
    BEACON_AMBIENT = 230,
    BEACON_DEACTIVATE = 231,
    BEACON_POWER = 232,
    CONDUIT_ACTIVATE = 233,
    CONDUIT_AMBIENT = 234,
    CONDUIT_ATTACK = 235,
    CONDUIT_DEACTIVATE = 236,
    CONDUIT_SHORT = 237,
    SWOOP = 238,
    BLOCK_BAMBOO_SAPLING_PLACE = 239,
    PRESNEEZE = 240,
    SNEEZE = 241,
    AMBIENT_TAME = 242,
    SCARED = 243,
    BLOCK_SCAFFOLDING_CLIMB = 244,
    CROSSBOW_LOADING_START = 245,
    CROSSBOW_LOADING_MIDDLE = 246,
    CROSSBOW_LOADING_END = 247,
    CROSSBOW_SHOOT = 248,
    CROSSBOW_QUICK_CHARGE_START = 249,
    CROSSBOW_QUICK_CHARGE_MIDDLE = 250,
    CROSSBOW_QUICK_CHARGE_END = 251,
    AMBIENT_AGGRESSIVE = 252,
    AMBIENT_WORRIED = 253,
    CANT_BREED = 254,
    ITEM_SHIELD_BLOCK = 255,
    ITEM_BOOK_PUT = 256,
    BLOCK_GRINDSTONE_USE = 257,
    BLOCK_BELL_HIT = 258,
    BLOCK_CAMPFIRE_CRACKLE = 259,
    ROAR = 260,
    STUN = 261,
    BLOCK_SWEET_BERRY_BUSH_HURT = 262,
    BLOCK_SWEET_BERRY_BUSH_PICK = 263,
    BLOCK_CARTOGRAPHY_TABLE_USE = 264,
    BLOCK_STONECUTTER_USE = 265,
    BLOCK_COMPOSTER_EMPTY = 266,
    BLOCK_COMPOSTER_FILL = 267,
    BLOCK_COMPOSTER_FILL_SUCCESS = 268,
    BLOCK_COMPOSTER_READY = 269,
    BLOCK_BARREL_OPEN = 270,
    BLOCK_BARREL_CLOSE = 271,
    RAID_HORN = 272,
    BLOCK_LOOM_USE = 273,
    AMBIENT_IN_RAID = 274,
    UI_CARTOGRAPHY_TABLE_TAKE_RESULT = 275,
    UI_STONECUTTER_TAKE_RESULT = 276,
    UI_LOOM_TAKE_RESULT = 277,
    BLOCK_SMOKER_SMOKE = 278,
    BLOCK_BLASTFURNACE_FIRE_CRACKLE = 279,
    BLOCK_SMITHING_TABLE_USE = 280,
    SCREECH = 281,
    SLEEP = 282,
    BLOCK_FURNACE_LIT = 283,
    CONVERT_MOOSHROOM = 284,
    MILK_SUSPICIOUSLY = 285,
    CELEBRATE = 286,
    JUMP_PREVENT = 287,
    AMBIENT_POLLINATE = 288,
    BLOCK_BEEHIVE_DRIP = 289,
    BLOCK_BEEHIVE_ENTER = 290,
    BLOCK_BEEHIVE_EXIT = 291,
    BLOCK_BEEHIVE_WORK = 292,
    BLOCK_BEEHIVE_SHEAR = 293,
    DRINK_HONEY = 294,
    AMBIENT_CAVE = 295,
    RETREAT = 296,
    CONVERTED_TO_ZOMBIFIED = 297,
    ADMIRE = 298,
    STEP_LAVA = 299,
    TEMPT = 300,
    PANIC = 301,
    ANGRY = 302,
    AMBIENT_WARPED_FOREST_MOOD = 303,
    AMBIENT_SOULSAND_VALLEY_MOOD = 304,
    AMBIENT_NETHER_WASTES_MOOD = 305,
    RESPAWN_ANCHOR_BASALT_DELTAS_MOOD = 306,
    AMBIENT_CRIMSON_FOREST_MOOD = 307,
    RESPAWN_ANCHOR_CHARGE = 308,
    RESPAWN_ANCHOR_DEPLETE = 309,
    RESPAWN_ANCHOR_SET_SPAWN = 310,
    RESPAWN_ANCHOR_AMBIENT = 311,
    PARTICLE_SOUL_ESCAPE_QUIET = 312,
    PARTICLE_SOUL_ESCAPE_LOUD = 313,
    RECORD_PIGSTEP = 314,
    LODESTONE_COMPASS_LINK_COMPASS_TO_LODESTONE = 315,
    SMITHING_TABLE_USE = 316,
    ARMOR_EQUIP_NETHERITE = 317,
    AMBIENT_WARPED_FOREST_LOOP = 318,
    AMBIENT_SOULSAND_VALLEY_LOOP = 319,
    AMBIENT_NETHER_WASTES_LOOP = 320,
    AMBIENT_BASALT_DELTAS_LOOP = 321,
    AMBIENT_CRIMSON_FOREST_LOOP = 322,
    AMBIENT_WARPED_FOREST_ADDITIONS = 323,
    AMBIENT_SOULSAND_VALLEY_ADDITIONS = 324,
    AMBIENT_NETHER_WASTES_ADDITIONS = 325,
    AMBIENT_BASALT_DELTAS_ADDITIONS = 326,
    AMBIENT_CRIMSON_FOREST_ADDITIONS = 327,
    SCULK_SENSOR_POWER_ON = 328,
    SCULK_SENSOR_POWER_OFF = 329,
    BUCKET_FILL_POWDER_SNOW = 330,
    BUCKET_EMPTY_POWDER_SNOW = 331,
    POINTED_DRIPSTONE_CAULDRON_DRIP_WATER = 332,
    POINTED_DRIPSTONE_CAULDRON_DRIP_LAVA = 333,
    POINTED_DRIPSTONE_DRIP_WATER = 334,
    POINTED_DRIPSTONE_DRIP_LAVA = 335,
    CAVE_VINES_PICK_BERRIES = 336,
    BIG_DRIPLEAF_TILT_DOWN = 337,
    BIG_DRIPLEAF_TILT_UP = 338,
    COPPER_WAX_ON = 339,
    COPPER_WAX_OFF = 340,
    SCRAPE = 341,
    PLAYER_HURT_DROWN = 342,
    PLAYER_HURT_ON_FIRE = 343,
    PLAYER_HURT_FREEZE = 344,
    USE_SPYGLASS = 345,
    STOP_USING_SPYGLASS = 346,
    AMETHYST_BLOCK_CHIME = 347,
    AMBIENT_SCREAMER = 348,
    HURT_SCREAMER = 349,
    DEATH_SCREAMER = 350,
    MILK_SCREAMER = 351,
    JUMP_TO_BLOCK = 352,
    PRE_RAM = 353,
    PRE_RAM_SCREAMER = 354,
    RAM_IMPACT = 355,
    RAM_IMPACT_SCREAMER = 356,
    SQUID_INK_SQUIRT = 357,
    GLOW_SQUID_INK_SQUIRT = 358,
    CONVERT_TO_STRAY = 359,
    CAKE_ADD_CANDLE = 360,
    EXTINGUISH_CANDLE = 361,
    AMBIENT_CANDLE = 362,
    BLOCK_CLICK = 363,
    BLOCK_CLICK_FAIL = 364,
    SCULK_CATALYST_BLOOM = 365,
    SCULK_SHRIEKER_SHRIEK = 366,
    WARDEN_NEARBY_CLOSE = 367,
    WARDEN_NEARBY_CLOSER = 368,
    WARDEN_NEARBY_CLOSEST = 369,
    WARDEN_SLIGHTLY_ANGRY = 370,
    RECORD_OTHERSIDE = 371,
    TONGUE = 372,
    IRONGOLEM_CRACK = 373,
    IRONGOLEM_REPAIR = 374,
    LISTENING = 375,
    HEARTBEAT = 376,
    HORNBREAK = 377,
    SCULK_SPREAD = 379,
    SCULK_CHARGE = 380,
    SCULK_SENSOR_PLACE = 381,
    SCULK_SHRIEKER_PLACE = 382,
    GOAT_CALL_0 = 383,
    GOAT_CALL_1 = 384,
    GOAT_CALL_2 = 385,
    GOAT_CALL_3 = 386,
    GOAT_CALL_4 = 387,
    GOAT_CALL_5 = 388,
    GOAT_CALL_6 = 389,
    GOAT_CALL_7 = 390,
    GOAT_CALL_8 = 391,
    GOAT_CALL_9 = 392,
    GOAT_HARMONY_0 = 393,
    GOAT_HARMONY_1 = 394,
    GOAT_HARMONY_2 = 395,
    GOAT_HARMONY_3 = 396,
    GOAT_HARMONY_4 = 397,
    GOAT_HARMONY_5 = 398,
    GOAT_HARMONY_6 = 399,
    GOAT_HARMONY_7 = 400,
    GOAT_HARMONY_8 = 401,
    GOAT_HARMONY_9 = 402,
    GOAT_MELODY_0 = 403,
    GOAT_MELODY_1 = 404,
    GOAT_MELODY_2 = 405,
    GOAT_MELODY_3 = 406,
    GOAT_MELODY_4 = 407,
    GOAT_MELODY_5 = 408,
    GOAT_MELODY_6 = 409,
    GOAT_MELODY_7 = 410,
    GOAT_MELODY_8 = 411,
    GOAT_MELODY_9 = 412,
    GOAT_BASS_0 = 413,
    GOAT_BASS_1 = 414,
    GOAT_BASS_2 = 415,
    GOAT_BASS_3 = 416,
    GOAT_BASS_4 = 417,
    GOAT_BASS_5 = 418,
    GOAT_BASS_6 = 419,
    GOAT_BASS_7 = 420,
    GOAT_BASS_8 = 421,
    GOAT_BASS_9 = 422,
    IMITATE_WARDEN = 426,
    LISTENING_ANGRY = 427,
    ITEM_GIVEN = 428,
    ITEM_TAKEN = 429,
    DISAPPEARED = 430,
    REAPPEARED = 431,
    FROG_SPAWN_HATCHED = 433,
    LAY_SPAWN = 434,
    FROG_SPAWN_BREAK = 435,
    SONIC_BOOM = 436,
    SONIC_CHARGE = 437,
    ITEM_THROWN = 438,
    RECORD_5 = 439,
    CONVERT_TO_FROG = 440,
    UNDEFINED = 441,
};

enum class PacketPriority {
    High,
    Immediate,
    Low,
    Medium,
    Count
};

enum class Reliability {
    Reliable,
    ReliableOrdered,
    Unreliable,
    UnreliableSequenced
};

enum class Compressibility {
    Compressible,
    Incompressible
};

class RakPeerInterface
{
public:
    BUILD_ACCESS(this, uintptr_t, vTable, 0x0);

    virtual ~RakPeerInterface() = 0;
    virtual void Startup(unsigned int, class SocketDescriptor *,unsigned int,int) = 0;
    virtual void InitializeSecurity(char const*,char const*,bool) = 0;
    virtual void DisableSecurity(void) = 0;
    virtual void AddToSecurityExceptionList(char const*) = 0;
    virtual void RemoveFromSecurityExceptionList(char const*) = 0;
    virtual void IsInSecurityExceptionList(char const*) = 0;
    virtual void SetMaximumIncomingConnections(unsigned short) = 0;
    virtual void GetMaximumIncomingConnections(void) = 0;
    virtual void NumberOfConnections(void) = 0;
    virtual void SetIncomingPassword(char const*,int) = 0;
    virtual void GetIncomingPassword(char *,int *) = 0;
    virtual void Connect(char const*,unsigned short,char const*,int, class PublicKey *,unsigned int,unsigned int,unsigned int,unsigned int) = 0;
    virtual void ConnectWithSocket(char const*,unsigned short,char const*,int,class RakNetSocket2 *,class PublicKey *,unsigned int,unsigned int,unsigned int) = 0;
    virtual void Shutdown(unsigned int,unsigned short, PacketPriority) = 0;
    virtual void IsActive(void) = 0;
    virtual void GetConnectionList(class SystemAddress *,unsigned short *) = 0;
    virtual void GetNextSendReceipt(void) = 0;
    virtual void IncrementNextSendReceipt(void) = 0;
    virtual void Send(char const* data,int,PacketPriority prio,class PacketReliability reliability,char orderingChannel,class AddressOrGUID systemIdentifier,bool broadcast,unsigned int forceReceiptNumber = 0) = 0;
    virtual void SendLoopback(char const*,int) = 0;
    virtual void Send2(class BitStream const*,PacketPriority,PacketReliability,char,AddressOrGUID,bool,unsigned int) = 0;
    virtual void SendList(char const**,int const*,int,PacketPriority,PacketReliability,char,AddressOrGUID,bool,unsigned int) = 0;
    virtual void Receive(void) = 0;
    virtual void DeallocatePacket(class Packet *) = 0;
    virtual void GetMaximumNumberOfPeers(void) = 0;
    virtual void CloseConnection(AddressOrGUID,bool,unsigned char,PacketPriority) = 0;
    virtual void GetConnectionState(AddressOrGUID) = 0;
    virtual void CancelConnectionAttempt(SystemAddress) = 0;
    virtual void GetIndexFromSystemAddress(SystemAddress) = 0;
    virtual void GetSystemAddressFromIndex(unsigned int) = 0;
    virtual void GetGUIDFromIndex(unsigned int) = 0;
    virtual void GetSystemList(/*DataStructures::List<SystemAddress> &,DataStructures::List<RakNetGUID> &*/) = 0;
    virtual void AddToBanList(char const*,unsigned int) = 0;
    virtual void RemoveFromBanList(char const*) = 0;
    virtual void ClearBanList(void) = 0;
    virtual void IsBanned(char const*) = 0;
    virtual void SetLimitIPConnectionFrequency(bool) = 0;
    virtual void Ping(SystemAddress) = 0;
    virtual void Ping2(char const*,unsigned short,bool,unsigned int) = 0;
    virtual void SendNatTraversalMessage(SystemAddress) = 0;
    virtual void GetAveragePing(AddressOrGUID) = 0;
    virtual void GetLastPing(AddressOrGUID) = 0;
    virtual void GetLowestPing(AddressOrGUID) = 0;
    virtual void SetOccasionalPing(bool) = 0;
    virtual void GetClockDifferential(AddressOrGUID) = 0;
    virtual void SetOfflinePingResponse(char const*,unsigned int) = 0;
    virtual void GetOfflinePingResponse(char **,unsigned int *) = 0;
    virtual void GetInternalID(SystemAddress,int) = 0;
    virtual void SetInternalID(SystemAddress,int) = 0;
    virtual void GetExternalID(SystemAddress) = 0;
    virtual void GetMyGUID(void) = 0;
    virtual void resetMyGUID(void) = 0;
    virtual void GetMyBoundAddress(int) = 0;
    virtual void GetGuidFromSystemAddress(SystemAddress) = 0;
    virtual void GetSystemAddressFromGuid(class RakNetGUID) = 0;
    virtual void GetClientPublicKeyFromSystemAddress(SystemAddress,char *) = 0;
    virtual void SetTimeoutTime(unsigned int,SystemAddress) = 0;
    virtual void GetTimeoutTime(SystemAddress) = 0;
    virtual void GetMTUSize(SystemAddress) = 0;
    virtual void GetNumberOfAdapters(void) = 0;
    virtual void GetLocalAdapter(unsigned int) = 0;
    virtual void GetNumberOfAddresses(void) = 0;
    virtual void GetLocalIP(unsigned int) = 0;
    virtual void IsLocalIP(char const*) = 0;
    virtual void AllowConnectionResponseIPMigration(bool) = 0;
    virtual void AdvertiseSystem(char const*,unsigned short,char const*,int,unsigned int) = 0;
    virtual void SetSplitMessageProgressInterval(int) = 0;
    virtual void GetSplitMessageProgressInterval(void) = 0;
    virtual void SetUnreliableTimeout(unsigned int) = 0;
    virtual void SendTTL(char const*,unsigned short,int,unsigned int) = 0;
    virtual void AttachPlugin(class PluginInterface2 *) = 0;
    virtual void DetachPlugin(class PluginInterface2 *) = 0;
    virtual void PushBackPacket(Packet *,bool) = 0;
    virtual void ChangeSystemAddress(class RakNetGUID,SystemAddress const&) = 0;
    virtual void AllocatePacket(unsigned int) = 0;
    virtual void GetSocket(SystemAddress) = 0;
    virtual void GetSockets(/*DataStructures::List<RakNetSocket2 *> &*/) = 0;
    virtual void ReleaseSockets(/*DataStructures::List<RakNetSocket2 *> &*/) = 0;
    virtual void WriteOutOfBandHeader(class BitStream *) = 0;
    virtual void SetUserUpdateThread(void (*)(RakPeerInterface *,void *),void *) = 0;
    virtual void SetIncomingDatagramEventHandler(bool (*)(class RNS2RecvStruct *)) = 0;
    virtual void ApplyNetworkSimulator(float,unsigned short,unsigned short) = 0;
    virtual void SetPerConnectionOutgoingBandwidthLimit(unsigned int) = 0;
    virtual void IsNetworkSimulatorActive(void) = 0;
    virtual void GetStatistics(SystemAddress,class RakNetStatistics *) = 0;
    virtual void GetStatistics(unsigned int,class RakNetStatistics *) = 0;
    virtual void GetStatisticsList(/*DataStructures::List<SystemAddress> &,DataStructures::List<RakNetGUID> &,DataStructures::List<RakNetStatistics> &*/) = 0;
    virtual void GetReceiveBufferSize(void) = 0;
    virtual void RunUpdateCycle(class BitStream &) = 0;
    virtual void SendOutOfBand(char const*,unsigned short,char const*,unsigned int,unsigned int) = 0;
    virtual void DeallocRNS2RecvStruct(class RNS2RecvStruct *,char const*,unsigned int) = 0;
    virtual void AllocRNS2RecvStruct(char const*,unsigned int) = 0;
    virtual void OnRNS2Recv(class RNS2RecvStruct *) = 0;

    // Get the adddress of RunUpdateCycle based on the vTable

    uint64_t GetVirtualFuncAddress(int vIndex)
    {
        return Memory::GetAddressByIndex(this->vTable, vIndex);
    }
};

    struct SocialGameConnectionInfo
    {
        BUILD_ACCESS_STRING(this, realIp, 0x8);
        BUILD_ACCESS_STRING(this, serverIp, 0x28);
    };

// TODO: Update this lol
class RakNetConnector
{
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);

    virtual ~RakNetConnector();
    virtual void getLocalIps(void);
    virtual void getLocalIp(void);
    virtual void getPort(void);
    virtual void getRefinedLocalIps(void);
    virtual void getConnectedGameInfo(void);
    virtual void setupNatPunch(bool);
    virtual void getNatPunchInfo(void);
    virtual void startNatPunchingClient(std::string const&, unsigned short);
    virtual void addConnectionStateListener(/*Connector::ConnectionStateListener**/);
    virtual void removeConnectionStateListener(/*Connector::ConnectionStateListener**/);
    virtual bool isIPv4Supported(void);
    virtual bool isIPv6Supported(void);
    virtual void getIPv4Port(void);
    virtual void getIPv6Port(void);
    virtual void getNetworkType(void);
    virtual void host(class ConnectionDefinition const&);
    virtual void connect(/*class Social::GameConnectionInfo const&, Social::GameConnectionInfo const&*/);
    virtual void disconnect(void);
    virtual void tick(void);
    virtual void runEvents(void);
    virtual bool isServer(void);
    virtual bool isConnected(/*NetworkIdentifier const&*/);
    virtual void closeNetworkConnection(/*NetworkIdentifier const&*/);
    virtual void* getNetworkIdentifier(void);
    virtual void setApplicationHandshakeCompleted(/*NetworkIdentifier const&*/);
    virtual RakPeerInterface* getPeer(void);
    virtual void getPeer2(void);
    virtual void _onDisable(void);
};


class RemoteConnectorComposite
{
public:
    BUILD_ACCESS(this, RakNetConnector*, raknet, 0x60);
};
class NetworkSystem
{
public:

    BUILD_ACCESS(this, RemoteConnectorComposite*, remoteConnector, 0x60);

    virtual ~NetworkSystem();
    virtual SocialGameConnectionInfo* getConnectionInfo(void);
    virtual void useIPv4Only(void);
    virtual void useIPv6Only(void);
    virtual unsigned __int16 getDefaultGamePort(void);
    virtual unsigned __int16 getDefaultGamePortv6(void);
    virtual void _onDisable(void);
    virtual void _onEnable(void);
    virtual void onNewIncomingConnection(class NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>);
    virtual void onNewOutgoingConnection(class NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>);
    virtual void onConnectionClosed(NetworkIdentifier const&, const std::string&, bool);
    virtual void onAllConnectionsClosed(const std::string&, bool);
    virtual void onAllRemoteConnectionsClosed(const std::string&, bool);
    virtual void onOutgoingConnectionFailed(void);
    virtual void onWebsocketRequest(const std::string&, const std::string&);
    virtual void onNewIncomingLocalConnection(NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>);
    virtual void onNewOutgoingLocalConnection(NetworkIdentifier const&, std::shared_ptr<class NetworkPeer>);
};

class MinecraftPackets {
public:
    static std::shared_ptr<Packet> CreatePacket(int packetId){
        using createPacket = std::shared_ptr<Packet>(__cdecl *)(int);
        static createPacket createPacketFunc = 0x0;
        if(!createPacketFunc) createPacketFunc = (createPacket)Memory::ResolveRef(
                    SigManager::GetSignatureAddress(SignatureEnum::MinecraftPackets_createPacket));
        return createPacketFunc(packetId);
    }

    static std::shared_ptr<Packet> CreatePacket(PacketID packetId){
        return CreatePacket((int)packetId);
    }

private:
    template<typename T>
    static std::shared_ptr<T> CreatePacket(int packetId){
        return std::reinterpret_pointer_cast<T>(CreatePacket(packetId));
    }

    template<typename T>
    static std::shared_ptr<T> CreatePacket(PacketID packetId){
        return std::reinterpret_pointer_cast<T>(CreatePacket(packetId));
    }

public:

    template<typename T>
    static std::shared_ptr<T> CreatePacket(){
        PacketID packetId = T::ID;
        return std::reinterpret_pointer_cast<T>(CreatePacket(packetId));
    }



};

class LoopbackPacketSender
{
public:
    BUILD_ACCESS(this, uintptr_t**, vTable, 0x0);
    BUILD_ACCESS(this, NetworkSystem*, networkSystem, 0x20);



    virtual ~LoopbackPacketSender();
    virtual void send(void* Packet);
    virtual void sendToServer(void* Packet);
    virtual void sendToClient(class UserEntityIdentifierComponent const*, void* Packet);
    virtual void sendToClient(class NetworkIdentifier const&, void* Packet, unsigned char);
    virtual void sendToClients(std::vector<class NetworkIdentifierWithSubId, std::allocator<class NetworkIdentifierWithSubId>> const&, void* Packet);
    virtual void sendBroadcast(void* Packet);
    virtual void sendBroadcast(class NetworkIdentifier const&, unsigned char, void* Packet);
    virtual void flush(class NetworkIdentifier const&);
};

class PacketHandlerDispatcher {
public:
    virtual ~PacketHandlerDispatcher();
    virtual void handle(void* networkIdentifier, void* netEventCallback, std::shared_ptr<Packet>);

    uintptr_t getHandleFunc(){
        // Get the handle function at index 1 from the packetHandlerDispatcher vtable
        uintptr_t* vtable = *(uintptr_t**)this;
        return vtable[1];
    }

};

class Packet {
public:
    BUILD_ACCESS(this, uintptr_t*, vTable, 0x0);
    PacketPriority packetPriority = PacketPriority::Low; //0x0008
    Reliability peerReliability = Reliability::Reliable; //0x000C
    uint64_t clientSubID; //0x0010
    char pad_0018[8]; //0x0018
    PacketHandlerDispatcher *packetHandlerDispatcher; //0x0020
    Compressibility compressType = Compressibility::Compressible; //0x0028
    char pad_002C[4]; //0x002C

    virtual void packetConstructor(void) {}
    virtual int getId(void) { return 0x0; }
    virtual class TextHolder getTypeName(void) { return TextHolder(); }
    virtual void write(uintptr_t BinaryStream) {}
    virtual void read(uintptr_t BinaryStream) {}
    virtual void readExtended(uintptr_t BinaryStream) {}
    virtual void disallowBatching(void) {}



    std::string getName() {
        return this->getTypeName().getText();
    }

    void setVTable(uint64_t vtable) {
        *(uint64_t *) (reinterpret_cast<uintptr_t>(this) + 0x0) = vtable;
    }

    uint64_t getVTable() {
        return *(uint64_t *) (reinterpret_cast<uintptr_t>(this) + 0x0);
    }

    uintptr_t getHandleFunc(){
        if (!packetHandlerDispatcher) __debugbreak();
        return packetHandlerDispatcher->getHandleFunc();
    }
};