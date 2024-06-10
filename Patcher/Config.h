#pragma once

namespace Config
{
	extern std::string gameDirectory;

	extern float desiredFrameRate;
	extern bool physicsFpsUnlocked;

	extern bool desiredFpsOverridesNetworkDataRates;

	extern bool use30FpsLuaDefaultWaitTime;
	extern bool allowScriptIdentitySpoofing;

	void init();

	extern bool initialized;
}
