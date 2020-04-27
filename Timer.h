#pragma once
#include <chrono>
#include <vector>

class Timer
{
public:
  Timer();
  void Start();
  void Stop();
  void Pause();

  double ElapsedMilliseconds();
  double ElapsedSeconds();

  bool IsRunning() { return m_running; }

private:
  std::chrono::time_point<std::chrono::steady_clock> m_startTime;
  std::chrono::time_point<std::chrono::steady_clock> m_endTime;
  std::vector<std::chrono::time_point<std::chrono::steady_clock>> m_intervals;
  bool m_running;
};