#include "pch.h"
#include "RBXHooks.h"
#include "LuaApiExtensions.h"
#include "LuaDefs.h"
#include "UrlHelper.h"
#include "Config.h"

// ===== `CRobloxWnd::RenderRequestJob` member function hooks =====

CRobloxWnd__RenderRequestJob__sleepTime_t CRobloxWnd__RenderRequestJob__sleepTime = reinterpret_cast<CRobloxWnd__RenderRequestJob__sleepTime_t>(0x004881F0);

// unlock fps (1)
double* __fastcall hook_CRobloxWnd__RenderRequestJob__sleepTime(CRobloxWnd__RenderRequestJob* _this, void*, double* a2, int a3)
{
	if (_this->awake)
		(reinterpret_cast<void(__thiscall*)(CRobloxWnd__RenderRequestJob*, double*, int, double)>(0x007FDDB0))(_this, a2, a3, Config::desiredFrameRate);
	else
		*a2 = std::numeric_limits<double>::max();

	return a2;
}

// ===== `CRobloxWnd::UserInputJob` member function hooks =====

CRobloxWnd__UserInputJob__sleepTime_t CRobloxWnd__UserInputJob__sleepTime = reinterpret_cast<CRobloxWnd__UserInputJob__sleepTime_t>(0x00486B30);

// unlock fps (2)
double* __fastcall hook_CRobloxWnd__UserInputJob__sleepTime(CRobloxWnd__UserInputJob* _this, void*, double* a2, int a3)
{
	(reinterpret_cast<void(__thiscall*)(CRobloxWnd__UserInputJob*, double*, int, double)>(0x007FDDB0))(_this, a2, a3, Config::desiredFrameRate);
	return a2;
}

// ===== `RBX::HeartbeatTask` member function hooks ====

RBX::HeartbeatTask__constructor_t RBX::HeartbeatTask__constructor = reinterpret_cast<RBX::HeartbeatTask__constructor_t>(0x00599B40);

// unlock fps (3)
RBX::HeartbeatTask* __fastcall hook_RBX__HeartbeatTask__constructor(RBX::HeartbeatTask* _this, void*, RBX::RunService* runService, void* a3)
{
	auto result = RBX::HeartbeatTask__constructor(_this, runService, a3);
	result->fps = Config::desiredFrameRate;
	return result;
}

// ===== `RBX::ContentProvider` member function hooks =====

RBX::ContentProvider__verifyScriptSignature_t RBX::ContentProvider__verifyScriptSignature = reinterpret_cast<RBX::ContentProvider__verifyScriptSignature_t>(0x00654380);

// SECURITY BYPASS
// never require script signatures (1)
void __cdecl hook_RBX__ContentProvider__verifyScriptSignature(vc90::std::string* source, bool required)
{
	RBX::ContentProvider__verifyScriptSignature(source, false);
}

RBX::ContentProvider__verifyRequestedScriptSignature_t RBX::ContentProvider__verifyRequestedScriptSignature = reinterpret_cast<RBX::ContentProvider__verifyRequestedScriptSignature_t>(0x00654B90);

// SECURITY BYPASS
// never require script signatures (2)
void __cdecl RBX__ContentProvider__verifyRequestedScriptSignature_hook(vc90::std::string* source, vc90::std::string* assetId, bool required)
{
	RBX::ContentProvider__verifyRequestedScriptSignature(source, assetId, false);
}

// ===== `RBX:Http` member function hooks =====

RBX::Http__constructor_t RBX::Http__constructor = reinterpret_cast<RBX::Http__constructor_t>(0x00420090);

// reconstruct asset urls to use the assetdelivery api
RBX::Http* __fastcall hook_RBX__Http__constructor(RBX::Http* _this, void*, vc90::std::string* url)
{
	const char* urlCStr = (*vc90::std::string__c_str)(url);

	UrlHelper urlHelper(urlCStr);

	std::string& hostname = urlHelper.hostname;
	std::string& path = urlHelper.path;

	std::transform(path.begin(), path.end(), path.begin(), std::tolower);

	if ((hostname == "roblox.com" || hostname == "www.roblox.com" || hostname == "assetgame.roblox.com") &&
		(path == "asset" || path == "asset/"))
	{
		urlHelper.protocol = "https";
		urlHelper.hostname = "assetdelivery.roblox.com";
		urlHelper.path = "v1/asset/";

		std::string newUrlStr = urlHelper.buildUrl();

		auto newUrl = vc90::std::string::construct(newUrlStr.c_str());

		auto result = RBX::Http__constructor(_this, newUrl);

		vc90::std::string::destruct(newUrl);

		return result;
	}

	return RBX::Http__constructor(_this, url);
}

RBX::Http__trustCheck_t RBX::Http__trustCheck = reinterpret_cast<RBX::Http__trustCheck_t>(0x005B6300);

// allow content from any url
bool __cdecl hook_RBX__Http__trustCheck(const char* url)
{
	return true;
}

// ===== `RBX::DataModel` member function hooks =====

RBX::DataModel__startCoreScripts_t RBX::DataModel__startCoreScripts = reinterpret_cast<RBX::DataModel__startCoreScripts_t>(0x005F67A0);

// execute a local Studio.ashx
void __fastcall hook_RBX__DataModel__startCoreScripts(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn)
{
	RBX::GuiBuilder guiBuilder;
	RBX::GuiBuilder__buildGui(&guiBuilder, adorn, _this, _this->workspace);

	auto scriptContext = RBX::DataModel__find__ScriptContext(_this);
	RBX::ScriptContext__executeInNewThread(scriptContext, 5, "loadfile('rbxasset://../extra/studio.lua')()", "Studio.ashx");
}

// ===== `RBX::ScriptContext` member function hooks =====

RBX::ScriptContext__openState_t RBX::ScriptContext__openState = reinterpret_cast<RBX::ScriptContext__openState_t>(0x00625BF0);

// add extensions to the Lua api
void __fastcall hook_RBX__ScriptContext__openState(RBX::ScriptContext* _this)
{
	if (!_this->globalState)
	{
		RBX::ScriptContext__openState(_this);

		// globalState should no longer be null at this point

		if (_this->globalState)
		{
			Lua::protectLibrary(_this->globalState, "CFrame");
			Lua::protectLibrary(_this->globalState, "Region3");
			Lua::protectLibrary(_this->globalState, "Vector3");
			Lua::protectLibrary(_this->globalState, "Vector2");
			Lua::protectLibrary(_this->globalState, "Ray");
			Lua::protectLibrary(_this->globalState, "Color3");
			Lua::protectLibrary(_this->globalState, "BrickColor");
			Lua::protectLibrary(_this->globalState, "UDim");
			Lua::protectLibrary(_this->globalState, "UDim2");
			Lua::protectLibrary(_this->globalState, "Faces");
			Lua::protectLibrary(_this->globalState, "Axes");
			Lua::protectLibrary(_this->globalState, "Instance");

			Lua::openProtectedLibrary(_this->globalState, "maya", Lua::openApiExtensionsLibrary);
		}
	}
}

// ===== `RBX::Network::Replicator::RockyItem` member function hooks =====

RBX::Network::Replicator__RockyItem__write_t RBX::Network::Replicator__RockyItem__write = reinterpret_cast<RBX::Network::Replicator__RockyItem__write_t>(0x004F98A0);

// SECURITY BYPASS
// never send rocky items to the server
bool __fastcall hook_RBX__Network__Replicator__RockyItem__write(RBX::Network::Replicator__RockyItem* _this, void*, void* bitStream)
{
	return true;
}
