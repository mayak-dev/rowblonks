#include "pch.h"
#include "Patches.h"
#include "RBXHooks.h"
#include "OtherHooks.h"

#include <detours.h>

static std::map<void*, void*> hooks = {
    // ===== `RBX::ContentProvider` member function hooks =====
    { &RBX::ContentProvider__verifyScriptSignature, RBX__ContentProvider__verifyScriptSignature_hook },
    { &RBX::ContentProvider__verifyRequestedScriptSignature, RBX__ContentProvider__verifyScriptSignature_hook },

    // ===== `RBX:Http` member function hooks =====
    { &RBX::Http__constructor, RBX__Http__constructor_hook },
    { &RBX::Http__trustCheck, RBX__Http__trustCheck_hook },

    // ===== `RBX::DataModel` member function hooks =====
    { &RBX::DataModel__startCoreScripts, RBX__DataModel__startCoreScripts_hook },

    // ===== `RBX::ScriptContext` member function hooks =====
    { &RBX::ScriptContext__openState, RBX__ScriptContext__openState_hook },

    // ===== `RBX::Network::Replicator::RockyItem` member function hooks =====
    { &RBX::Network::Replicator__RockyItem__write, RBX__Network__Replicator__RockyItem__write_hook },

    // ===== other hooks =====
    { &sub_6C34D0, sub_6C34D0_hook },
    { &sub_6C47A0, sub_6C47A0_hook },
    { &sub_794AF0, sub_794AF0_hook },
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

static void initializeHooks()
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
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (1)", address, (DWORD)address + size);

    std::memcpy(address, data, size);

    if (!VirtualProtect(address, size, oldFlags, &oldFlags))
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (2)", address, (DWORD)address + size);
}

static void fillBytes(void* address, int value, size_t size, DWORD flags)
{
    DWORD oldFlags;
    if (!VirtualProtect(address, size, flags, &oldFlags))
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (1)", address, (DWORD)address + size);

    std::memset(address, value, size);

    if (!VirtualProtect(address, size, oldFlags, &oldFlags))
        throw patchError("VirtualProtect failed to change protection flags for 0x%p - 0x%p (2)", address, (DWORD)address + size);
}

void Patches::initialize()
{
    // ===== bypass SSL certificate checks =====
    // we are writing a JMP from 0x006EAAB0 to 0x006EABF7, within RBX::Http::httpGetPostWinInet
    writeBytes(reinterpret_cast<void*>(0x006EAAB0), "\xE9\x42\x01\x00\x00", 5, PAGE_EXECUTE_READWRITE);

    // SECURITY BYPASS
    // ===== high-privilege "Local url" scripts =====
    // have these scripts execute with identity 7
    // this is replacing PUSH 1 with PUSH 7
    fillBytes(reinterpret_cast<void*>(0x00478637), 7, 1, PAGE_EXECUTE_READWRITE);

    // SECURITY BYPASS
    // ===== always send a clean value for RBX::DataModel::sendStats =====
    // this is so that the server doesn't receive any so-called "hack flags"
    // write `XOR EDX, EDX` followed by a series of NOPs where these flags would normally be stored in EDX
    writeBytes(reinterpret_cast<void*>(0x005105A5), "\x33\xD2\x90\x90\x90\x90\x90", 7, PAGE_EXECUTE_READWRITE);

    initializeHooks();
}
