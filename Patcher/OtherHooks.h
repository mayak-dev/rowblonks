#pragma once

#include "VC90Defs.h"

// ===== bypass script hash verification =====
typedef bool(__thiscall* sub_6C34D0_t)(void* _this, void*, vc90::std::string* scriptHash, vc90::std::string*, bool);
extern sub_6C34D0_t sub_6C34D0_orig;

bool __fastcall sub_6C34D0_hook(void* _this, void*, vc90::std::string* scriptHash, vc90::std::string*, bool);

typedef bool(__thiscall* sub_6C47A0_t)(void* _this, vc90::std::string*, int);
extern sub_6C47A0_t sub_6C47A0_orig;

bool __fastcall sub_6C47A0_hook(void* _this, void*, vc90::std::string*, int);

// ===== bypass "invalid request" for some urls =====
// really only takes 1 std::string by value, and is responsible for calling its destructor
typedef void(__cdecl* sub_794AF0_t)(char a1, int a2, int a3, int a4, int a5, int a6, int a7);
extern sub_794AF0_t sub_794AF0_orig;

void __cdecl sub_794AF0_hook(char a1, int a2, int a3, int a4, int a5, int a6, int a7);

// ===== unlock fps (4) =====

extern void* ptrToHook_6668F6;

// inline hook
void inlineHook_6668F6();

// ===== raknet ID_OUT_OF_BAND_INTERNAL packet crash fix =====

extern void* ptrToHook_529031;

// inline hook
void inlineHook_529031();

// ===== use "rowblonks" directories instead of "Roblox" =====

typedef vc90::std::string* (__cdecl* sub_636AB0_t)(vc90::std::string* a1, bool a2, int a3, const char* a4);
extern sub_636AB0_t sub_636AB0_orig;

vc90::std::string* __cdecl sub_636AB0_hook(vc90::std::string* a1, bool a2, int a3, const char* a4);

typedef BOOL (__stdcall* CreateDirectoryA_t)(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
extern CreateDirectoryA_t CreateDirectoryA_orig;

BOOL __stdcall CreateDirectoryA_hook(LPCSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);

// ===== fix TextXmlParser hanging while attempting to parse attributes =====

extern void* ptrToHook_613019;

// inline hook
void inlineHook_613019();
