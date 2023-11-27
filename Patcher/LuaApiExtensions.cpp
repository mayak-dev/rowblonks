#include "pch.h"
#include "LuaApiExtensions.h"
#include "RBXDefs.h"
#include "rowblonks.h"

// ===== functionality for global/game chat messages =====

int Lua::produceGameChat(lua_State* L)
{
    RBX::DataModel* dataModel = Lua::getScriptContextAndDataModel(L).second;
    auto networkServer = RBX::DataModel__find__Network__Server(dataModel);

    if (!networkServer)
        luaL_error(L, "A NetworkServer must be active to produce game chat");

    const char* message = luaL_checkstring(L, 1);
   
    auto players = RBX::DataModel__find__Players(dataModel);

    auto messageStr = vc90::std::create_string(message);

    RBX::Players__gameChat(players, messageStr);

    (*vc90::std::string__destructor)(messageStr);

    return 0;
}

// ===== functionality for local corescripts and libraries =====

int Lua::addLocalCoreScript(lua_State* L)
{
    Lua::checkPermissions(L, 2, "add a local CoreScript");

    const char* name = luaL_checkstring(L, 1);
    auto parent = Lua::checkInstance(L, 2);

    std::string source = "rbxasset://../extra/corescripts/";
    source += name;
    source += ".lua";

    auto coreScript = RBX::create_CoreScript(source.c_str());

    // i don't know a good place to put this
    // create a boost::shared_ptr<RBX::CoreScript>
    (reinterpret_cast<void* (__thiscall*)(void*, RBX::CoreScript*, bool)>(0x0061AF40))((*vc90::operator_new)(8), coreScript, false);

    auto nameStr = vc90::std::create_string(name);

    RBX::Instance__setName(reinterpret_cast<RBX::Instance*>(coreScript), nameStr);

    (*vc90::std::string__destructor)(nameStr);

    RBX::Instance__setRobloxLocked(reinterpret_cast<RBX::Instance*>(coreScript), true);

    auto scriptContext = Lua::getScriptContextAndDataModel(L).first;

    RBX::Instance__setParent(reinterpret_cast<RBX::Instance*>(coreScript), parent);

    if (parent != reinterpret_cast<RBX::Instance*>(scriptContext))
        RBX::ScriptContext__addScript(scriptContext, coreScript);

    return 0;
}

int Lua::addLocalStarterScript(lua_State* L)
{
    Lua::checkPermissions(L, 2, "add a local StarterScript");

    const char* name = luaL_checkstring(L, 1);
    auto parent = Lua::checkInstance(L, 2);

    std::string source = "rbxasset://../extra/corescripts/";
    source += name;
    source += ".lua";

    auto starterScript = RBX::create_StarterScript(source.c_str());

    // i don't know a good place to put this
    // create a boost::shared_ptr<RBX::StarterScript>
    (reinterpret_cast<void* (__thiscall*)(void*, RBX::StarterScript*, bool)>(0x0061AE90))((*vc90::operator_new)(8), starterScript, false);

    auto nameStr = vc90::std::create_string(name);

    RBX::Instance__setName(reinterpret_cast<RBX::Instance*>(starterScript), nameStr);

    (*vc90::std::string__destructor)(nameStr);

    RBX::Instance__setRobloxLocked(reinterpret_cast<RBX::Instance*>(starterScript), true);

    RBX::ScriptContext* scriptContext = Lua::getScriptContextAndDataModel(L).first;

    RBX::Instance__setParent(reinterpret_cast<RBX::Instance*>(starterScript), reinterpret_cast<RBX::Instance*>(scriptContext));

    return 0;
}

int Lua::registerLocalLibrary(lua_State* L)
{
    const char* name = luaL_checkstring(L, 1);

    if (strncmp(name, "Rbx", 3) == 0)
        Lua::checkPermissions(L, 2, "register a local Rbx library");

    auto script = RBX::create_Script();

    // i don't know a good place to put this
    // create a boost::shared_ptr<RBX::Script>
    (reinterpret_cast<void* (__thiscall*)(void*, RBX::Script*, bool)>(0x00428EE0))((*vc90::operator_new)(8), script, false);

    std::stringstream sourceStream;
    
    // script objects can only be registered as a library if their source property is set
    // so we have to read the library file ourselves and set the property
    std::string path = g_gamePath + "/extra/libraries/";
    path += name;
    path += ".lua";

    std::ifstream sourceFile(path);
    sourceStream << sourceFile.rdbuf();
    sourceFile.close();

    auto nameStr = vc90::std::create_string(name);
    auto sourceStr = vc90::std::create_string(sourceStream.str().c_str());

    RBX::Instance__setName(reinterpret_cast<RBX::Instance*>(script), nameStr);

    RBX::Script__setDisabled(script, true);
    RBX::Script__setSource(script, sourceStr);

    RBX::Instance__setRobloxLocked(reinterpret_cast<RBX::Instance*>(script), true);

    RBX::ScriptContext* scriptContext = Lua::getScriptContextAndDataModel(L).first;
    
    RBX::Instance__setParent(reinterpret_cast<RBX::Instance*>(script), reinterpret_cast<RBX::Instance*>(scriptContext));

    // not fully sure what this is, but this is done to register the script object as a library
    auto ptr = *(reinterpret_cast<DWORD**>(scriptContext) + 129) + 8;
    auto res = (reinterpret_cast<RBX::Script** (__thiscall*)(void*, vc90::std::string*)>(0x006145A0))(ptr, nameStr);
    *res = script;

    (*vc90::std::string__destructor)(nameStr);
    (*vc90::std::string__destructor)(sourceStr);

    return 0;
}
