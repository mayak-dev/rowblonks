#pragma once

#include "Patches.h"

// this is necessary for compatibility, as we are using a newer version of MSVC as well as possibly different build configs

namespace vc90
{
	const auto operator_new = reinterpret_cast<void* (__cdecl**)(size_t)>(Patches::resolveNewVa(0x00A42FC0));

	const auto _RTDynamicCast = reinterpret_cast<PVOID (__cdecl**)(PVOID inptr, LONG VfDelta, PVOID SrcType, PVOID TargetType, BOOL isReference)>(Patches::resolveNewVa(0x00A40A24));

	const auto _CxxThrowException = reinterpret_cast<void(__stdcall**)(void* pExceptionObject, void* pThrowInfo)>(Patches::resolveNewVa(0x00A40A5C));

	namespace std
	{
		// ===== `string` class =====
		// in other words, a `std::basic_string<char, std::char_traits<char>, std::allocator<char>>`
		// this DLL is built with _ITERATOR_DEBUG_LEVEL=1 on Release, making std::string and vc90::std::string *mostly* compatible

		const auto string__destructor = reinterpret_cast<void(__thiscall**)(class string* _this)>(Patches::resolveNewVa(0x00A404D0));

		class string
		{
		public:
			// destructor helper
			static void destruct(string* str)
			{
				(*string__destructor)(str);
			}
		};

		const auto string__assign = reinterpret_cast<void(__thiscall**)(string* _this, const char* str)>(Patches::resolveNewVa(0x00A404B4));

		// ===== `runtime_error` class =====

		const auto runtime_error__constructor = reinterpret_cast<class runtime_error* (__thiscall*)(runtime_error* _this, const ::std::string& message)>(Patches::resolveNewVa(0x0040A950));

		const auto runtime_error_ThrowInfo = reinterpret_cast<void*>(Patches::resolveNewVa(0x00B89120));

		class runtime_error
		{
		public:
			constexpr static size_t size = 40;

			// constructor helper
			static runtime_error* construct(const ::std::string& message)
			{
				auto result = reinterpret_cast<runtime_error*>((*vc90::operator_new)(size));
				(*runtime_error__constructor)(result, message);
				return result;
			}

			// helper to throw
			static void raise(runtime_error* error)
			{
				(*_CxxThrowException)(error, runtime_error_ThrowInfo);
			}
		};
	}
}
