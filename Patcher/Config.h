#pragma once

namespace Config
{
	extern std::string gameDirectory;

	extern float desiredFrameRate;

	extern bool desiredFpsOverridesNetworkDataRates;

	extern bool use30FpsLuaDefaultWaitTime;
	extern bool allowScriptIdentitySpoofing;

	void init();
}
