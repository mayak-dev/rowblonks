#include "pch.h"
#include "RBXHooks.h"
#include "LuaApiExtensions.h"

// ===== `RBX::ContentProvider` member function hooks =====

RBX::ContentProvider__verifyScriptSignature_t RBX::ContentProvider__verifyScriptSignature = reinterpret_cast<RBX::ContentProvider__verifyScriptSignature_t>(0x00654380);

void __cdecl RBX__ContentProvider__verifyScriptSignature_hook(vc90::std::string* source, bool required)
{
	RBX::ContentProvider__verifyScriptSignature(source, false);
}

RBX::ContentProvider__verifyRequestedScriptSignature_t RBX::ContentProvider__verifyRequestedScriptSignature = reinterpret_cast<RBX::ContentProvider__verifyRequestedScriptSignature_t>(0x00654B90);

void __cdecl RBX__ContentProvider__verifyRequestedScriptSignature_hook(vc90::std::string* source, vc90::std::string* assetId, bool required)
{
	RBX::ContentProvider__verifyRequestedScriptSignature(source, assetId, false);
}

// ===== `RBX:Http` member function hooks =====

RBX::Http__constructor_t RBX::Http__constructor = reinterpret_cast<RBX::Http__constructor_t>(0x00420090);

RBX::Http* __fastcall RBX__Http__constructor_hook(RBX::Http* _this, void*, vc90::std::string* url)
{
	// TODO: PARSE URL AND MODIFY TO USE NEW ASSET API

	return RBX::Http__constructor(_this, url);
}

RBX::Http__trustCheck_t RBX::Http__trustCheck = reinterpret_cast<RBX::Http__trustCheck_t>(0x005B6300);

bool __cdecl RBX__Http__trustCheck_hook(const char* url)
{
	return true;
}

// ===== `RBX::DataModel` member function hooks =====

RBX::DataModel__startCoreScripts_t RBX::DataModel__startCoreScripts = reinterpret_cast<RBX::DataModel__startCoreScripts_t>(0x005F67A0);

void __fastcall RBX__DataModel__startCoreScripts_hook(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn)
{
	RBX::GuiBuilder guiBuilder;
	RBX::GuiBuilder__buildGui(&guiBuilder, adorn, _this, _this->workspace);

	auto scriptContext = RBX::DataModel__find__ScriptContext(_this);
	RBX::ScriptContext__executeInNewThread(scriptContext, 5, "loadfile('rbxasset://../extra/studio.lua')()", "Studio.ashx");
}

// ===== `RBX::ScriptContext` member function hooks =====

RBX::ScriptContext__openState_t RBX::ScriptContext__openState = reinterpret_cast<RBX::ScriptContext__openState_t>(0x00625BF0);

void __fastcall RBX__ScriptContext__openState_hook(RBX::ScriptContext* _this)
{
	if (!_this->globalState)
	{
		RBX::ScriptContext__openState(_this);

		// globalState should no longer be null at this point

		if (_this->globalState)
		{
			lua_pushcfunction(_this->globalState, Lua::addLocalCoreScript);
			lua_setglobal(_this->globalState, "AddLocalCoreScript");

			lua_pushcfunction(_this->globalState, Lua::registerLocalLibrary);
			lua_setglobal(_this->globalState, "RegisterLocalLibrary");
		}
	}
}
