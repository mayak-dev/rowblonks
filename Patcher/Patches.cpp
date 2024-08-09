#include "pch.h"
#include "Patches.h"
#include "RBXHooks.h"
#include "OtherHooks.h"
#include "Config.h"

#include <detours.h>

static const std::unordered_map<void*, void*> hooks = {
    // ===== `CRobloxWnd::RenderRequestJob` member function hooks =====
    { &CRobloxWnd__RenderRequestJob__sleepTime_orig, CRobloxWnd__RenderRequestJob__sleepTime_hook },

    // ===== `CRobloxWnd::UserInputJob` member function hooks =====
    { &CRobloxWnd__UserInputJob__sleepTime_orig, CRobloxWnd__UserInputJob__sleepTime_hook },

    // ===== `RBX::HeartbeatTask` member function hooks ====
    { &RBX::HeartbeatTask__constructor_orig, RBX::HeartbeatTask__constructor_hook },

    // ===== `RBX::PhysicsJob` member function hooks =====
	{ &RBX::PhysicsJob__constructor_orig, RBX::PhysicsJob__constructor_hook },

    // ===== `RBX::ContentProvider` member function hooks =====
    { &RBX::ContentProvider__verifyScriptSignature_orig, RBX::ContentProvider__verifyScriptSignature_hook },
    { &RBX::ContentProvider__verifyRequestedScriptSignature_orig, RBX::ContentProvider__verifyScriptSignature_hook },

    // ===== `RBX::ContentId` member function hooks =====
    { &RBX::ContentId__convertLegacyContent_orig, RBX::ContentId__convertLegacyContent_hook },

    // ===== `RBX:Http` member function hooks =====
    { &RBX::Http__trustCheck_orig, RBX::Http__trustCheck_hook },

    // ===== `RBX::DataModel` member function hooks =====
    { &RBX::DataModel__startCoreScripts_orig, RBX::DataModel__startCoreScripts_hook },
    { &RBX::DataModel__physicsStep_orig, RBX::DataModel__physicsStep_hook },

    // ===== `RBX::ScriptContext` member function hooks =====
    { &RBX::ScriptContext__openState_orig, RBX::ScriptContext__openState_hook },
    { &RBX::ScriptContext__executeInNewThread_orig, RBX::ScriptContext__executeInNewThread_hook },

    // ===== `RBX::Network::Replicator::RockyItem` member function hooks =====
    { &RBX::Network::Replicator__RockyItem__write_orig, RBX::Network__Replicator__RockyItem__write_hook },

    // ===== `RBX::PlayerChatLine` member function hooks =====
    { &RBX::PlayerChatLine__constructor_orig, RBX::PlayerChatLine__constructor_hook },

    // ===== `RBX::NetworkSettings` member function hooks ====
    { &RBX::NetworkSettings__setDataSendRate_orig, RBX::NetworkSettings__setDataSendRate_hook },
    { &RBX::NetworkSettings__setReceiveRate_orig, RBX::NetworkSettings__setReceiveRate_hook },
    { &RBX::NetworkSettings__setPhysicsSendRate_orig, RBX::NetworkSettings__setPhysicsSendRate_hook },

    // ===== `RBX::VideoControl` member function hooks =====
    { &RBX::VideoControl__isVideoRecording_orig, RBX::VideoControl__isVideoRecording_hook },

    // ===== `RBX::RunService` member function hooks =====
    { &RBX::RunService__step_orig, RBX::RunService__step_hook },

    // ===== `RBX::RotateConnector` member function hooks =====
    { &RBX::RotateConnector__setRotationalGoal_orig, RBX::RotateConnector__setRotationalGoal_hook },
    { &RBX::RotateConnector__setVelocityGoal_orig, RBX::RotateConnector__setVelocityGoal_hook },

    // ===== other hooks =====
    { &sub_6C34D0_orig, sub_6C34D0_hook },
    { &sub_6C47A0_orig, sub_6C47A0_hook },
    { &sub_794AF0_orig, sub_794AF0_hook },
    { &ptrToHook_6668F6, inlineHook_6668F6 },
    { &ptrToHook_529031, inlineHook_529031 },
    { &ptrToHook_613019, inlineHook_613019 },
    { &sub_79F6A0_orig, sub_79F6A0_hook },
    { &sub_79F680_orig, sub_79F680_hook },
    { &sub_79F6B0_orig, sub_79F6B0_hook },
    { &ptrToHook_7A3221, inlineHook_7A3221 },
    { &ptrToHook_7A39E1, inlineHook_7A39E1 },
};

#ifdef _DEBUG
static std::runtime_error patchError(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    int size = vsnprintf(nullptr, 0, format, args) + 1;

    std::string buffer;
    buffer.resize(size);

    vsnprintf(&buffer[0], size, format, args);

    va_end(args);

    return std::runtime_error(buffer);
}
#else
#define patchError(x, ...) std::runtime_error("")
#endif

static void initHooks()
{
    LONG error = DetourTransactionBegin();
    if (error != NO_ERROR)
        throw patchError("DetourTransactionBegin returned %d", error);

    for (auto& hook : hooks)
    {
        auto ptr = reinterpret_cast<void**>(hook.first);

        error = DetourAttach(ptr, hook.second);
        if (error != NO_ERROR)
            throw patchError("Couldn't hook function at 0x%p! DetourAttach returned %d", *ptr, error);
    }

    error = DetourTransactionCommit();
    if (error != NO_ERROR)
        throw patchError("DetourTransactionCommit returned %d", error);
}

static void writeBytes(void* address, const char* data, size_t size, DWORD flags)
{
    DWORD oldFlags;
    if (!VirtualProtect(address, size, flags, &oldFlags))
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (1)", address, reinterpret_cast<DWORD>(address) + size);

    std::memcpy(address, data, size);

    if (!VirtualProtect(address, size, oldFlags, &oldFlags))
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (2)", address, reinterpret_cast<DWORD>(address) + size);
}

template<typename T>
static void writeValue(void* address, T value, DWORD flags)
{
    writeBytes(address, reinterpret_cast<const char*>(&value), sizeof(T), flags);
}

static void fillBytes(void* address, uint8_t value, size_t size, DWORD flags)
{
    DWORD oldFlags;
    if (!VirtualProtect(address, size, flags, &oldFlags))
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (1)", address, reinterpret_cast<DWORD>(address) + size);

    std::memset(address, value, size);

    if (!VirtualProtect(address, size, oldFlags, &oldFlags))
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (2)", address, reinterpret_cast<DWORD>(address) + size);
}

inline uint32_t Patches::resolveNewVa(uint32_t old)
{
    constexpr uint32_t imageBase = 0x00400000;
    static const uint32_t newImageBase = reinterpret_cast<uint32_t>(GetModuleHandle(nullptr));

    return old - imageBase + newImageBase;
}

void Patches::init()
{
    assert(Config::initialized);

    constexpr uint8_t NOP = 0x90;

    // ===== bypass SSL certificate checks =====
    // we are writing a relative JMP from 0x006EAAB0 to 0x006EABF7, within RBX::Http::httpGetPostWinInet
    writeBytes(reinterpret_cast<void*>(resolveNewVa(0x006EAAB0)), "\xE9\x42\x01\x00\x00", 0x7, PAGE_EXECUTE_READWRITE);

    // SECURITY BYPASS
    // ===== always send a clean value for RBX::DataModel::sendStats =====
    // this is so that the server doesn't receive any so-called "hack flags"
    // write `XOR EDX, EDX` followed by a series of NOPs where these flags would normally be stored in EDX
    writeBytes(reinterpret_cast<void*>(resolveNewVa(0x005105A5)), "\x33\xD2\x90\x90\x90\x90\x90", 0x7, PAGE_EXECUTE_READWRITE);

    // ===== fix freezing from dragging IDE toolbars =====
    // removes a redundant call to GetMessageA and a comparison of its result following PeekMessageA in the window message loop
    fillBytes(reinterpret_cast<void*>(resolveNewVa(0x008A2073)), NOP, 0x15, PAGE_EXECUTE_READWRITE);

    // ===== disable executing Lua bytecode provided by the user =====
    // this makes it so f_parser will always call luaY_parser instead of luaU_undump
    fillBytes(reinterpret_cast<void*>(resolveNewVa(0x0077E6BC)), NOP, 0xA, PAGE_EXECUTE_READWRITE);

    // ===== unlock fps (kernel timing) =====
    // the getter for this constant was optimized out in some places, so we have to overwrite the value directly
    // it is used for calculating how forces are applied over each kernel iteration
    if (Config::physicsFpsUnlocked)
        writeValue(reinterpret_cast<float*>(resolveNewVa(0x00A9B6A4)), 1.0f / (19.0f * 4.0f * Config::desiredFrameRate), PAGE_READWRITE);

    initHooks();
}
