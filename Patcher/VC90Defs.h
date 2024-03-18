#pragma once

// this is necessary for compatibility, as we are using a newer version of MSVC as well as possibly different build configs

namespace vc90
{
	const auto _RTDynamicCast = reinterpret_cast<PVOID (__cdecl**)(PVOID inptr, LONG VfDelta, PVOID SrcType, PVOID TargetType, BOOL isReference)>(0x00A40A24);

	namespace std
	{
		// ===== `string` class =====
		// in other words, a `std::basic_string<char, std::char_traits<char>, std::allocator<char>>`

		const auto string__destructor = reinterpret_cast<void(__thiscall**)(class string* _this)>(0x00A404D0);

		class string
		{
		public:
			// destructor helper
			static void destruct(string* str)
			{
				(*string__destructor)(str);
			}
		};
	}
}
