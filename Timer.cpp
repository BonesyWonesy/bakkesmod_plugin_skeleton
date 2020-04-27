#include "Timer.h"

Timer::Timer() : m_running(false) {}

void Timer::Start() {
  if (!m_running) {
    m_startTime = std::chrono::steady_clock::now();
    m_intervals.push_back(m_startTime);
    m_running = true;
  }
}

void Timer::Stop() {
  m_endTime = std::chrono::steady_clock::now();
  m_running = false;
  m_intervals.clear();
}

void Timer::Pause() {
  m_running = false;
  m_endTime = std::chrono::steady_clock::now();
  m_intervals.push_back(m_endTime);
}


double Timer::ElapsedMilliseconds() {
  if (!m_running && !m_intervals.size()) {
    return 0.0;
  }

  double storedIntervals = 0.0;

  if (m_intervals.size()) {
    for (unsigned i = 1; i <= m_intervals.size() - 1; i+=2) {      
        storedIntervals += std::chrono::duration_cast<std::chrono::milliseconds>(m_intervals[i] - m_intervals[i - 1]).count();
    }
  }

  std::chrono::time_point<std::chrono::steady_clock> endTime;

  if (m_running) {
    endTime = std::chrono::steady_clock::now();
  }
  else if (m_intervals.size()) {
    return storedIntervals;
  }
  else {
    endTime = m_endTime;
  }

  return storedIntervals + std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();
}

double Timer::ElapsedSeconds() {
  return ElapsedMilliseconds() / 1000.0;
}
