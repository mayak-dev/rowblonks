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

RBX::DataModel* Lua::getDataModel(lua_State* L)
{
    auto scriptContext = RobloxExtraSpace::get(L)->shared->scriptContext;
    auto dataModel = reinterpret_cast<RBX::DataModel*>(reinterpret_cast<RBX::Instance*>(scriptContext)->parent);
    return dataModel;
}

void Lua::checkIdentity(lua_State* L, int minIdentity, const char* action)
{
    int identity = RobloxExtraSpace::get(L)->identity;

    if (identity < minIdentity)
        luaL_error(L, "Insufficient permissions to %s (expected identity >= %d, got %d)", action, minIdentity, identity);
}
