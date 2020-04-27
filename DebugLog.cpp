#include "DebugLog.h"

DebugLog::DebugLog() {
	m_loaded = false;
	m_visible = true;
	m_maxLogMessages = 20;
}

DebugLog::~DebugLog() {
	this->ClearLogs();
}

void DebugLog::OnLoad(std::shared_ptr<CVarManagerWrapper> cvarWrapper) {
	if (m_loaded) {
		return;
	}

	m_visible = true;
	m_loaded = true;
	m_cvarWrapper = cvarWrapper;

	Log("[DebugLog] loaded module");
}

void DebugLog::OnUnload() {
	m_loaded = false;
	RawLog("[DebugLog] unloaded module");
}

void DebugLog::ShowLogs(bool visible) {
	m_visible = visible;
}

void DebugLog::RawLog(std::string message) {
	if (m_cvarWrapper) {
		m_cvarWrapper->log(message);
	}
}

void DebugLog::Log(std::string message) {
	LogMessage* log = new LogMessage();
	log->m_text = message;
	m_logs.push_back(log);
	RawLog(message);

	if (m_logs.size() > m_maxLogMessages) {
		LogMessage* purge = m_logs.front();
		delete purge;
		purge = NULL;
		m_logs.pop_front();
	}
}

void DebugLog::ClearLogs() {
	while (m_logs.size()) {
		LogMessage* purge = m_logs.front();
		delete purge;
		purge = NULL;
		m_logs.pop_front();
	}
}

void DebugLog::Render(CanvasWrapper& canvas, int xPos, int yPos) {
	if (!m_loaded) {
		RawLog("[DebugLog::Render] not loaded");
		return;
	}

	if (!m_visible) {
		RawLog("[DebugLog::Render] not visible");
		return;
	}

	// Render logs:
	auto screenSize = canvas.GetSize();

	list<LogMessage*>::iterator it;
	unsigned count = 0;
	for (it = m_logs.begin(); it != m_logs.end(); ++it, ++count) {
		LogMessage* log = *it;
		log->Render(canvas, xPos, yPos - (count * 35), 2, 2, 255);
	}
}

// Log messages are told where to draw
void LogMessage::Render(CanvasWrapper& canvas, int xPos, int yPos, int xScale, int yScale, int alpha) {
	Vector2 drawLoc = { xPos, yPos };
	canvas.SetPosition(drawLoc);
	canvas.SetColor(m_color.R, m_color.G, m_color.B, alpha);
	canvas.DrawString(m_text, xScale, yScale);
}