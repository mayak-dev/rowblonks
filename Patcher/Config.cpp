#include "pch.h"
#include "Config.h"

#include <INIReader.h>

std::string Config::gameDirectory;

float Config::desiredFrameRate;
bool Config::desiredRenderFpsOverridesDataRates;

void Config::init()
{
	gameDirectory.resize(MAX_PATH);
	GetModuleFileName(nullptr, &gameDirectory[0], MAX_PATH);
	gameDirectory.resize(gameDirectory.rfind('\\'));

	INIReader ini(gameDirectory + "/rowblonks.ini");

	desiredFrameRate = static_cast<float>(ini.GetReal("Rendering", "DesiredFrameRate", 60.0));
	desiredRenderFpsOverridesDataRates = ini.GetBoolean("Network", "DesiredRenderFpsOverridesDataRates", true);
}
