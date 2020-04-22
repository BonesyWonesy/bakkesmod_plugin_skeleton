#include "DebugLog.h"


DebugLog::DebugLog() {

}

DebugLog::~DebugLog() {
	this->ClearLogs();
}

void DebugLog::Log(std::string message) {
	LogPopup* log = new LogPopup();
	log->text = message;
	m_logs.push_back(log);
}

void DebugLog::ClearLogs() {
	for (std::vector<LogPopup*>::iterator it = m_logs.begin(); it != m_logs.end(); ++it)
	{
		LogPopup* log = *it;
		delete log;
		log = NULL;
	}

	m_logs.clear();
}

void DebugLog::Render(CanvasWrapper canvas) {
	// Render logs:
	auto screenSize = canvas.GetSize();
	for (int i = 0; i < m_logs.size(); i++)
	{
		//auto log = m_logs.at(i);
		LogPopup* log = m_logs.at(i);
		if (log->startLocation.X < 0)
		{
			log->startLocation = { (int)(screenSize.X * 0.35), (int)(screenSize.Y * 0.1 + i * 0.035 * screenSize.Y) };
		}

		Vector2 drawLoc = { log->startLocation.X, log->startLocation.Y };
		canvas.SetPosition(drawLoc);
		canvas.SetColor(log->color.R, log->color.G, log->color.B, 255);
		canvas.DrawString(log->text, 3, 3);
	}

}