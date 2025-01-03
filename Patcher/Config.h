#pragma once

namespace Config
{
	extern std::string gameDirectory;

	extern int desiredFrameRate;
	extern bool physicsFpsUnlocked;
	extern float cameraFov;

	extern bool desiredFpsOverridesNetworkDataRates;

	extern bool use30FpsLuaDefaultWaitTime;
	extern bool allowScriptIdentitySpoofing;

	extern std::vector<std::string> assetOverrides;

	extern bool readPlayerBetaCookieJar;

	void init();

	extern bool initialized;
}
