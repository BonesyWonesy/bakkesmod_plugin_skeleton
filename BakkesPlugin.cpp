/**




*/
#include "BakkesPlugin.h"

BAKKESMOD_PLUGIN(BakkesPlugin, "BagOBones", "1.0", PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING)

BakkesPlugin::BakkesPlugin() {
	m_loaded = false;
	m_registeredRender = false;
}

BakkesPlugin::~BakkesPlugin() {
}

void BakkesPlugin::onLoad() {
	m_debugLogs.Log(std::string("BagOBones Plugin Loaded"));

	m_pluginEnabled = make_shared<bool>(true);
	cvarManager->registerCvar("cl_bones_plugin_enabled", "0", "Enable plugin", true, true, 0.f, true, 1.f, true).bindTo(m_pluginEnabled);
	cvarManager->getCvar("cl_bones_plugin_enabled").addOnValueChanged(std::bind(&BakkesPlugin::OnPluginEnabledValueChanged, this, std::placeholders::_1, std::placeholders::_2));

	// Set DebugLogs cvars
	m_debugLogsEnabled = make_shared<bool>(true);
	cvarManager->registerCvar("cl_bones_plugin_enableDebugLogs", "0", "Enable debug logs", true, true, 0, true, 1).bindTo(m_debugLogsEnabled);
	cvarManager->getCvar("cl_bones_plugin_enableDebugLogs").addOnValueChanged(std::bind(&BakkesPlugin::OnDebugLogsEnabledValueChanged, this, std::placeholders::_1, std::placeholders::_2));

	m_debugLogPosX = make_shared<int>(0);
	m_debugLogPosY = make_shared<int>(0);
	cvarManager->registerCvar("cl_bones_plugin_debuglog_posx", "0", "Debug Log Position X", true, true, 0, true, 10000).bindTo(m_debugLogPosX);
	cvarManager->registerCvar("cl_bones_plugin_debuglog_posy", "0", "Debug Log Position Y", true, true, 0, true, 10000).bindTo(m_debugLogPosY);

	m_jugglerEnabled = make_shared<bool>(true);
	cvarManager->registerCvar("cl_bones_plugin_enableJuggler", "1", "Enable Juggler information", true, true, 0, true, 1).bindTo(m_jugglerEnabled);
	cvarManager->getCvar("cl_bones_plugin_enableJuggler").addOnValueChanged(std::bind(&BakkesPlugin::OnJugglerEnabledValueChanged, this, std::placeholders::_1, std::placeholders::_2));

	m_jugglerPosX = make_shared<int>(0);
	m_jugglerPosY = make_shared<int>(0);
	cvarManager->registerCvar("cl_bones_plugin_juggler_posx", "0", "Juggler Position X", true, true, 0, true, 10000).bindTo(m_jugglerPosX);
	cvarManager->registerCvar("cl_bones_plugin_juggler_posy", "0", "Juggler Position Y", true, true, 0, true, 10000).bindTo(m_jugglerPosY);

	cvarManager->registerNotifier("resetHitCount", [this](std::vector<string> commands) {
		m_juggler.ResetHighestHits();
	}, "", 0);

	cvarManager->registerNotifier("resetAirTime", [this](std::vector<string> commands) {
		m_juggler.ResetHighestAirTime();
	}, "", 0);

	m_hitboxEnabled = make_shared<bool>(true);
	cvarManager->registerCvar("cl_bones_plugin_enableHitbox", "1", "Enable hitbox information for car", true, true, 0, true, 1).bindTo(m_hitboxEnabled);
	cvarManager->getCvar("cl_bones_plugin_enableHitbox").addOnValueChanged(std::bind(&BakkesPlugin::OnHitboxEnabledValueChanged, this, std::placeholders::_1, std::placeholders::_2));

	// Game is started
	gameWrapper->HookEvent("Function TAGame.GameEvent_TA.Init", bind(&BakkesPlugin::OnFreeplayLoad, this, std::placeholders::_1));

	// Game is cleaning up
	gameWrapper->HookEvent("Function TAGame.GameEvent_TA.Destroyed", bind(&BakkesPlugin::OnFreeplayDestroy, this, std::placeholders::_1));

	// Ball hits ground
	gameWrapper->HookEvent("Function TAGame.Ball_TA.EventHitGround", bind(&BakkesPlugin::OnBallHitGround, this, std::placeholders::_1));

	// Car hits ball
	gameWrapper->HookEvent("Function TAGame.Car_TA.EventHitBall", bind(&BakkesPlugin::OnCarHitBall, this, std::placeholders::_1));

	// Pause menu
	gameWrapper->HookEvent("Function TAGame.PlayerController_TA.OnOpenPauseMenu", bind(&BakkesPlugin::OnPauseMenu, this, std::placeholders::_1));

	// On Unpause
	gameWrapper->HookEvent("Function ProjectX.GameInfo_X.RemovePauser", bind(&BakkesPlugin::OnUnpause, this, std::placeholders::_1));
}

void BakkesPlugin::OnCarHitBall(std::string eventName) {

	if (*m_jugglerEnabled) {
		m_juggler.HitBall();
	}
}

void BakkesPlugin::OnBallHitGround(std::string eventName) {
	m_juggler.Reset();
}

void BakkesPlugin::OnPauseMenu(std::string eventName) {
	m_juggler.Pause();
}

void BakkesPlugin::OnUnpause(std::string eventName) {
	m_juggler.Unpause();
}

void BakkesPlugin::OnFreeplayLoad(std::string eventName) {
	if (m_registeredRender) {
		return;
	}

	m_debugLogs.OnLoad(cvarManager);
	m_debugLogs.ShowLogs(*m_debugLogsEnabled);
	m_juggler.OnLoad(&m_debugLogs);
	m_hitbox.OnLoad(&m_debugLogs);
	m_registeredRender = true;
	gameWrapper->RegisterDrawable(std::bind(&BakkesPlugin::Render, this, std::placeholders::_1));
	m_debugLogs.RawLog(std::string("Freeplay load"));

	if (!gameWrapper->IsInFreeplay()) {
		m_debugLogs.Log(std::string("OnFreeplayLoad: Not in freeplay"));
		return;
	}
}

void BakkesPlugin::OnFreeplayDestroy(std::string eventName) {
	m_debugLogs.OnUnload();
	m_juggler.OnDestroy();
	m_hitbox.OnDestroy();
	m_registeredRender = false;
	gameWrapper->UnregisterDrawables();
}

/*
Happens when the plugin enabled value changes
*/
void BakkesPlugin::OnPluginEnabledValueChanged(std::string oldValue, CVarWrapper cvar) {
	m_debugLogs.Log(std::string("Plugin enabled value changed"));
}

/*
Conditionally turning logs on/off

*/
void BakkesPlugin::OnDebugLogsEnabledValueChanged(std::string oldValue, CVarWrapper cvar) {
	if (cvar.getBoolValue()) {
		m_debugLogs.ShowLogs(true);
		m_debugLogs.Log(std::string("DebugLogEnabledValueChanged: true"));
	}
	else {
		m_debugLogs.ShowLogs(false);
		m_debugLogs.Log(std::string("DebugLogEnabledValueChanged: false"));
	}
}

void BakkesPlugin::OnJugglerEnabledValueChanged(std::string oldValue, CVarWrapper cvar) {
	if (cvar.getBoolValue()) {
		m_debugLogs.Log(std::string("OnJugglerEnabledValueChanged: true"));
	}
	else {
		m_debugLogs.Log(std::string("OnJugglerEnabledValueChanged: false"));
	}
}

void BakkesPlugin::OnHitboxEnabledValueChanged(std::string oldValue, CVarWrapper cvar) {
	if (cvar.getBoolValue()) {
		m_debugLogs.Log(std::string("OnHitboxEnabledValueChanged: true"));
	}
	else {
		m_debugLogs.Log(std::string("OnHitboxEnabledValueChanged: false"));
	}
}


void BakkesPlugin::Render(CanvasWrapper canvas)
{
	if (!gameWrapper->IsInFreeplay() && !gameWrapper->IsInCustomTraining()) {
		m_debugLogs.Log(std::string("Not in a game"));
		return;
	}

	if (!gameWrapper->IsInGame()) {
		m_debugLogs.Log(std::string("Not in a game"));
		return;
	}

	if (*m_debugLogsEnabled) {
		m_debugLogs.Render(canvas, *m_debugLogPosX, *m_debugLogPosY);
	}

	if (*m_jugglerEnabled) {
			m_juggler.Render(canvas, gameWrapper, *m_jugglerPosX, *m_jugglerPosY);
	}
		
	if (*m_hitboxEnabled) {
			m_hitbox.Render(canvas, gameWrapper);
	}
}

void BakkesPlugin::onUnload()
{
	m_debugLogs.OnUnload();
	m_juggler.OnDestroy();
	m_hitbox.OnDestroy();
	m_registeredRender = false;
	m_loaded = false;
}

