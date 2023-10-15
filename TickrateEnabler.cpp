//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose:
//
// $NoKeywords: $
//
//===========================================================================//

#include <stdio.h>
#include <sourcehook/sourcehook_impl.h>
#include <eiface.h>

class TickrateEnabler: public IServerPluginCallbacks
{
public:
	TickrateEnabler() {}
	~TickrateEnabler() {}

	virtual bool Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory);
	virtual void Unload(void);
	virtual void Pause(void) {}
	virtual void UnPause(void) {}
	virtual const char *GetPluginDescription(void)
	{
		return "Tickrate Enabler 0.5, updated, original by Didrole";
	}
	virtual void LevelInit(char const *pMapName) {}
	virtual void ServerActivate(edict_t *pEdictList, int edictCount, int clientMax) {}
	virtual void GameFrame(bool simulating) {}
	virtual void LevelShutdown(void) {}
	virtual void ClientActive(edict_t *pEntity) {}
	virtual void ClientDisconnect(edict_t *pEntity) {}
	virtual void ClientPutInServer(edict_t *pEntity, char const *playername) {}
	virtual void SetCommandClient(int index) {}
	virtual void ClientSettingsChanged(edict_t *pEdict) {}
	virtual PLUGIN_RESULT ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen)
	{
		return PLUGIN_CONTINUE;
	}
	virtual PLUGIN_RESULT ClientCommand(edict_t *pEntity, const CCommand &args)
	{
		return PLUGIN_CONTINUE;
	}
	virtual PLUGIN_RESULT NetworkIDValidated(const char *pszUserName, const char *pszNetworkID)
	{
		return PLUGIN_CONTINUE;
	}
	virtual void OnQueryCvarValueFinished(QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue) {}
	virtual void OnEdictAllocated(edict_t *edict) {}
	virtual void OnEdictFreed(const edict_t *edict) {}
};

IServerGameDLL *server = nullptr;
SourceHook::Impl::CSourceHookImpl g_SourceHook;
SourceHook::ISourceHook *g_SHPtr = &g_SourceHook;
int g_PLID = 0;

TickrateEnabler g_TickrateEnabler;
EXPOSE_SINGLE_INTERFACE_GLOBALVAR(TickrateEnabler, IServerPluginCallbacks, INTERFACEVERSION_ISERVERPLUGINCALLBACKS, g_TickrateEnabler);

SH_DECL_HOOK0(IServerGameDLL, GetTickInterval, const, 0, float);

float GetTickInterval()
{
	float tickinterval = 0.01;
	RETURN_META_VALUE(MRES_SUPERCEDE, tickinterval);
}

bool TickrateEnabler::Load(CreateInterfaceFn interfaceFactory, CreateInterfaceFn gameServerFactory)
{
	server = reinterpret_cast<IServerGameDLL*>(gameServerFactory("ServerGameDLL010", nullptr));

	if (!server)
	{
		Warning("Failed to get a pointer on ServerGameDLL.");
		return false;
	}

	SH_ADD_HOOK(IServerGameDLL, GetTickInterval, server, SH_STATIC(GetTickInterval), false);

	return true;
}

void TickrateEnabler::Unload(void)
{
	SH_REMOVE_HOOK(IServerGameDLL, GetTickInterval, server, SH_STATIC(GetTickInterval), false);
}
