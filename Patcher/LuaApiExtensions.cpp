#include "pch.h"
#include "LuaApiExtensions.h"
#include "LuaDefs.h"
#include "RBXDefs.h"
#include "Config.h"
#include "Patches.h"

static const luaL_Reg mayaLib[] = {
    { "GetIdentity", Lua::Api::getIdentity },
    { "SpoofIdentity", Lua::Api::spoofIdentity },

    { "ProduceGameChat", Lua::Api::produceGameChat },

    { "AddLocalCoreScript", Lua::Api::addLocalCoreScript },
    { "AddLocalStarterScript", Lua::Api::addLocalStarterScript },
    { "RegisterLocalLibrary", Lua::Api::registerLocalLibrary },

    { "ShowIDEWindow", Lua::Api::showIdeWindow },

    { nullptr, nullptr },
};

int Lua::openApiExtensionsLibrary(lua_State* L)
{
    // make `warn` a global function for compatibility
    lua_pushcfunction(L, Lua::Api::warn);
    lua_setglobal(L, "warn");

    // register other API extensions
    luaL_register(L, "maya", mayaLib);
    return 1;
}

// ===== functionality to output warnings =====

int Lua::Api::warn(lua_State* L)
{
    std::string result;

    // replicate the same behavior as the `print` function, which comes from the Lua source

    int n = lua_gettop(L);  /* number of arguments */

    lua_getglobal(L, "tostring");
    
    for (int i = 1; i <= n; i++) {
        lua_pushvalue(L, -1);  /* function to be called */
        lua_pushvalue(L, i);   /* value to print */
        lua_call(L, 1, 1);

        const char* s = lua_tostring(L, -1);  /* get result */
        if (!s)
            return luaL_error(L, "'tostring' must return a string to 'warn'");
        
        if (i > 1)
            result += ' ';
        result += s;

        lua_pop(L, 1);  /* pop result */
    }

    // now that the string is built, output the warning
    auto standardOut = RBX::StandardOut::singleton();
    RBX::StandardOut__print(standardOut, RBX::MESSAGE_WARNING, result);

    return 0;
}

// ===== functionality related to script identities =====

int Lua::Api::getIdentity(lua_State* L)
{
    int identity = RobloxExtraSpace::get(L)->identity;
    lua_pushinteger(L, identity);
    return 1;
}

// requires script to yield and resume to take effect
int Lua::Api::spoofIdentity(lua_State* L)
{
    if (!Config::allowScriptIdentitySpoofing)
        luaL_error(L, "Script identity spoofing is disabled");

    int identity = luaL_checkint(L, 1);
    RobloxExtraSpace::get(L)->identity = identity;
    return 0;
}

// ===== functionality for global/game chat messages =====

int Lua::Api::produceGameChat(lua_State* L)
{
    std::string message = luaL_checkstring(L, 1);

    RBX::DataModel* dataModel = Lua::getScriptContextAndDataModel(L).second;

    auto networkServer = RBX::DataModel__find__Network__Server(dataModel);
    if (!networkServer)
        luaL_error(L, "A NetworkServer must be active to produce game chat");
   
    auto players = RBX::DataModel__find__Players(dataModel);
    RBX::Players__gameChat(players, message);

    return 0;
}

// ===== functionality for local corescripts and libraries =====

static const auto sub_61AF40 = reinterpret_cast<void* (__thiscall*)(void*, RBX::CoreScript*, bool)>(Patches::resolveNewVa(0x0061AF40));

int Lua::Api::addLocalCoreScript(lua_State* L)
{
    Lua::checkPermissions(L, 2, "add a local CoreScript");

    std::string name = luaL_checkstring(L, 1);
    auto parent = Lua::checkInstance(L, 2);

    std::string source = "rbxasset://../extra/corescripts/" + name + ".lua";

    auto coreScript = RBX::CoreScript::construct(source);

    // i don't know a good place to put this
    // create a boost::shared_ptr<RBX::CoreScript>
    sub_61AF40((*vc90::operator_new)(8), coreScript, false);

    RBX::Instance__setRobloxLocked(reinterpret_cast<RBX::Instance*>(coreScript), true);
    RBX::Instance__setName(reinterpret_cast<RBX::Instance*>(coreScript), name);
    RBX::Instance__setParent(reinterpret_cast<RBX::Instance*>(coreScript), parent);

    RBX::ScriptContext* scriptContext = Lua::getScriptContextAndDataModel(L).first;
    if (parent != reinterpret_cast<RBX::Instance*>(scriptContext))
        RBX::ScriptContext__addScript(scriptContext, coreScript);

    return 0;
}

static const auto sub_61AE90 = reinterpret_cast<void* (__thiscall*)(void*, RBX::StarterScript*, bool)>(Patches::resolveNewVa(0x0061AE90));

int Lua::Api::addLocalStarterScript(lua_State* L)
{
    Lua::checkPermissions(L, 2, "add a local StarterScript");

    std::string name = luaL_checkstring(L, 1);

    std::string source = "rbxasset://../extra/corescripts/" + name + ".lua";

    auto starterScript = RBX::StarterScript::construct(source);

    // i don't know a good place to put this
    // create a boost::shared_ptr<RBX::StarterScript>
    sub_61AE90((*vc90::operator_new)(8), starterScript, false);

    RBX::Instance__setRobloxLocked(reinterpret_cast<RBX::Instance*>(starterScript), true);
    RBX::Instance__setName(reinterpret_cast<RBX::Instance*>(starterScript), name);

    RBX::ScriptContext* scriptContext = Lua::getScriptContextAndDataModel(L).first;
    RBX::Instance__setParent(reinterpret_cast<RBX::Instance*>(starterScript), reinterpret_cast<RBX::Instance*>(scriptContext));

    return 0;
}

static const auto sub_428EE0 = reinterpret_cast<void* (__thiscall*)(void*, RBX::Script*, bool)>(Patches::resolveNewVa(0x00428EE0));
static const auto sub_6145A0 = reinterpret_cast<RBX::Script** (__thiscall*)(void*, const std::string&)>(Patches::resolveNewVa(0x006145A0));

int Lua::Api::registerLocalLibrary(lua_State* L)
{
    std::string name = luaL_checkstring(L, 1);

    if (name.substr(0, 3) == "Rbx")
        Lua::checkPermissions(L, 2, "register a local Rbx library");

    auto script = RBX::Script::construct();

    // i don't know a good place to put this
    // create a boost::shared_ptr<RBX::Script>
    sub_428EE0((*vc90::operator_new)(8), script, false);

    std::stringstream sourceStream;
    
    // script objects can only be registered as a library if their source property is set
    // so we have to read the library file ourselves and set the property
    std::string path = Config::gameDirectory + "/extra/libraries/" + name + ".lua";

    std::ifstream sourceFile(path);
    sourceStream << sourceFile.rdbuf();
    sourceFile.close();

    RBX::Instance__setRobloxLocked(reinterpret_cast<RBX::Instance*>(script), true);
    RBX::Instance__setName(reinterpret_cast<RBX::Instance*>(script), name);
    RBX::Script__setDisabled(script, true);
    RBX::Script__setSource(script, sourceStream.str());

    RBX::ScriptContext* scriptContext = Lua::getScriptContextAndDataModel(L).first;
    RBX::Instance__setParent(reinterpret_cast<RBX::Instance*>(script), reinterpret_cast<RBX::Instance*>(scriptContext));

    // not fully sure what this is (probably adding to a map?), but this is done to register the script object as a library
    // this is NOT checking if a library was already registered with the given name
    auto res = sub_6145A0(*(reinterpret_cast<uint32_t**>(scriptContext) + 129) + 8, name);
    *res = script;

    return 0;
}

// ===== functionality to show an IDE window =====

int Lua::Api::showIdeWindow(lua_State* L)
{
    Lua::checkPermissions(L, 3, "show an IDE window");

    std::string name = luaL_checkstring(L, 1);

    auto mainFrame = g_CRobloxApp->m_pActiveWnd;

    if (name == "Output")
        CMainFrame__ShowOutput(mainFrame);
    else if (name == "Task Scheduler")
        CMainFrame__ShowTaskScheduler(mainFrame);
    else
        luaL_error(L, "Invalid IDE window name");

    return 0;
}
