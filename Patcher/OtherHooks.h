#pragma once

#include "VC90Defs.h"

// ===== bypass script hash verification =====
typedef bool(__thiscall* sub_6C34D0_t)(void* _this, void*, const std::string& scriptHash, const std::string&, bool);
extern sub_6C34D0_t sub_6C34D0_orig;

bool __fastcall sub_6C34D0_hook(void* _this, void*, const std::string& scriptHash, const std::string&, bool);

typedef bool(__thiscall* sub_6C47A0_t)(void* _this, const std::string&, int);
extern sub_6C47A0_t sub_6C47A0_orig;

bool __fastcall sub_6C47A0_hook(void* _this, void*, const std::string&, int);

// ===== bypass "invalid request" for some urls =====
// really only takes 1 std::string by value, and is responsible for calling its destructor
typedef void(__cdecl* sub_794AF0_t)(char a1, int a2, int a3, int a4, int a5, int a6, int a7);
extern sub_794AF0_t sub_794AF0_orig;

void __cdecl sub_794AF0_hook(char a1, int a2, int a3, int a4, int a5, int a6, int a7);

// ===== unlock fps (flycam) =====

extern void* ptrToHook_6668F6;

// inline hook
void inlineHook_6668F6();

// ===== raknet ID_OUT_OF_BAND_INTERNAL packet crash fix =====

extern void* ptrToHook_529031;

// inline hook
void inlineHook_529031();

// ===== fix TextXmlParser hanging while attempting to parse attributes =====

extern void* ptrToHook_613019;

// inline hook
void inlineHook_613019();

// ===== unlock fps (humanoid physics) =====

typedef double(__cdecl* sub_79F6A0_t)();
extern sub_79F6A0_t sub_79F6A0_orig;

double sub_79F6A0_hook();

// ===== unlock fps (world timing) =====

typedef int(__cdecl* sub_79F680_t)();
extern sub_79F680_t sub_79F680_orig;

int sub_79F680_hook();

typedef double(__cdecl* sub_79F6B0_t)();
extern sub_79F6B0_t sub_79F6B0_orig;

double sub_79F6B0_hook();

// ===== unlock fps (motor[6d] physics) =====

extern void* ptrToHook_7A3203;

void inlineHook_7A3203();

extern void* ptrToHook_7A39C3;

void inlineHook_7A39C3();
