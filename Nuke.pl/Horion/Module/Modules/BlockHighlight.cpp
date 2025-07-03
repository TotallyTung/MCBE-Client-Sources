#include "BlockHighlight.h"

BlockOutline::BlockOutline() : IModule(0, Category::VISUAL, "Highlights the block your looking at")
{
	registerBoolSetting("Sync", &sync, sync);
	registerBoolSetting("Fill", &fill, fill);
	registerEnumSetting("Mode", &boxtype, 0);
	boxtype.addEntry("New", 0);
	boxtype.addEntry("Old", 1);
	// TODO: Add 2d render like chronos but im wayy too lazy, use same logic as AutoCrystalWTA renders
	// boxtype.addEntry("2D", 2);
	registerFloatSetting("Red", &red, red, 0, 1);
	registerFloatSetting("Green", &green, green, 0, 1);
	registerFloatSetting("Blue", &blue, blue, 0, 1);
	registerFloatSetting("Width", &width, width, 0.3, 1);
	registerFloatSetting("FillOpacity", &this->Opacity, 0.1f, 0.f, 1.f);
	registerFloatSetting("OutlineOpacity", &this->OutOpacity, 0.1f, 0.f, 1.f);
}

const char* BlockOutline::getModuleName()
{
	return ("BlockOutline");
}

void BlockOutline::onPreRender(MinecraftUIRenderContext* renderCtx)
{
	auto player = Game.getLocalPlayer();
	if (player == nullptr)
		return;
	MC_Color interfacrColor = moduleMgr->getModule<Colors>()->getColorV2();
	Level* pointing = Game.getLocalPlayer()->level;
	if (Game.canUseMoveKeys())
	{
		Vec3 outline = pointing->block.toVec3t();
		Vec3 block = pointing->block.toVec3t();
		outline = outline.floor();
		block = block.floor();
		outline.x += 1.f;
		outline.y += 1.f;
		outline.z += 1.f;

		if (!sync)
			DrawUtils::setColor(red, green, blue, OutOpacity);
		if (sync)
			DrawUtils::setColor(interfacrColor.r, interfacrColor.g, interfacrColor.b, OutOpacity);

		if (boxtype.getSelectedValue() == 0)
		{
			DrawUtils::drawBox(block.sub(0, 0, 0), block.add(1, 1, 1), .4f, false);
		}
		if (boxtype.getSelectedValue() == 1)
		{
			if (block != Vec3{ 0, 0, 0 })
				DrawUtils::drawBox(block, outline, width, true);
		}
	}
	if (fill)
	{
		auto player = Game.getLocalPlayer();
		if (player == nullptr)
			return;
		Level* pointing = Game.getLocalPlayer()->level;
		if (Game.canUseMoveKeys())
		{
			Vec3 outline = pointing->block.toVec3t();
			Vec3 block = pointing->block.toVec3t().add(0.5f, 0.5f, 0.5f);
			outline = outline.floor();
			block = block.floor().add(0.5f, 0.5f, 0.5f);
			outline.x += 1.f;
			outline.y += 1.f;
			outline.z += 1.f;
			{
				float rentimer = 1;
				float zero = rentimer / 2;
				if (!sync)
					DrawUtils::setColor(red, green, blue, Opacity);
				if (sync)
					DrawUtils::setColor(interfacrColor.r, interfacrColor.g, interfacrColor.b, Opacity);
				{
					Vec2 fill1 = DrawUtils::worldToScreen(Vec3(block).add(zero, zero, zero));
					Vec2 fill2 = DrawUtils::worldToScreen(Vec3(block).add(-zero, zero, zero));
					Vec2 fill3 = DrawUtils::worldToScreen(Vec3(block).add(-zero, zero, -zero));
					Vec2 fill4 = DrawUtils::worldToScreen(Vec3(block).add(zero, zero, -zero));
					DrawUtils::drawQuad(Vec2(fill3), Vec2(fill4), Vec2(fill1), Vec2(fill2));
				}
				{

					Vec2 fill1 = DrawUtils::worldToScreen(Vec3(block).add(-zero, -zero, -zero));
					Vec2 fill2 = DrawUtils::worldToScreen(Vec3(block).add(-zero, -zero, zero));
					Vec2 fill3 = DrawUtils::worldToScreen(Vec3(block).add(zero, -zero, zero));
					Vec2 fill4 = DrawUtils::worldToScreen(Vec3(block).add(zero, -zero, -zero));
					DrawUtils::drawQuad(Vec2(fill3), Vec2(fill4), Vec2(fill1), Vec2(fill2));
				}
				{

					Vec2 fill1 = DrawUtils::worldToScreen(Vec3(block).add(-zero, -zero, zero));
					Vec2 fill2 = DrawUtils::worldToScreen(Vec3(block).add(-zero, zero, zero));
					Vec2 fill3 = DrawUtils::worldToScreen(Vec3(block).add(zero, zero, zero));
					Vec2 fill4 = DrawUtils::worldToScreen(Vec3(block).add(zero, -zero, zero));
					DrawUtils::drawQuad(Vec2(fill3), Vec2(fill4), Vec2(fill1), Vec2(fill2));
				}
				{

					Vec2 fill1 = DrawUtils::worldToScreen(Vec3(block).add(zero, zero, -zero));
					Vec2 fill2 = DrawUtils::worldToScreen(Vec3(block).add(-zero, zero, -zero));
					Vec2 fill3 = DrawUtils::worldToScreen(Vec3(block).add(-zero, -zero, -zero));
					Vec2 fill4 = DrawUtils::worldToScreen(Vec3(block).add(zero, -zero, -zero));
					DrawUtils::drawQuad(Vec2(fill3), Vec2(fill4), Vec2(fill1), Vec2(fill2));
				}
				{

					Vec2 fill1 = DrawUtils::worldToScreen(Vec3(block).add(zero, -zero, zero));
					Vec2 fill2 = DrawUtils::worldToScreen(Vec3(block).add(zero, zero, zero));
					Vec2 fill3 = DrawUtils::worldToScreen(Vec3(block).add(zero, zero, -zero));
					Vec2 fill4 = DrawUtils::worldToScreen(Vec3(block).add(zero, -zero, -zero));
					DrawUtils::drawQuad(Vec2(fill3), Vec2(fill4), Vec2(fill1), Vec2(fill2));
				}
				{

					Vec2 fill1 = DrawUtils::worldToScreen(Vec3(block).add(-zero, zero, -zero));
					Vec2 fill2 = DrawUtils::worldToScreen(Vec3(block).add(-zero, zero, zero));
					Vec2 fill3 = DrawUtils::worldToScreen(Vec3(block).add(-zero, -zero, zero));
					Vec2 fill4 = DrawUtils::worldToScreen(Vec3(block).add(-zero, -zero, -zero));
					DrawUtils::drawQuad(Vec2(fill3), Vec2(fill4), Vec2(fill1), Vec2(fill2));
				}
			}
		}
	}
}