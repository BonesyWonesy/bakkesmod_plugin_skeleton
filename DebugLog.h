#pragma once

#include "CommonData.h"
#include "bakkesmod/plugin/bakkesmodplugin.h"

struct LogPopup
{
	std::string text = "";
	Color color = { 255, 255, 255 };
	Vector2 startLocation = { -1, -1 };

	LogPopup() {
		text = "";
		color = { 255, 255, 255 };
		startLocation = { -1, -1 };
	}
};


class DebugLog
{
public:
	DebugLog();
	~DebugLog();

	void Log(std::string message);
	void ClearLogs();
	void Render(CanvasWrapper canvas);

private:

	std::vector<LogPopup*> m_logs;
};

