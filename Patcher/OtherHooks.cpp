#include "pch.h"
#include "OtherHooks.h"

// ===== bypass script hash verification =====

sub_6C34D0_t sub_6C34D0 = reinterpret_cast<sub_6C34D0_t>(0x006C34D0);

bool __fastcall hook_sub_6C34D0(void* _this, void*, vc90::std::string* scriptHash, vc90::std::string*, bool)
{
	return true;
}

sub_6C47A0_t sub_6C47A0 = reinterpret_cast<sub_6C47A0_t>(0x006C47A0);

bool __fastcall hook_sub_6C47A0(void* _this, void*, vc90::std::string*, int)
{
	return true;
}

// ===== bypass "invalid request" for some urls =====

sub_794AF0_t sub_794AF0 = reinterpret_cast<sub_794AF0_t>(0x00794AF0);

// really only takes 1 std::string by value, and is responsible for calling its destructor
void __cdecl hook_sub_794AF0(char a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	auto url = reinterpret_cast<vc90::std::string*>(&a1);
	(*vc90::std::string__destructor)(url);
}
