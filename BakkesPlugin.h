#pragma once
#include <memory>
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "DebugLog.h"
#include "Juggler.h"
#include "Hitbox.h"

/**
  Aims to be a framework skeleton for a BakkesMod plugin. It has built in functionality to make development easier and quicker
  for plugin development.

   - Debug logging: Has a way to log debug messages to the screen so you can see when things are happening.
   - Standard configurability: will provide standard plugin options that are almost always universal but also make it easy to
     add additional options to your plugin.
*/
class BakkesPlugin : public BakkesMod::Plugin::BakkesModPlugin
{

public:
	BakkesPlugin();
	~BakkesPlugin();

	/// <summary>
	/// Inhereted from BakkesModPlugin
	/// </summary>
	virtual void onLoad();

	/// <summary>
	/// Inhereted from BakkesModPlugin
	/// </summary>
	virtual void onUnload();

private:

	// "Enable Plugin"
	void OnPluginEnabledValueChanged(std::string oldValue, CVarWrapper cvar);

	// "Enable Debug Logs"
	void OnDebugLogsEnabledValueChanged(std::string oldValue, CVarWrapper cvar);

	// "Enable Juggler"
	void OnJugglerEnabledValueChanged(std::string oldValue, CVarWrapper cvar);

	// "Enable Hitbox"
	void OnHitboxEnabledValueChanged(std::string oldValue, CVarWrapper cvar);


	void OnFreeplayLoad(std::string eventName);
	void OnFreeplayDestroy(std::string eventName);
	void OnBallHitGround(std::string eventName);
	void OnCarHitBall(std::string eventName);
	void OnPauseMenu(std::string eventName);
	void OnUnpause(std::string eventName);

	virtual void Render(CanvasWrapper canvas);

	// Variables for standard plugin options:
	// - enabled
	// - showLogs
	// - saveSettings
	// 
	std::shared_ptr<bool> m_pluginEnabled;
	std::shared_ptr<bool> m_debugLogsEnabled;

	DebugLog m_debugLogs;
	std::shared_ptr<int> m_debugLogPosX;
	std::shared_ptr<int> m_debugLogPosY;

	Juggler m_juggler;
	std::shared_ptr<bool> m_jugglerEnabled;
	std::shared_ptr<int> m_jugglerPosX;
	std::shared_ptr<int> m_jugglerPosY;

	Hitbox m_hitbox;
	std::shared_ptr<bool> m_hitboxEnabled;

	bool m_loaded;
	bool m_registeredRender;

	// Inhereted objects:
	// std::shared_ptr<GameWrapper> gameWrapper;
	// std::shared_ptr<CVarManagerWrapper> cvarManager;
};