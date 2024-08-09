#include "pch.h"
#include "RBXHooks.h"
#include "LuaApiExtensions.h"
#include "LuaDefs.h"
#include "UrlHelper.h"
#include "Config.h"
#include "Patches.h"

// ===== `CRobloxWnd::RenderRequestJob` member function hooks =====

CRobloxWnd__RenderRequestJob__sleepTime_t CRobloxWnd__RenderRequestJob__sleepTime_orig = reinterpret_cast<CRobloxWnd__RenderRequestJob__sleepTime_t>(Patches::resolveNewVa(0x004881F0));

// unlock fps
double* __fastcall CRobloxWnd__RenderRequestJob__sleepTime_hook(CRobloxWnd__RenderRequestJob* _this, void*, double* a2, int a3)
{
	if (_this->awake)
		RBX::DataModelJob__sleepTime(reinterpret_cast<RBX::DataModelJob*>(_this), a2, a3, Config::desiredFrameRate);
	else
		*a2 = std::numeric_limits<double>::max();

	return a2;
}

// ===== `CRobloxWnd::UserInputJob` member function hooks =====

CRobloxWnd__UserInputJob__sleepTime_t CRobloxWnd__UserInputJob__sleepTime_orig = reinterpret_cast<CRobloxWnd__UserInputJob__sleepTime_t>(Patches::resolveNewVa(0x00486B30));

// unlock fps
double* __fastcall CRobloxWnd__UserInputJob__sleepTime_hook(CRobloxWnd__UserInputJob* _this, void*, double* a2, int a3)
{
	RBX::DataModelJob__sleepTime(reinterpret_cast<RBX::DataModelJob*>(_this), a2, a3, Config::desiredFrameRate);
	return a2;
}

// ===== `RBX::HeartbeatTask` member function hooks ====

RBX::HeartbeatTask__constructor_t RBX::HeartbeatTask__constructor_orig = reinterpret_cast<RBX::HeartbeatTask__constructor_t>(Patches::resolveNewVa(0x00599B40));

// unlock fps
RBX::HeartbeatTask* __fastcall RBX::HeartbeatTask__constructor_hook(RBX::HeartbeatTask* _this, void*, RBX::RunService* runService, void* a3)
{
	auto result = RBX::HeartbeatTask__constructor_orig(_this, runService, a3);
	result->fps = Config::desiredFrameRate;
	return result;
}

// ===== `RBX::PhysicsJob` member function hooks =====

RBX::PhysicsJob__constructor_t RBX::PhysicsJob__constructor_orig = reinterpret_cast<RBX::PhysicsJob__constructor_t>(Patches::resolveNewVa(0x00599180));

// unlock fps
RBX::PhysicsJob* __fastcall RBX::PhysicsJob__constructor_hook(RBX::PhysicsJob* _this, void*, RBX::DataModel* dataModel, void* a3)
{
	auto result = RBX::PhysicsJob__constructor_orig(_this, dataModel, a3);
	if (Config::physicsFpsUnlocked)
		result->fps = Config::desiredFrameRate;
	return result;
}

// ===== `RBX::ContentProvider` member function hooks =====

RBX::ContentProvider__verifyScriptSignature_t RBX::ContentProvider__verifyScriptSignature_orig = reinterpret_cast<RBX::ContentProvider__verifyScriptSignature_t>(Patches::resolveNewVa(0x00654380));

// SECURITY BYPASS
// never require script signatures (1)
void __cdecl RBX::ContentProvider__verifyScriptSignature_hook(const std::string& source, bool required)
{
	RBX::ContentProvider__verifyScriptSignature_orig(source, false);
}

RBX::ContentProvider__verifyRequestedScriptSignature_t RBX::ContentProvider__verifyRequestedScriptSignature_orig = reinterpret_cast<RBX::ContentProvider__verifyRequestedScriptSignature_t>(Patches::resolveNewVa(0x00654B90));

// SECURITY BYPASS
// never require script signatures (2)
void __cdecl RBX::ContentProvider__verifyRequestedScriptSignature_hook(const std::string& source, const std::string& assetId, bool required)
{
	RBX::ContentProvider__verifyRequestedScriptSignature_orig(source, assetId, false);
}

// ===== `RBX::ContentId` member function hooks =====

RBX::ContentId__convertLegacyContent_t RBX::ContentId__convertLegacyContent_orig = reinterpret_cast<RBX::ContentId__convertLegacyContent_t>(Patches::resolveNewVa(0x00653B20));

// asset redirection
// this is executed after ContentId::convertAssetId in ContentProvider::privateLoadContent,
// as well as in other places, making it a good spot to modify the ContentId
void __fastcall RBX::ContentId__convertLegacyContent_hook(RBX::ContentId* _this)
{
	ContentId__convertLegacyContent_orig(_this);

	UrlHelper urlHelper(_this->id);

	if (urlHelper.isAssetUrl())
	{
		std::string query = urlHelper.query;
		std::transform(query.begin(), query.end(), query.begin(), std::tolower);

		static const std::string idParam = "id=";
		size_t idParamPos = query.find(idParam);
		if (idParamPos != std::string::npos)
		{
			size_t idPos = idParamPos + idParam.size();
			size_t idEndPos = query.find('&', idPos);
			if (idEndPos == std::string::npos)
				idEndPos = query.size();

			std::string assetId = query.substr(idPos, idEndPos - idPos);
			if (std::find(Config::assetOverrides.begin(), Config::assetOverrides.end(), assetId) != Config::assetOverrides.end())
			{
				std::string newId = "rbxasset://../extra/assetoverrides/" + assetId;
				(*vc90::std::string__assign)(reinterpret_cast<vc90::std::string*>(&_this->id), newId.c_str());
				return;
			}
		}

		urlHelper.protocol = "https";
		urlHelper.hostname = "assetdelivery.roblox.com";
		urlHelper.path = "v1/asset/";

		(*vc90::std::string__assign)(reinterpret_cast<vc90::std::string*>(&_this->id), urlHelper.buildUrl().c_str());
	}
}

// ===== `RBX::Http` member function hooks =====

RBX::Http__trustCheck_t RBX::Http__trustCheck_orig = reinterpret_cast<RBX::Http__trustCheck_t>(Patches::resolveNewVa(0x005B6300));

// allow content from any url
bool __cdecl RBX::Http__trustCheck_hook(const char* url)
{
	return true;
}

// ===== `RBX::DataModel` member function hooks =====

RBX::DataModel__startCoreScripts_t RBX::DataModel__startCoreScripts_orig = reinterpret_cast<RBX::DataModel__startCoreScripts_t>(Patches::resolveNewVa(0x005F67A0));

// execute a local Studio.ashx
void __fastcall RBX::DataModel__startCoreScripts_hook(RBX::DataModel* _this, void*, RBX::AdornRbxGfx* adorn)
{
	RBX::GuiBuilder guiBuilder;
	RBX::GuiBuilder__buildGui(&guiBuilder, adorn, _this, _this->workspace);

	auto scriptContext = RBX::DataModel__find__ScriptContext(_this);
	RBX::ScriptContext__executeInNewThread_orig(scriptContext, 5, "loadfile('rbxasset://../extra/studio.lua')()", "Studio.ashx");
}

RBX::DataModel__physicsStep_t RBX::DataModel__physicsStep_orig = reinterpret_cast<RBX::DataModel__physicsStep_t>(Patches::resolveNewVa(0x005F8A10));

// unlock fps
void __fastcall RBX::DataModel__physicsStep_hook(RBX::DataModel* _this, void*, float a2, double a3, double a4)
{
	if (Config::physicsFpsUnlocked)
		a2 = 1.0f / Config::desiredFrameRate;

	RBX::DataModel__physicsStep_orig(_this, a2, a3, a4);
}

// ===== `RBX::ScriptContext` member function hooks =====

RBX::ScriptContext__openState_t RBX::ScriptContext__openState_orig = reinterpret_cast<RBX::ScriptContext__openState_t>(Patches::resolveNewVa(0x00625BF0));

// add extensions to the Lua api
void __fastcall RBX::ScriptContext__openState_hook(RBX::ScriptContext* _this)
{
	if (!_this->globalState)
	{
		RBX::ScriptContext__openState_orig(_this);

		// globalState should no longer be null at this point

		if (_this->globalState)
		{
			// patch a script privilege escalation vulnerability
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

			// open our extra libraries
			Lua::openProtectedLibrary(_this->globalState, "maya", Lua::openApiExtensionsLibrary);
			Lua::openProtectedLibrary(_this->globalState, "bit", luaopen_bit);
		}
	}

	// "DefaultWaitTime" is a BoundProp<double,1> so let's just set it here instead of doing anything annoying
	if (Config::use30FpsLuaDefaultWaitTime)
	{
		auto luaSettings = RBX::LuaSettings::singleton();
		if (luaSettings->defaultWaitTime != 1.0 / 30.0)
		{
			luaSettings->defaultWaitTime = 1.0 / 30.0;

			RBX::Instance__raisePropertyChanged(reinterpret_cast<RBX::Instance*>(luaSettings), reinterpret_cast<void*>(Patches::resolveNewVa(0x00CCDA00)));
		}
	}
}

RBX::ScriptContext__executeInNewThread_t RBX::ScriptContext__executeInNewThread_orig = reinterpret_cast<RBX::ScriptContext__executeInNewThread_t>(Patches::resolveNewVa(0x00629A00));

// SECURITY BYPASS
// have "Local url" scripts execute with identity 7 for high privileges
void __fastcall RBX::ScriptContext__executeInNewThread_hook(RBX::ScriptContext* _this, void*, int identity, const char* source, const char* name)
{
	if (std::strcmp(name, "Local url") == 0)
	{
		name = "Local url (elevated)";
		identity = 7;
	}

	RBX::ScriptContext__executeInNewThread_orig(_this, identity, source, name);
}

// ===== `RBX::Network::Replicator::RockyItem` member function hooks =====

RBX::Network::Replicator__RockyItem__write_t RBX::Network::Replicator__RockyItem__write_orig = reinterpret_cast<RBX::Network::Replicator__RockyItem__write_t>(Patches::resolveNewVa(0x004F98A0));

// SECURITY BYPASS
// never send rocky items to the server
bool __fastcall RBX::Network__Replicator__RockyItem__write_hook(RBX::Network::Replicator__RockyItem* _this, void*, void* bitStream)
{
	return true;
}

// ===== `RBX::PlayerChatLine` member function hooks =====

RBX::PlayerChatLine__constructor_t RBX::PlayerChatLine__constructor_orig = reinterpret_cast<RBX::PlayerChatLine__constructor_t>(Patches::resolveNewVa(0x007CC810));

static const std::unordered_map<std::string, std::array<uint8_t, 3>> chatColors = {
	{	"maya",			{	0,	0,	0	} },
	{	"800",			{	111,213,247	} },
	{	"strange",		{	195,129,249	} },
	{	"alex",			{	110,0,	0	} },
};

// custom name colors in chat for friends and myself
RBX::PlayerChatLine* __fastcall RBX::PlayerChatLine__constructor_hook(RBX::PlayerChatLine* _this, void*, int a2, RBX::Player* player, void* a4, int a5, int a6, int a7)
{
	auto result = RBX::PlayerChatLine__constructor_orig(_this, a2, player, a4, a5, a6, a7);

	if (player && player->neutral)
	{
		auto it = chatColors.find(result->name);
		if (it != chatColors.end())
		{
			result->nameR = it->second[0] / 255.0f;
			result->nameG = it->second[1] / 255.0f;
			result->nameB = it->second[2] / 255.0f;
		}
	}

	// i made it, so i get to be special
	if (result->name == "maya")
		(*vc90::std::string__assign)(reinterpret_cast<vc90::std::string*>(&result->name), "(dev) maya");

	return result;
}

// ===== `RBX::NetworkSettings` member function hooks =====

RBX::NetworkSettings__setDataSendRate_t RBX::NetworkSettings__setDataSendRate_orig = reinterpret_cast<RBX::NetworkSettings__setDataSendRate_t>(Patches::resolveNewVa(0x004E42B0));

// rewritten to remove value clamp
void __fastcall RBX::NetworkSettings__setDataSendRate_hook(RBX::NetworkSettings* _this, void*, float dataSendRate)
{
	if (Config::desiredFpsOverridesNetworkDataRates)
		dataSendRate = Config::desiredFrameRate;

	if (_this->dataSendRate != dataSendRate)
	{
		_this->dataSendRate = dataSendRate;

		static const auto ptr_CB7D78 = reinterpret_cast<void*>(Patches::resolveNewVa(0x00CB7D78));
		RBX::Instance__raisePropertyChanged(reinterpret_cast<RBX::Instance*>(_this), ptr_CB7D78);
	}
}

RBX::NetworkSettings__setReceiveRate_t RBX::NetworkSettings__setReceiveRate_orig = reinterpret_cast<RBX::NetworkSettings__setReceiveRate_t>(Patches::resolveNewVa(0x004E4390));

// rewritten to remove value clamp
void __fastcall RBX::NetworkSettings__setReceiveRate_hook(RBX::NetworkSettings* _this, void*, double receiveRate)
{
	if (Config::desiredFpsOverridesNetworkDataRates)
		receiveRate = Config::desiredFrameRate;

	if (_this->receiveRate != receiveRate)
	{
		_this->receiveRate = receiveRate;

		static const auto ptr_CB7BD8 = reinterpret_cast<void*>(Patches::resolveNewVa(0x00CB7BD8));
		RBX::Instance__raisePropertyChanged(reinterpret_cast<RBX::Instance*>(_this), ptr_CB7BD8);
	}
}

RBX::NetworkSettings__setPhysicsSendRate_t RBX::NetworkSettings__setPhysicsSendRate_orig = reinterpret_cast<RBX::NetworkSettings__setPhysicsSendRate_t>(Patches::resolveNewVa(0x004E4320));

// rewritten to remove value clamp
void __fastcall RBX::NetworkSettings__setPhysicsSendRate_hook(RBX::NetworkSettings* _this, void*, float physicsSendRate)
{
	if (Config::desiredFpsOverridesNetworkDataRates && Config::physicsFpsUnlocked)
		physicsSendRate = Config::desiredFrameRate;

	if (_this->physicsSendRate != physicsSendRate)
	{
		_this->physicsSendRate = physicsSendRate;

		static const auto ptr_CB7CAC = reinterpret_cast<void*>(Patches::resolveNewVa(0x00CB7CAC));
		RBX::Instance__raisePropertyChanged(reinterpret_cast<RBX::Instance*>(_this), ptr_CB7CAC);
	}
}

// ===== `RBX::VideoControl` member function hooks =====

RBX::VideoControl__isVideoRecording_t RBX::VideoControl__isVideoRecording_orig = reinterpret_cast<RBX::VideoControl__isVideoRecording_t>(Patches::resolveNewVa(0x0049B030));

bool __fastcall RBX::VideoControl__isVideoRecording_hook(RBX::VideoControl* _this)
{
	// fix null pointer dereference in NoGraphics mode
	return _this && RBX::VideoControl__isVideoRecording_orig(_this);
}

// ===== `RBX::RunService` member function hooks =====

RBX::RunService__step_t RBX::RunService__step_orig = reinterpret_cast<RBX::RunService__step_t>(Patches::resolveNewVa(0x0059BED0));

// unlock fps
void __fastcall RBX::RunService__step_hook(RBX::RunService* _this, void*, double delta)
{
	double elapsedTime = delta + _this->elapsedTime;
	_this->elapsedTime = elapsedTime;

	double a[2];
	a[0] = elapsedTime;

	a[1] = 1.0 / 30.0;
	if (Config::physicsFpsUnlocked)
		a[1] = 1.0 / Config::desiredFrameRate;

	static const auto sub_59B850 = reinterpret_cast<void(__thiscall*)(void*, double*)>(Patches::resolveNewVa(0x0059B850));
	sub_59B850(&_this->heartbeatSignal, a);

	// this is a hack so that the Stepped event gets fired about 30 times per second
	// some places were relying on this event to update the velocity of parts, causing them to
	// accelerate faster than they should
	if (elapsedTime - _this->elapsedTimeAtLastStep >= 1.0 / 30.0)
	{
		_this->elapsedTimeAtLastStep = elapsedTime;

		static const auto sub_59BAC0 = reinterpret_cast<void(__thiscall*)(void*, double, double)>(Patches::resolveNewVa(0x0059BAC0));
		sub_59BAC0(&_this->steppedSignal, elapsedTime, 1.0 / 30.0);
	}
}

// ===== `RBX::RotateConnector` member function hooks =====

RBX::RotateConnector__setRotationalGoal_t RBX::RotateConnector__setRotationalGoal_orig = reinterpret_cast<RBX::RotateConnector__setRotationalGoal_t>(Patches::resolveNewVa(0x0079B310));

// unlock fps
void __fastcall RBX::RotateConnector__setRotationalGoal_hook(RBX::RotateConnector* _this, void*, float goal)
{
	RBX::RotateConnector__setRotationalGoal_orig(_this, goal);

	if (Config::physicsFpsUnlocked)
		_this->increment *= 60.0f / Config::desiredFrameRate;
}

RBX::RotateConnector__setVelocityGoal_t RBX::RotateConnector__setVelocityGoal_orig = reinterpret_cast<RBX::RotateConnector__setVelocityGoal_t>(Patches::resolveNewVa(0x0079B390));

// unlock fps
void __fastcall RBX::RotateConnector__setVelocityGoal_hook(RBX::RotateConnector* _this, void*, float goal)
{
	RBX::RotateConnector__setVelocityGoal_orig(_this, goal);

	if (Config::physicsFpsUnlocked)
		_this->increment *= 60.0f / Config::desiredFrameRate;
}
