#pragma once
#include <memory>
#pragma comment( lib, "bakkesmod.lib" )
#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "DebugLog.h"


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

	// Callback functions:
	void OnPluginEnabledValueChanged(std::string oldValue, CVarWrapper cvar);


	virtual void OnFreeplayLoad(std::string eventName);
	virtual void OnFreeplayDestroy(std::string eventName);


	virtual void Render(CanvasWrapper canvas);

private:
	// Variables for standard plugin options:
	// - enabled
	// - showLogs
	// - saveSettings
	// 
	std::shared_ptr<bool> m_pluginEnabled;
	std::shared_ptr<bool> m_debugLogsEnabled;

	DebugLog m_debugLogs;

	// Inhereted objects:
	// std::shared_ptr<GameWrapper> gameWrapper;
	// std::shared_ptr<CVarManagerWrapper> cvarManager;
};