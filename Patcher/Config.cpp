#include "pch.h"
#include "Config.h"

#include <INIReader.h>

std::string Config::gameDirectory;

float Config::desiredFrameRate;
bool Config::physicsFpsUnlocked;

bool Config::desiredFpsOverridesNetworkDataRates;

bool Config::use30FpsLuaDefaultWaitTime;
bool Config::allowScriptIdentitySpoofing;

bool Config::initialized;

void Config::init()
{
	gameDirectory.resize(MAX_PATH);
	GetModuleFileName(nullptr, &gameDirectory[0], MAX_PATH);
	gameDirectory.resize(gameDirectory.rfind('\\'));

	INIReader ini(gameDirectory + "/rowblonks.ini");

	desiredFrameRate = static_cast<float>(ini.GetReal("Game", "DesiredFrameRate", 60.0));
	physicsFpsUnlocked = static_cast<float>(ini.GetBoolean("Game", "PhysicsFpsUnlocked", false));

	desiredFpsOverridesNetworkDataRates = ini.GetBoolean("Network", "DesiredFpsOverridesNetworkDataRates", true);

	use30FpsLuaDefaultWaitTime = ini.GetBoolean("Lua", "Use30FpsLuaDefaultWaitTime", true);
	allowScriptIdentitySpoofing = ini.GetBoolean("Lua", "AllowScriptIdentitySpoofing", false);

	initialized = true;
}
