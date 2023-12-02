#pragma once

#include "VC90Defs.h"

// ===== bypass script hash verification =====
typedef bool(__thiscall* sub_6C34D0_t)(void* _this, void*, vc90::std::string* scriptHash, vc90::std::string*, bool);
extern sub_6C34D0_t sub_6C34D0;

bool __fastcall hook_sub_6C34D0(void* _this, void*, vc90::std::string* scriptHash, vc90::std::string*, bool);

typedef bool(__thiscall* sub_6C47A0_t)(void* _this, vc90::std::string*, int);
extern sub_6C47A0_t sub_6C47A0;

bool __fastcall hook_sub_6C47A0(void* _this, void*, vc90::std::string*, int);

// ===== bypass "invalid request" for some urls =====
// really only takes 1 std::string by value, and is responsible for calling its destructor
typedef void(__cdecl* sub_794AF0_t)(char a1, int a2, int a3, int a4, int a5, int a6, int a7);
extern sub_794AF0_t sub_794AF0;

void __cdecl hook_sub_794AF0(char a1, int a2, int a3, int a4, int a5, int a6, int a7);

// ===== unlock fps (4) =====

extern void* ptr_6668F6;

// inline hook
void hook_6668F6();
