#pragma once

namespace Config
{
	extern std::string gameDirectory;

	void getGameDirectory(HMODULE hModule);

	extern float desiredFrameRate;

	void read();
}
