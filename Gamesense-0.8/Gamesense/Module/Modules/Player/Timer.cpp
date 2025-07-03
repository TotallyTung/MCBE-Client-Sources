#include "Timer.h"

using namespace std;
Timer::Timer() : IModule(0, Category::PLAYER, "Modifies the games speed")
{
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Vanilla", 0);    // Normal timer
	mode.addEntry("Nukkit", 1);	    // Bypass for ign servers aka nukkit servers
	mode.addEntry("TickShift", 2);  // Switches between min tps and max tps, can bypass nukkit if u have the right config
	mode.addEntry("NukkitSafe", 3); // Safer version of nukkit
	registerBoolSetting("Show TPS", &showTps, showTps);
	registerIntSetting("Vanilla TPS", &timer, timer, 1, 500);
	registerIntSetting("TickShift Min TPS", &minTps, minTps, 1, 500);
	registerIntSetting("TickShift Max TPS", &maxTps, maxTps, 1, 500);
	registerFloatSetting("TickShift Duration", &duration, duration, 0.5f, 20.f); // Duratiom of each random tick, 20 = 1 second
}

const char *Timer::getRawModuleName()
{
	return "Timer";
}

void Timer::onTick(C_GameMode *gm)
{
	if (mode.getSelectedValue() == 0)
	{
		g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
	}
	if (mode.getSelectedValue() == 1)
	{
		g_Data.getClientInstance()->minecraft->setTimerSpeed(26.f);
	}
	if (mode.getSelectedValue() == 2)
	{
		speedShift *= duration;
		speedShift = randomFloat(minTps, maxTps);
		g_Data.getClientInstance()->minecraft->setTimerSpeed(speedShift);
	}
	if (mode.getSelectedValue() == 3)
	{
		g_Data.getClientInstance()->minecraft->setTimerSpeed(25.f);
	}
}
const char *Timer::getModuleName()
{
	auto timerMod = moduleMgr->getModule<Timer>();
	switch (mode.getSelectedValue())
	{
	case 0:
		if (showTps)
		{
			name = string("Timer ") + string(GRAY) + string("Vanilla") + string(WHITE) + string(" [") + string(GRAY) + to_string((int)(timer)) + string(WHITE) + string("]");
		}
		if (!showTps)
		{
			name = string("Timer ") + string(GRAY) + string("Vanilla");
		}
		break;
	case 1:
		name = string("Timer ") + string(GRAY) + string("Nukkit");
		break;
	case 2:
		if (showTps)
		{
			name = string("Timer ") + string(GRAY) + string("TickShift") + string(WHITE) + string(" [") + string(GRAY) + to_string((int)(speedShift)) + string(WHITE) + string("]");
		}
		if (!showTps)
		{
			name = string("Timer ") + string(GRAY) + string("TickShift");
		}
		break;
	case 3:
		name = string("Timer ") + string(GRAY) + string("Nukkit");
		break;
	}
	return name.c_str();
}

void Timer::onDisable()
{
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}