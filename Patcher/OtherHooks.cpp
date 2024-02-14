#include "pch.h"
#include "OtherHooks.h"
#include "Config.h"

// ===== bypass script hash verification =====

sub_6C34D0_t sub_6C34D0_orig = reinterpret_cast<sub_6C34D0_t>(0x006C34D0);

bool __fastcall sub_6C34D0_hook(void* _this, void*, vc90::std::string* scriptHash, vc90::std::string*, bool)
{
	return true;
}

sub_6C47A0_t sub_6C47A0_orig = reinterpret_cast<sub_6C47A0_t>(0x006C47A0);

bool __fastcall sub_6C47A0_hook(void* _this, void*, vc90::std::string*, int)
{
	return true;
}

// ===== bypass "invalid request" for some urls =====

sub_794AF0_t sub_794AF0_orig = reinterpret_cast<sub_794AF0_t>(0x00794AF0);

// really only takes 1 std::string by value, and is therefore responsible for calling its destructor
void __cdecl sub_794AF0_hook(char a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	auto url = reinterpret_cast<vc90::std::string*>(&a1);
	vc90::std::string::destruct(url);
}

// ===== unlock fps (4) =====

void* ptrToHook_6668F6 = reinterpret_cast<void*>(0x006668F6);

static float __stdcall getFlyCamAccelMultiplier(int steps)
{
	const float fpsRatio = 60.0f / Config::desiredFrameRate;

	float multiplier = fpsRatio;

	if (steps > 120.0f / fpsRatio)
		multiplier *= 8.0f;
	else if (steps > 60.0f / fpsRatio)
		multiplier *= 4.0f;
	else
		multiplier *= 2.0f;

	return multiplier;
}

// inline hook
// change fly camera acceleration multiplier with respect to desired frame rate
void __declspec(naked) inlineHook_6668F6()
{
	__asm
	{
		push [esp + 0x98]
		call getFlyCamAccelMultiplier
		fstp [esp + 0x14]

		// jump to original code, skipping the original calculation
		mov eax, 0x0066692D
		jmp eax
	}
}

// ===== raknet ID_OUT_OF_BAND_INTERNAL packet crash fix =====

void* ptrToHook_529031 = reinterpret_cast<void*>(0x00529031);

// inline hook
// add a missing minimum length check for offline message packets with ID_OUT_OF_BAND_INTERNAL
void __declspec(naked) inlineHook_529031()
{
	__asm
	{
		// at this point, packet length is in ECX
		cmp ecx, 0x19 // sizeof(OFFLINE_MESSAGE_DATA_ID) + sizeof(MessageID) + RakNetGUID::size() == 0x19
		jae suc

		// length check failed; abort mission
		mov eax, 0x00529101
		jmp eax
		
	suc:
		// jump to original code, continuing as normal
		jmp [ptrToHook_529031]
	}
}

// ===== use "rowblonks" directories instead of "Roblox" =====

static void fixRobloxPath(std::string& path)
{
	constexpr const char* robloxDir = "Roblox\\";

	size_t pos = path.rfind(robloxDir);
	if (pos != std::string::npos)
		path.replace(pos, std::strlen(robloxDir), "rowblonks\\");
}

sub_636AB0_t sub_636AB0_orig = reinterpret_cast<sub_636AB0_t>(0x00636AB0);

vc90::std::string* __cdecl sub_636AB0_hook(vc90::std::string* a1, bool a2, int a3, const char* a4)
{
	auto res = sub_636AB0_orig(a1, a2, a3, a4);

	std::string path = (*vc90::std::string__c_str)(res);
	fixRobloxPath(path);

	(*vc90::std::string__assign_from_c_str)(res, path.c_str());
	return res;
}

CreateDirectoryA_t CreateDirectoryA_orig = CreateDirectoryA;

BOOL __stdcall CreateDirectoryA_hook(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	std::string path = lpPathName;
	fixRobloxPath(path);

	return CreateDirectoryA_orig(path.c_str(), lpSecurityAttributes);
}
