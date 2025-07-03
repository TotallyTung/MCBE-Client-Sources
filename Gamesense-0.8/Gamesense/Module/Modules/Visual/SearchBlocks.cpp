#include "SearchBlocks.h"

// Too lazy to recompile
bool shouldOutline;
bool grass; // oh boy
bool air; // oh boy 2
bool portal;
SearchBlocks::SearchBlocks() : IModule(0, Category::VISUAL, "Search for specific blocks")
{
	registerIntSetting("Radius", &range, range, 5, 100);
	registerBoolSetting("Vertex", &outline, outline, "Should do vertex type rendering");
	registerBoolSetting("Outline", &shouldOutline, shouldOutline, "Should render outline");
	registerBoolSetting("Fill", &shouldFill, shouldFill, "Should render fill");
	registerFloatSetting("Thickness", &thick, thick, 0.1f, 0.8f);
	registerFloatSetting("ShutterSpeed", &delay, delay, 0.f, 1.f, 0.1f, "Shuttering speed of the esp, 0 for disabled");
	registerFloatSetting("Opacity", &opacity, opacity, 0.1f, 1.f);
	registerBoolSetting("Chest", &chest, chest);
	registerBoolSetting("TrapChest", &trapped_chest, trapped_chest);
	registerBoolSetting("EnderChest", &ender_chest, ender_chest);
	registerBoolSetting("ShulkerBox", &shulker_box, shulker_box);
	registerBoolSetting("Dropper", &dropper, dropper);
	registerBoolSetting("Dispenser", &dispenser, dispenser);
	registerBoolSetting("Hopper", &hopper, hopper);
	registerBoolSetting("Barrel", &barrel, barrel);
	registerBoolSetting("Furnace", &furnace, furnace);
	registerBoolSetting("Obsidian", &obsidian, obsidian);
	registerBoolSetting("Portal", &portal, portal);
	registerBoolSetting("Grass", &grass, grass, "Ohh boy");
	registerBoolSetting("Air", &air, air, "NO DO NOT DO THIS NO");
}

SearchBlocks::~SearchBlocks()
{
}

const char *SearchBlocks::getRawModuleName()
{
	return ("SearchBlock");
}

void SearchBlocks::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
	if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys()))
	{
		if (g_Data.isInGame())
		{
			Odelay++;
			if (Odelay > delay * 20)
			{
				vec3_t *pos = g_Data.getLocalPlayer()->getPos();
				for (int x = (int)pos->x - range; x < pos->x + range; x++)
				{
					for (int z = (int)pos->z - range; z < pos->z + range; z++)
					{
						for (int y = (int)pos->y - range; y < pos->y + range; y++)
						{
							vec3_t blockPos = vec3_t(x, y, z);
							auto fillPos = blockPos.add(0.5f, 0.5f, 0.5f);

							C_Block *block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
							if (block != nullptr)
							{
								int blockId = block->toLegacy()->blockId;
								bool render0 = false;
								bool render1 = false;
								bool render2 = false;
								bool render3 = false;
								bool render4 = false;
								bool render5 = false;
								bool render6 = false;
								bool render7 = false;
								bool render8 = false;
								bool render9 = false;
								bool renderAir = false;
								bool renderGrass = false;
								bool renderPortal = false;

								if (chest)
								{
									if (blockId == 54)
										render0 = true;
								}
								if (trapped_chest)
								{
									if (blockId == 146)
										render1 = true;
								}
								if (ender_chest)
								{
									if (blockId == 130)
										render2 = true;
								}
								if (shulker_box)
								{
									if (blockId == 205 || blockId == 218)
										render3 = true;
								}
								if (barrel)
								{
									if (blockId == 458)
										render4 = true;
								}
								if (dropper)
								{
									if (blockId == 125)
										render5 = true;
								}
								if (dispenser)
								{
									if (blockId == 23)
										render6 = true;
								}
								if (hopper)
								{
									if (blockId == 154)
										render7 = true;
								}
								if (furnace)
								{
									if (blockId == 61 || blockId == 451 || blockId == 453)
										render8 = true;
								}
								if (obsidian)
								{
									if (blockId == 49)
										render9 = true;
								}
								if (portal)
								{
									if (blockId == 90)
										renderPortal = true;
								}
								if (grass)
								{
									if (blockId == 2)
										renderGrass = true;
								}
								if (air)
								{
									if (blockId == 0)
										renderAir = true;
								}
								// chest
								if (render0)
								{
									DrawUtils::setColor(1.f, 0.65f, 0.f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// trapped_chest
								if (render1)
								{
									DrawUtils::setColor(1.f, 0.40f, 0.f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// ender_chest
								if (render2)
								{
									DrawUtils::setColor(1.f, 0.f, 1.f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// shulker_box
								if (render3)
								{
									DrawUtils::setColor(1.f, 1.f, 1.f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// barrel
								if (render4)
								{
									DrawUtils::setColor(0.58f, 0.29f, 0.f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// dropper
								if (render5)
								{
									DrawUtils::setColor(0.5f, 0.5f, 0.5f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// dispenser
								if (render6)
								{
									DrawUtils::setColor(0.5f, 0.5f, 0.5f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// hopper
								if (render7)
								{
									DrawUtils::setColor(0.5f, 0.5f, 1.5f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// furnace
								if (render8)
								{
									DrawUtils::setColor(0.5f, 0.5f, 0.5f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// obsidian
								if (render9)
								{
									DrawUtils::setColor(0.5f, 0.f, 0.5f, opacity);
									if (shouldOutline)
									DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// portal
								if (renderPortal)
								{
									DrawUtils::setColor(0.5f, 0.f, 0.5f, opacity);
									if (shouldOutline)
										DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// :skull:
								if (renderAir)
								{
									DrawUtils::setColor(1.f, 1.f, 1.f, opacity);
									if (shouldOutline)
										DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
								// :skull:
								if (renderGrass)
								{
									DrawUtils::setColor(0.f, 1.f, 0.f, opacity);
									if (shouldOutline)
										DrawUtils::drawBox(blockPos, vec3_t(blockPos).add(1), (float)thick / (float)1.f, outline);
									if (shouldFill)
										DrawUtils::drawFilledBox(fillPos);
								}
							}
						}
					}
					Odelay = 0;
				}
			}
		}
	}
}