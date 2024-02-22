#include "pch.h"
#include "Config.h"

#include <INIReader.h>

std::string Config::gameDirectory;

float Config::desiredFrameRate;

bool Config::desiredFpsOverridesNetworkDataRates;

bool Config::use30FpsLuaDefaultWaitTime;
bool Config::allowScriptIdentitySpoofing;

void Config::init()
{
	gameDirectory.resize(MAX_PATH);
	GetModuleFileName(nullptr, &gameDirectory[0], MAX_PATH);
	gameDirectory.resize(gameDirectory.rfind('\\'));

	INIReader ini(gameDirectory + "/rowblonks.ini");

	desiredFrameRate = static_cast<float>(ini.GetReal("Game", "DesiredFrameRate", 60.0));

	desiredFpsOverridesNetworkDataRates = ini.GetBoolean("Network", "DesiredFpsOverridesNetworkDataRates", true);

	use30FpsLuaDefaultWaitTime = ini.GetBoolean("Lua", "Use30FpsLuaDefaultWaitTime", true);
	allowScriptIdentitySpoofing = ini.GetBoolean("Lua", "AllowScriptIdentitySpoofing", false);
}
