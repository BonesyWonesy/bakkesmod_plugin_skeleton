#pragma once

#include <list>
#include "CommonData.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

class LogMessage
{
public:

	std::string m_text = "";
	Color m_color = { 255, 255, 255 };

	LogMessage() {
		m_text = "";
		m_color = { 255, 255, 255 };
	}

	void Render(CanvasWrapper& canvas, int xPos, int yPos, int xScale, int yScale, int alpha);
};


class DebugLog
{
public:
	DebugLog();
	~DebugLog();
	void OnLoad(std::shared_ptr<CVarManagerWrapper> cvarWrapper);
	void OnUnload();
	void ShowLogs(bool visible);

	void RawLog(std::string message);
	void Log(std::string message);
	void ClearLogs();
	void Render(CanvasWrapper& canvas, int xPos, int yPos);

private:
	bool m_loaded;
	bool m_visible;
	int m_maxLogMessages;
	//std::vector<LogMessage*> m_logs;
	std::list<LogMessage*> m_logs;
	shared_ptr<CVarManagerWrapper> m_cvarWrapper;

};

