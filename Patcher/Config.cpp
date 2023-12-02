#include "pch.h"
#include "Config.h"

#include <INIReader.h>

void Config::getGameDirectory(HMODULE hModule)
{
	Config::gameDirectory.resize(MAX_PATH);
	GetModuleFileName(hModule, &Config::gameDirectory[0], MAX_PATH);
	Config::gameDirectory.resize(Config::gameDirectory.rfind('\\'));
}

// default values can go here
float Config::desiredFrameRate = 60.0f;

void Config::read()
{
	INIReader ini(Config::gameDirectory + "/rowblonks.ini");

	Config::desiredFrameRate = static_cast<float>(ini.GetReal("", "DesiredFrameRate", Config::desiredFrameRate));
}
