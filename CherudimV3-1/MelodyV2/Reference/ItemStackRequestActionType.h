
enum ItemStackRequestActionType : unsigned char {
  Take = 0x0000,
  Place = 0x0001,
  Swap = 0x0002,
  Drop = 0x0003,
  Destroy = 0x0004,
  Consume = 0x0005,
  Create = 0x0006,
  LabTableCombine = 0x0007,
  BeaconPayment = 0x0008,
  MineBlock = 0x0009,
  CraftRecipe = 0x000a,
  CraftRecipeAuto = 0x000b,
  CraftCreative = 0x000c,
  CraftRecipeOptional = 0x000d,
  CraftNonImplemented_DEPRECATEDASKTYLAING = 0x000e,
  CraftResults_DEPRECATEDASKTYLAING = 0x000f,
};
