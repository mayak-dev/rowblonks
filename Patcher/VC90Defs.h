#pragma once

// this is necessary for compatibility, as we are using a newer version of MSVC as well as possibly different build configs

namespace vc90
{
	const auto operator_new = reinterpret_cast<void* (__cdecl**)(size_t)>(0x00A42FC0);

	const auto _RTDynamicCast = reinterpret_cast<PVOID (__cdecl**)(PVOID inptr, LONG VfDelta, PVOID SrcType, PVOID TargetType, BOOL isReference)>(0x00A40A24);

	const auto _CxxThrowException = reinterpret_cast<void(__stdcall**)(void* pExceptionObject, void* pThrowInfo)>(0x00A40A5C);

	namespace std
	{
		// ===== `string` class =====
		// in other words, a `std::basic_string<char, std::char_traits<char>, std::allocator<char>>`

		const auto string__constructor = reinterpret_cast<class string* (__thiscall**)(string* _this, const char*)>(0x00A404C4);
		const auto string__destructor = reinterpret_cast<void(__thiscall**)(string* _this)>(0x00A404D0);

		class string
		{
		public:
			constexpr static size_t size = 28;

			// constructor helper
			static string* construct(const char* str)
			{
				auto result = reinterpret_cast<string*>((*operator_new)(size));
				(*string__constructor)(result, str);

				return result;
			}

			// destructor helper
			static void destruct(string* str)
			{
				(*string__destructor)(str);
			}
		};

		const auto string__c_str = reinterpret_cast<const char* (__thiscall**)(string* _this)>(0x00A404CC);
		const auto string__assign_from_c_str = reinterpret_cast<void(__thiscall**)(string* _this, const char* str)>(0x00A404B4);

		// ===== `runtime_error` class =====

		const auto runtime_error__constructor = reinterpret_cast<class runtime_error*(__thiscall*)(runtime_error* _this, string* message)>(0x0040A950);

		class runtime_error
		{
		public:
			constexpr static size_t size = 40;

			// constructor helper
			static runtime_error* construct(const char* message)
			{
				auto messageStr = string::construct(message);

				auto result = reinterpret_cast<runtime_error*>((*operator_new)(size));
				(*runtime_error__constructor)(result, messageStr);

				string::destruct(messageStr);

				return result;
			}

			// helper to throw
			static void raise(runtime_error* error)
			{
				(*_CxxThrowException)(error, reinterpret_cast<void*>(0x00B89120));
			}
		};
	}
}
