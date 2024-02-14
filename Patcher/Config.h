#pragma once

namespace Config
{
	extern std::string gameDirectory;

	extern float desiredFrameRate;
	extern bool desiredRenderFpsOverridesDataRates;

	void init();
}
