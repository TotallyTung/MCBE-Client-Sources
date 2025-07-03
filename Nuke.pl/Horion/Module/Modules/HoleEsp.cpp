#include "HoleEsp.h"

HoleEsp::HoleEsp() : IModule(0x0, Category::VISUAL, "Shows nearby holes")
{
	registerBoolSetting("Obsidian", &this->obsidian, this->obsidian);
	registerBoolSetting("Bedrock", &this->bedrock, this->bedrock);
	registerBoolSetting("Mixed", &this->mixed, this->mixed);
	registerBoolSetting("Outline", &this->outline, this->outline);
	registerFloatSetting("FillOpacity", &this->Opacity, 0.1f, 0.f, 1.f);
	registerFloatSetting("LineOpacity", &this->lineOpacity, 0.6f, 0.f, 1.f);
	registerFloatSetting("LineSize", &this->LineSize, 1, 0.1f, 2.f);
	registerIntSetting("Range", &this->range, 1, 3.f, 15.f);
	registerBoolSetting("ObsidianSync", &osync, osync);
	registerBoolSetting("BedrockSync", &bsync, bsync);
	registerBoolSetting("MixedSync", &msync, msync);

	registerFloatSetting("ObsidianRed", &obr, obr, 0.f, 255.f);
	registerFloatSetting("ObsidianGreen", &obg, obg, 0.f, 255.f);
	registerFloatSetting("ObsidianBlue", &obb, obb, 0.f, 255.f);

	registerFloatSetting("BedrockRed", &br, br, 0.f, 255.f);
	registerFloatSetting("BedrockGreen", &bg, bg, 0.f, 255.f);
	registerFloatSetting("BedrockBlue", &bb, bb, 0.f, 255.f);

	registerFloatSetting("MixedRed", &mr, mr, 0.f, 255.f);
	registerFloatSetting("MixedGreen", &mg, mg, 0.f, 255.f);
	registerFloatSetting("MixedBlue", &mb, mb, 0.f, 255.f);
}

HoleEsp::~HoleEsp()
{
}

const char* HoleEsp::getModuleName()
{
	return ("HoleESP");
}

void HoleEsp::onPostRender(MinecraftUIRenderContext* renderCtx)
{
	if (!(Game.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys()))
	{
		if (Game.isInGame())
		{
			if (!moduleMgr->getModule<ClickGuiMod>()->isEnabled())
			{
				Vec3* pos = Game.getLocalPlayer()->getPos();
				for (int x = (int)pos->x - range; x < pos->x + range; x++)
				{
					for (int z = (int)pos->z - range; z < pos->z + range; z++)
					{
						for (int y = (int)pos->y - range; y < pos->y + range; y++)
						{
							Vec4 underline;
							Vec3 blockPos = Vec3(x, y, z);
							Vec3 air = Vec3(x, y + 1, z);
							Vec3 air2 = Vec3(x, y + 2, z);

							Vec3 blockPos2 = Vec3(x + 1, y, z);
							Vec3 blockPos3 = Vec3(x, y, z + 1);
							Vec3 blockPos4 = Vec3(x, y - 1, z);
							Vec3 blockPos5 = Vec3(x - 1, y, z);
							Vec3 blockPos6 = Vec3(x, y, z - 1);

							Vec3 blockPos22 = Vec3(x + 1, y + 1, z);
							Vec3 blockPos32 = Vec3(x, y + 1, z + 1);
							Vec3 blockPos52 = Vec3(x - 1, y + 1, z);
							Vec3 blockPos62 = Vec3(x, y + 1, z - 1);

							Block* block = Game.getLocalPlayer()->region->getBlock(blockPos);
							Block* airpos = Game.getLocalPlayer()->region->getBlock(air);
							Block* airpos2 = Game.getLocalPlayer()->region->getBlock(air2);
							Block* block2 = Game.getLocalPlayer()->region->getBlock(blockPos2);
							Block* block3 = Game.getLocalPlayer()->region->getBlock(blockPos3);
							Block* block4 = Game.getLocalPlayer()->region->getBlock(blockPos4);
							Block* block5 = Game.getLocalPlayer()->region->getBlock(blockPos5);
							Block* block6 = Game.getLocalPlayer()->region->getBlock(blockPos6);

							Block* block22 = Game.getLocalPlayer()->region->getBlock(blockPos22);
							Block* block32 = Game.getLocalPlayer()->region->getBlock(blockPos32);
							Block* block52 = Game.getLocalPlayer()->region->getBlock(blockPos52);
							Block* block62 = Game.getLocalPlayer()->region->getBlock(blockPos62);

							if (block != nullptr && block2 != nullptr && block3 != nullptr && block4 != nullptr)
							{
								int blockId = block->toLegacy()->blockId;
								int aircheck = airpos->toLegacy()->blockId;
								int aircheck2 = airpos2->toLegacy()->blockId;
								int blockId2 = block2->toLegacy()->blockId;
								int blockId3 = block3->toLegacy()->blockId;
								int blockId4 = block4->toLegacy()->blockId;
								int blockId5 = block5->toLegacy()->blockId;
								int blockId6 = block6->toLegacy()->blockId;

								int blockId22 = block22->toLegacy()->blockId;
								int blockId32 = block32->toLegacy()->blockId;
								int blockId52 = block52->toLegacy()->blockId;
								int blockId62 = block62->toLegacy()->blockId;

								bool obs = false;

								if (blockId == 0)
									obs = true;

								static Colors* ColorsMod = moduleMgr->getModule<Colors>();
								auto interfaceColor = ColorsMod->getColorV2();
								if (bedrock)
								{
									if ((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0))
									{
										// Vec3 block = blockPos.add(0.5, -0.5, 0.5);
										// float rentimer = 1;
										// float zero = rentimer / 2;
										if (!bsync)
											DrawUtils::setColor(br, bg, bb, Opacity);
										if (bsync)
											DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity);
										{
											Vec2 fillTop1 = DrawUtils::worldToScreen(Vec3(blockPos).add(1.f, -0.01f, 1.f));
											Vec2 fillTop2 = DrawUtils::worldToScreen(Vec3(blockPos).add(1.f, -0.01f, 0.f));
											Vec2 fillTop3 = DrawUtils::worldToScreen(Vec3(blockPos).add(0.f, -0.01f, 0.f));
											Vec2 fillTop4 = DrawUtils::worldToScreen(Vec3(blockPos).add(0.f, -0.01f, 1.f));
											DrawUtils::drawQuad(Vec2(fillTop3), Vec2(fillTop4), Vec2(fillTop1), Vec2(fillTop2));
										}
										if (outline)
										{
											if (!bsync)
												DrawUtils::setColor(br, bg, bb, lineOpacity);
											if (bsync)
												DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, lineOpacity);
											DrawUtils::drawBox(blockPos, Vec3(blockPos).add(1.f, -0.01f, 1.f), (float)LineSize, false);
										}
									}
								}
								if (obsidian)
								{
									if ((blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId4 == 49 || blockId4 == 7) && (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)))
									{
										// Vec3 block = blockPos.add(0.5, -0.5, 0.5);
										// float rentimer = 1;
										// float zero = rentimer / 2;
										if (!osync)
											DrawUtils::setColor(obr, obg, obb, Opacity);
										if (osync)
											DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity);
										{
											Vec2 fillTop1 = DrawUtils::worldToScreen(Vec3(blockPos).add(1.f, -0.01f, 1.f));
											Vec2 fillTop2 = DrawUtils::worldToScreen(Vec3(blockPos).add(1.f, -0.01f, 0.f));
											Vec2 fillTop3 = DrawUtils::worldToScreen(Vec3(blockPos).add(0.f, -0.01f, 0.f));
											Vec2 fillTop4 = DrawUtils::worldToScreen(Vec3(blockPos).add(0.f, -0.01f, 1.f));
											DrawUtils::drawQuad(Vec2(fillTop3), Vec2(fillTop4), Vec2(fillTop1), Vec2(fillTop2));
										}
										if (outline)
										{
											if (!osync)
												DrawUtils::setColor(obr, obg, obb, lineOpacity);
											if (osync)
												DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, lineOpacity);
											DrawUtils::drawBox(blockPos, Vec3(blockPos).add(1.f, -0.01f, 1.f), (float)LineSize, false);
										}
									}
								}
								if (mixed)
								{
									if ((blockId2 == 7 || blockId3 == 7 || blockId5 == 7 || blockId6 == 7) && (blockId == 0) && (blockId2 == 49 || blockId3 == 49 || blockId5 == 49 || blockId6 == 49))
									{

										auto vec1 = Vec3(1.f, 0.1f, 1.f);
										if ((blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId4 == 49) && (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0))
										{
											if (Game.getLocalPlayer()->region->getBlock(blockPos.add(0, 1, 0))->blockLegacy->blockId != 0)
												continue;

											if (Game.getLocalPlayer()->region->getBlock(blockPos.add(0, 2, 0))->blockLegacy->blockId)
												continue;

											if (!msync)
												DrawUtils::setColor(mr, mg, mb, Opacity);
											if (msync)
												DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity);
											{
												Vec2 fillTop1 = DrawUtils::worldToScreen(Vec3(blockPos).add(1.f, -0.01f, 1.f));
												Vec2 fillTop2 = DrawUtils::worldToScreen(Vec3(blockPos).add(1.f, -0.01f, 0.f));
												Vec2 fillTop3 = DrawUtils::worldToScreen(Vec3(blockPos).add(0.f, -0.01f, 0.f));
												Vec2 fillTop4 = DrawUtils::worldToScreen(Vec3(blockPos).add(0.f, -0.01f, 1.f));
												DrawUtils::drawQuad(Vec2(fillTop3), Vec2(fillTop4), Vec2(fillTop1), Vec2(fillTop2));
											}
											if (outline)
											{
												if (!msync)
													DrawUtils::setColor(mr, mg, mb, lineOpacity);
												if (msync)
													DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, lineOpacity);
												DrawUtils::drawBox(blockPos, Vec3(blockPos).add(1.f, -0.01f, 1.f), (float)LineSize, false);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}