#include "pch.h"
#include "Patches.h"
#include "RBXHooks.h"

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

void Patches::initialize()
{
    initializeHooks();
}
