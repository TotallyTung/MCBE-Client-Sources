#include "BlockOutline.h"

BlockOutline::BlockOutline() : IModule(0, Category::VISUAL, "Highlights the block your looking at")
{
	registerBoolSetting("Sync", &sync, sync);
	registerBoolSetting("Fill", &fill, fill);
	registerEnumSetting("Mode", &boxtype, 0);
	boxtype.addEntry("3D", 0);
	boxtype.addEntry("Hologram", 1);
	//boxtype.addEntry("2D", 2); Works but kinda bad
	registerFloatSetting("Red", &red, red, 0, 1);
	registerFloatSetting("Green", &green, green, 0, 1);
	registerFloatSetting("Blue", &blue, blue, 0, 1);
	registerFloatSetting("Width", &width, width, 0.3, 1);
	registerFloatSetting("FillOpacity", &this->Opacity, 0.1f, 0.f, 1.f);
	registerFloatSetting("OutlineOpacity", &this->OutOpacity, 0.1f, 0.f, 1.f);
}

const char *BlockOutline::getRawModuleName()
{
	return ("BlockHighlight");
}

void BlockOutline::onPreRender(C_MinecraftUIRenderContext *renderCtx)
{
	auto interfacrColor = ColorUtil::interfaceColor(1);
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	PointingStruct *pointing = g_Data.getLocalPlayer()->pointingStruct;
	if (g_Data.canUseMoveKeys())
	{
		vec3_t outline = pointing->block.toVector3();
		vec3_t block = pointing->block.toVector3();
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
			DrawUtils::drawBox(block.sub(0, 0, 0), block.add(1, 1, 1), .4f, false, true);
		}
		if (boxtype.getSelectedValue() == 1)
		{
			if (block != vec3_t{0, 0, 0})
				DrawUtils::drawBox(block, outline, width, false, 2);
		}
	}
	if (fill)
	{
		auto player = g_Data.getLocalPlayer();
		if (player == nullptr)
			return;
		auto interfacrColor = ColorUtil::interfaceColor(1);
		PointingStruct *pointing = g_Data.getLocalPlayer()->pointingStruct;
		if (g_Data.canUseMoveKeys())
		{
			vec3_t outline = pointing->block.toVector3();
			vec3_t block = pointing->block.toVector3().add(0.5f, 0.5f, 0.5f);
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
				if (boxtype.getSelectedValue() == 0 || boxtype.getSelectedValue() == 1)	
				    DrawUtils::drawFilledBox(block);
				if (boxtype.getSelectedValue() == 2)
					DrawUtils::drawFilledBox2D(block);
			}
		}
	}
}