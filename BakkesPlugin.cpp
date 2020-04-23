/**




*/
#include "BakkesPlugin.h"

BAKKESMOD_PLUGIN(BakkesPlugin, "Bakkesmod Skeleton Plugin", "1.0", PLUGINTYPE_FREEPLAY | PLUGINTYPE_CUSTOM_TRAINING)

BakkesPlugin::BakkesPlugin() {
}

BakkesPlugin::~BakkesPlugin() {
}

void BakkesPlugin::onLoad() {
	m_debugLogs.Log(std::string("Juggler Plugin Loaded"));

	gameWrapper->HookEvent("Function TAGame.GameEvent_Tutorial_TA.OnInit", bind(&BakkesPlugin::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_Tutorial_TA.Destroyed", bind(&BakkesPlugin::OnFreeplayDestroy, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TrainingEditor_TA.StartPlayTest", bind(&BakkesPlugin::OnFreeplayLoad, this, std::placeholders::_1));
	gameWrapper->HookEvent("Function TAGame.GameEvent_TrainingEditor_TA.Destroyed", bind(&BakkesPlugin::OnFreeplayDestroy, this, std::placeholders::_1));

	m_pluginEnabled = make_shared<bool>(true);
	cvarManager->registerCvar("cl_plugin_enabled", "0", "Enable plugin", true, true, 0, true, 1).bindTo(m_pluginEnabled);
	cvarManager->getCvar("cl_plugin_enabled").addOnValueChanged(std::bind(&BakkesPlugin::OnPluginEnabledValueChanged, this, std::placeholders::_1, std::placeholders::_2));



	//hitboxOn = make_shared<bool>(true);
	//cvarManager->registerCvar("cl_soccar_showhitbox", "0", "Show Hitbox", true, true, 0, true, 1).bindTo(hitboxOn);
	//cvarManager->getCvar("cl_soccar_showhitbox").addOnValueChanged(std::bind(&HitboxPlugin::OnHitboxOnValueChanged, this, std::placeholders::_1, std::placeholders::_2));

	//hitboxType = make_shared<int>(0);
	//cvarManager->registerCvar("cl_soccar_sethitboxtype", "0", "Set Hitbox Car Type", true, true, 0, true, 32767, false).bindTo(hitboxType);
	//cvarManager->getCvar("cl_soccar_sethitboxtype").addOnValueChanged(std::bind(&HitboxPlugin::OnHitboxTypeChanged, this, std::placeholders::_1, std::placeholders::_2));



	//cvarManager->registerNotifier("cl_soccar_listhitboxtypes", [this](vector<string> params) {
	//	cvarManager->log(CarManager::getHelpText());
	//	}, "List all hitbox integer types, use these values as parameters for cl_soccar_sethitboxtype", PERMISSION_ALL);
}

void BakkesPlugin::OnPluginEnabledValueChanged(std::string oldValue, CVarWrapper cvar) {
	m_debugLogs.Log(std::string("Plugin enabled value changed"));
}

void BakkesPlugin::OnFreeplayLoad(std::string eventName)
{
	m_debugLogs.Log(std::string("Freeplay Loaded"));
}

void BakkesPlugin::OnFreeplayDestroy(std::string eventName)
{
	// Delete allocated text objects:
	this->m_debugLogs.ClearLogs();

	//gameWrapper->UnregisterDrawables();
}

void BakkesPlugin::Render(CanvasWrapper canvas)
{
	if (!gameWrapper->IsInGame()) {
		return;		
	}

	if (m_debugLogsEnabled) {
		m_debugLogs.Render(canvas);
	}

}

void BakkesPlugin::onUnload()
{
	m_debugLogs.ClearLogs();
}

