#include "pch.h"
#include "Config.h"

#include <INIReader.h>

std::string Config::gameDirectory;

void Config::getGameDirectory(HMODULE hModule)
{
	gameDirectory.resize(MAX_PATH);
	GetModuleFileName(hModule, &gameDirectory[0], MAX_PATH);
	gameDirectory.resize(gameDirectory.rfind('\\'));
}

// default values can go here
float Config::desiredFrameRate = 60.0f;

void Config::read()
{
	INIReader ini(gameDirectory + "/rowblonks.ini");

	desiredFrameRate = static_cast<float>(ini.GetReal("", "DesiredFrameRate", desiredFrameRate));
}
