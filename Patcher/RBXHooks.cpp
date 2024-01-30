#include "pch.h"
#include "RBXHooks.h"
#include "LuaApiExtensions.h"
#include "LuaDefs.h"
#include "UrlHelper.h"
#include "Config.h"

// ===== `CRobloxWnd::RenderRequestJob` member function hooks =====

CRobloxWnd__RenderRequestJob__sleepTime_t CRobloxWnd__RenderRequestJob__sleepTime_orig = reinterpret_cast<CRobloxWnd__RenderRequestJob__sleepTime_t>(0x004881F0);

// unlock fps (1)
double* __fastcall CRobloxWnd__RenderRequestJob__sleepTime_hook(CRobloxWnd__RenderRequestJob* _this, void*, double* a2, int a3)
{
	if (_this->awake)
		(reinterpret_cast<void(__thiscall*)(CRobloxWnd__RenderRequestJob*, double*, int, double)>(0x007FDDB0))(_this, a2, a3, Config::desiredFrameRate);
	else
		*a2 = std::numeric_limits<double>::max();

	return a2;
}

// ===== `CRobloxWnd::UserInputJob` member function hooks =====

CRobloxWnd__UserInputJob__sleepTime_t CRobloxWnd__UserInputJob__sleepTime_orig = reinterpret_cast<CRobloxWnd__UserInputJob__sleepTime_t>(0x00486B30);

// unlock fps (2)
double* __fastcall CRobloxWnd__UserInputJob__sleepTime_hook(CRobloxWnd__UserInputJob* _this, void*, double* a2, int a3)
{
	(reinterpret_cast<void(__thiscall*)(CRobloxWnd__UserInputJob*, double*, int, double)>(0x007FDDB0))(_this, a2, a3, Config::desiredFrameRate);
	return a2;
}

// ===== `RBX::HeartbeatTask` member function hooks ====

RBX::HeartbeatTask__constructor_t RBX::HeartbeatTask__constructor_orig = reinterpret_cast<RBX::HeartbeatTask__constructor_t>(0x00599B40);

// unlock fps (3)
RBX::HeartbeatTask* __fastcall RBX::HeartbeatTask__constructor_hook(RBX::HeartbeatTask* _this, void*, RBX::RunService* runService, void* a3)
{
	auto result = RBX::HeartbeatTask__constructor_orig(_this, runService, a3);
	result->fps = Config::desiredFrameRate;
	return result;
}

// ===== `RBX::ContentProvider` member function hooks =====

RBX::ContentProvider__verifyScriptSignature_t RBX::ContentProvider__verifyScriptSignature_orig = reinterpret_cast<RBX::ContentProvider__verifyScriptSignature_t>(0x00654380);

// SECURITY BYPASS
// never require script signatures (1)
void __cdecl RBX::ContentProvider__verifyScriptSignature_hook(vc90::std::string* source, bool required)
{
	RBX::ContentProvider__verifyScriptSignature_orig(source, false);
}

RBX::ContentProvider__verifyRequestedScriptSignature_t RBX::ContentProvider__verifyRequestedScriptSignature_orig = reinterpret_cast<RBX::ContentProvider__verifyRequestedScriptSignature_t>(0x00654B90);

// SECURITY BYPASS
// never require script signatures (2)
void __cdecl RBX::ContentProvider__verifyRequestedScriptSignature_hook(vc90::std::string* source, vc90::std::string* assetId, bool required)
{
	RBX::ContentProvider__verifyRequestedScriptSignature_orig(source, assetId, false);
}

// ===== `RBX:Http` member function hooks =====

RBX::Http__constructor_t RBX::Http__constructor_orig = reinterpret_cast<RBX::Http__constructor_t>(0x00420090);

// reconstruct asset urls to use the assetdelivery api
RBX::Http* __fastcall RBX::Http__constructor_hook(RBX::Http* _this, void*, vc90::std::string* url)
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

		auto result = RBX::Http__constructor_orig(_this, newUrl);

		vc90::std::string::destruct(newUrl);

		return result;
	}

	return RBX::Http__constructor_orig(_this, url);
}

RBX::Http__trustCheck_t RBX::Http__trustCheck_orig = reinterpret_cast<RBX::Http__trustCheck_t>(0x005B6300);

// allow content from any url
bool __cdecl RBX::Http__trustCheck_hook(const char* url)
{
	return true;
}

// ===== `RBX::DataModel` member function hooks =====

RBX::DataModel__startCoreScripts_t RBX::DataModel__startCoreScripts_orig = reinterpret_cast<RBX::DataModel__startCoreScripts_t>(0x005F67A0);

// execute a local Studio.ashx
void __fastcall RBX::DataModel__startCoreScripts_hook(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn)
{
	RBX::GuiBuilder guiBuilder;
	RBX::GuiBuilder__buildGui(&guiBuilder, adorn, _this, _this->workspace);

	auto scriptContext = RBX::DataModel__find__ScriptContext(_this);
	RBX::ScriptContext__executeInNewThread(scriptContext, 5, "loadfile('rbxasset://../extra/studio.lua')()", "Studio.ashx");
}

// ===== `RBX::ScriptContext` member function hooks =====

RBX::ScriptContext__openState_t RBX::ScriptContext__openState_orig = reinterpret_cast<RBX::ScriptContext__openState_t>(0x00625BF0);

// add extensions to the Lua api
void __fastcall RBX::ScriptContext__openState_hook(RBX::ScriptContext* _this)
{
	if (!_this->globalState)
	{
		RBX::ScriptContext__openState_orig(_this);

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
			Lua::openProtectedLibrary(_this->globalState, "bit", luaopen_bit);
		}
	}
}

// ===== `RBX::Network::Replicator::RockyItem` member function hooks =====

RBX::Network::Replicator__RockyItem__write_t RBX::Network::Replicator__RockyItem__write_orig = reinterpret_cast<RBX::Network::Replicator__RockyItem__write_t>(0x004F98A0);

// SECURITY BYPASS
// never send rocky items to the server
bool __fastcall RBX::Network__Replicator__RockyItem__write_hook(RBX::Network::Replicator__RockyItem* _this, void*, void* bitStream)
{
	return true;
}

// ===== `RBX::PlayerChatLine` member function hooks =====

RBX::PlayerChatLine__constructor_t RBX::PlayerChatLine__constructor_orig = reinterpret_cast<RBX::PlayerChatLine__constructor_t>(0x007CC810);

const std::map<std::string, std::vector<uint8_t>> chatColors = {
	{	"maya",			{	0,	0,	0	} },
	{	"800",			{	111,213,247	} },
	{	"strange",		{	195,129,249	} },
	{	"alex",			{	110,0,	0	} },
};

RBX::PlayerChatLine* __fastcall RBX::PlayerChatLine__constructor_hook(RBX::PlayerChatLine* _this, void*, int a2, RBX::Player* player, void* a4, int a5, int a6, int a7)
{
	auto result = RBX::PlayerChatLine__constructor_orig(_this, a2, player, a4, a5, a6, a7);

	auto nameStr = reinterpret_cast<vc90::std::string*>(&result->name);
	const char* name = (*vc90::std::string__c_str)(nameStr);

	if (player->neutral)
	{
		auto it = chatColors.find(name);
		if (it != chatColors.end())
		{
			result->nameR = it->second[0] / 255.0f;
			result->nameG = it->second[1] / 255.0f;
			result->nameB = it->second[2] / 255.0f;
		}
	}

	if (std::strcmp(name, "maya") == 0)
		(*vc90::std::string__assign_from_c_str)(nameStr, "(dev) maya");

	return result;
}
