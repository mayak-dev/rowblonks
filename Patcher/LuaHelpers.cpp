#include "pch.h"
#include "LuaHelpers.h"
#include "RBXDefs.h"

RBX::Instance* Lua::checkInstance(lua_State* L, int n)
{
    // userdata should be a boost::shared_ptr<RBX::Reflection::DescribedBase>
    // RBX::Reflection::DescribedBase* should be at sharedPtr[0]
    auto ptr =  *reinterpret_cast<void**>(luaL_checkudata(L, n, "Object"));

    void* const describedBaseTypeDesc = reinterpret_cast<void*>(0x00C073F8);
    void* const instanceTypeDesc = reinterpret_cast<void*>(0x00C071F8);

    auto result = reinterpret_cast<RBX::Instance*>((*vc90::_RTDynamicCast)(ptr, 0, describedBaseTypeDesc, instanceTypeDesc, FALSE));
    return result;
}

std::pair<RBX::ScriptContext*, RBX::DataModel*> Lua::getScriptContextAndDataModel(lua_State* L)
{
    auto scriptContext = RobloxExtraSpace::get(L)->shared->scriptContext;
    auto dataModel = reinterpret_cast<RBX::DataModel*>(reinterpret_cast<RBX::Instance*>(scriptContext)->parent);
    return std::make_pair(scriptContext, dataModel);
}

// it would be a good idea to turn identities and permission roles into enums at some point
void Lua::checkPermissions(lua_State* L, int role, const char* action)
{
    int identity = RobloxExtraSpace::get(L)->identity;

    switch (identity)
    {
    case 0:
    case 2:
        if (role == 0)
            return;

        break;
    case 1:
    case 5:
        if (role == 0 || role == 1 || role == 2 || role == 3)
            return;

        break;
    case 3:
        if (role == 0 || role == 1)
            return;

        break;
    case 4:
        if (role == 0 || role == 1 || role == 2)
            return;

        break;
    case 6:
    case 7:
        return;
    case 8:
        if (role == 0 || role == 1 || role == 2 || role == 4)
            return;

        break;
    }

    luaL_error(L, "Insufficient permissions to %s. Identity %d does not have permission role %d.", action, identity, role);
}
