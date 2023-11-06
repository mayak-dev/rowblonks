#pragma once

// this is necessary for compatibility, as we are using a newer version of MSVC as well as possibly different build configs

namespace vc90
{
	const auto operator_new = reinterpret_cast<void* (__cdecl**)(size_t)>(0x00A42FC0);

	const auto _RTDynamicCast = reinterpret_cast<PVOID (__cdecl**)(PVOID inptr, LONG VfDelta, PVOID SrcType, PVOID TargetType, BOOL isReference)>(0x00A40A24);

	namespace std
	{
		// ===== `string` class =====
		// in other words, a `std::basic_string<char, std::char_traits<char>, std::allocator<char>>`

		class string;

		const auto string__constructor = reinterpret_cast<string* (__thiscall**)(string* _this, const char*)>(0x00A404C4);
		const auto string__destructor = reinterpret_cast<void(__thiscall**)(string* _this)>(0x00A404D0);

		// constructor helper
		inline string* create_string(const char* str)
		{
			constexpr size_t sizeof_string = 28;

			auto result = reinterpret_cast<string*>((*operator_new)(sizeof_string));
			(*string__constructor)(result, str);

			return result;
		}

		const auto string__c_str = reinterpret_cast<const char* (__thiscall**)(string* _this)>(0x00A404CC);
	}
}
