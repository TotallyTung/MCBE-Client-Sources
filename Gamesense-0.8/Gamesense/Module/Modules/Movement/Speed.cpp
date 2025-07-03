#include "Speed.h"

bool testOnGround = false;
float speedMax = 0.69f; // inf value
float speedMin = 0.59f; // inf value
float jumpSpeed = 0.50f;
int packetsSent = 0;
int enabledTicks = 0;
bool isInLiquid = false;
using namespace std;
Speed::Speed() : IModule(0, Category::MOVEMENT, "Increases your speed")
{
	registerEnumSetting("Mode", &mode, 0, "What type of speed should we use");
	mode.addEntry("Vanilla", 0);
	mode.addEntry("Instant", 1);
	mode.addEntry("Nukkit", 2);
	mode.addEntry("IgnPvP", 3);
	registerFloatSetting("Height", &height, height, 0.f, 0.40f, 0.01f, "How high should you jump, 0 to disable");
	registerFloatSetting("Speed", &speed, speed, 0.1f, 2.f, 0.01f, "How fast should you go");
	registerFloatSetting("Jump Speed", &jumpSpeed, jumpSpeed, 0.1f, 2.f, 0.01f, "How fast should you go when jumping");
	registerIntSetting("Timer", &timer, timer, 20, 35, "How fast should timer be, 20 for disabled");
}

const char *Speed::getRawModuleName()
{
	return "Speed";
}

const char *Speed::getModName()
{
	switch (mode.getSelectedValue()) {
	case 0:
		name = " Vanilla";
		break;
	case 1:
		name = " Instant";
		break;
	case 2:
		name = " Nukkit";
		break;
	case 3:
		name = " IgnPvP";
		break;
	}
	return name;
}

void Speed::onEnable()
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	enabledTicks = 0;
}

void Speed::onTick(C_GameMode *gm)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	if (enabledTicks > 10)
		enabledTicks++;
	else
		enabledTicks = 0;

	if (player->onGround)
	{
		groundTicks++;
		offGroundTicks = 0;
	}
	else
	{
		offGroundTicks++;
		groundTicks = 0;
	}
	if (mode.getSelectedValue() == 3 && gm->player->isInWater() && gm->player->isInLava())
		isInLiquid = true;
	else if (mode.getSelectedValue() == 3 && !gm->player->isInWater() && !gm->player->isInLava())
		isInLiquid = false;

	C_GameSettingsInput *input = g_Data.getClientInstance()->getGameSettingsInput();
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
}

void Speed::onMove(C_MoveInputHandler *input)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;

	bool pressed = MoveUtil::keyPressed();
	if (!pressed)
		MoveUtil::stop(false);

	if (pressed && !mode.getSelectedValue() == 1)
	{
		player->setSprinting(true);
	}
	// Nukkit // Instant
	// * Old nukkit bypass, this one stops sprinting altogether
	if (mode.getSelectedValue() == 1)
	{
		C_GameSettingsInput *input = g_Data.getClientInstance()->getGameSettingsInput();
		if (mode.getSelectedValue() == 1 || GameData::isKeyDown(*input->forwardKey))
			g_Data.getCGameMode()->player->setSprinting(false);
	}
	// nukkit bypads ong ong
	// ? in some nukkit servers you cant eat whilst sprinting, havent found an actual bypass so this is a nice workaround
	// * New nukkit bypass, stops sprinting when eating
	if (mode.getSelectedValue() == 2 || mode.getSelectedValue() == 3)
	{
		auto player = g_Data.getLocalPlayer();
		if ((player->getSelectedItemId() == 259 && g_Data.isRightClickDown()) || (player->getSelectedItemId() == 300 && g_Data.isRightClickDown()))
		{
			player->setSprinting(false);
		}
		else
		{
			player->setSprinting(true);
		}
	}

	// Vanilla
	if (mode.getSelectedValue() == 0)
	{
		static bool useVelocity = false;
		// eat my absctrionalie
		if (height >= 0.385)
		{
			if (player->onGround && pressed)
				player->jumpFromGround();
			useVelocity = false;
		}
		else
			useVelocity = true;
		if (height <= 0.04 && !input->isJumping)
		{
			player->velocity.y += height;
			useVelocity = false;
		}

		if (player->onGround && useVelocity && pressed && !input->isJumping)
			player->velocity.y = height;
		C_GameSettingsInput *inputcc = g_Data.getClientInstance()->getGameSettingsInput();
		if (sneakspeed1 && (GameData::isKeyDown(*inputcc->sneakKey) || moduleMgr->getModule<Sneak>()->isEnabled()))
		{
			MoveUtil::setSpeed(sneakspeed2);
		}
		else
		{
			MoveUtil::setSpeed(speed);
		}
	}

	// Nukkit // Instant
	if (mode.getSelectedValue() == 1)
	{
		static bool useVelocity = false;
		if (height >= 0.385)
		{
			if (player->onGround && pressed)
				player->jumpFromGround();
			useVelocity = false;
		}
		else
			useVelocity = true;
		if (height <= 0.04 && !input->isJumping)
		{
			player->velocity.y += height;
			useVelocity = false;
		}

		if (player->onGround && useVelocity && pressed && !input->isJumping)
			player->velocity.y = height;
		C_GameSettingsInput *inputcc = g_Data.getClientInstance()->getGameSettingsInput();
		if (sneakspeed1 && (GameData::isKeyDown(*inputcc->sneakKey) || moduleMgr->getModule<Sneak>()->isEnabled()))
		{
			MoveUtil::setSpeed(sneakspeed2);
		}
		else
		{
			MoveUtil::setSpeed(speed);
		}
	}
	// Nukkit aka infinite gayms network
	if (mode.getSelectedValue() == 2)
	{
		static bool useVelocity = false;
		if (height >= 0.385)
		{
			if (player->onGround && pressed)
				player->jumpFromGround();
			useVelocity = false;
		}
		else
			useVelocity = true;
		if (height <= 0.04 && !input->isJumping)
		{
			player->velocity.y += height;
			useVelocity = false;
		}

		if (player->onGround && useVelocity && pressed && !input->isJumping)
			player->velocity.y = height;
		C_GameSettingsInput *inputcc = g_Data.getClientInstance()->getGameSettingsInput();
		if (sneakspeed1 && (GameData::isKeyDown(*inputcc->sneakKey) || moduleMgr->getModule<Sneak>()->isEnabled()))
		{
			MoveUtil::setSpeed(sneakspeed2);
		}
		else
		{
			MoveUtil::setSpeed(speed);
		}
	}
	// IGN Pvp speed i found
	if (mode.getSelectedValue() == 3)
	{
		if (!isInLiquid) {
			static bool useVelocity = false;
			if (height >= 0.385)
			{
				if (player->onGround && pressed)
					player->jumpFromGround();
				useVelocity = false;
			}
			else
				useVelocity = true;
			if (height <= 0.04 && !input->isJumping)
			{
				useVelocity = false;
			}
			if (player->onGround && useVelocity && pressed && !input->isJumping)
				player->velocity.y = height;
			C_GameSettingsInput* inputcc = g_Data.getClientInstance()->getGameSettingsInput();
			if (pressed && !player->onGround)
				MoveUtil::setSpeed(jumpSpeed);
			if (pressed && player->onGround) {
				MoveUtil::setSpeed(speed);
				player->jumpFromGround();
				player->velocity.y = -1;
			}
		}
		else if (isInLiquid) {
			static bool useVelocity = false;
			if (height >= 0.385)
			{
				if (player->onGround && pressed)
					player->jumpFromGround();
				useVelocity = false;
			}
			else
				useVelocity = true;
			if (height <= 0.04 && !input->isJumping)
			{
				player->velocity.y += height;
				useVelocity = false;
			}

			if (player->onGround && useVelocity && pressed && !input->isJumping)
				player->velocity.y = height;
			C_GameSettingsInput* inputcc = g_Data.getClientInstance()->getGameSettingsInput();
			MoveUtil::setSpeed(speed);
		}
	}
}

void Speed::onSendPacket(C_Packet *packet)
{
	C_GameSettingsInput *input = g_Data.getClientInstance()->getGameSettingsInput();
	auto scaffold = moduleMgr->getModule<Scaffold>();
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr || input == nullptr)
		return;
}

void Speed::onDisable()
{
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
		return;
	MoveUtil::stop(false);

	preventKick = false;
	packetsSent = 0;
}