#include "Profiler.h"

Profiler::Profiler()
{
	Reset();
}

void Profiler::StartEvent(const char* eventName)
{
	if (m_eventStarted)
	{
		return;
	}
	m_events.emplace_back(eventName);
	m_eventStarted = true;
	m_eventStartTime = m_clock.getElapsedTime().asMilliseconds();
}

void Profiler::EndEvent()
{
	if (!m_eventStarted)
	{
		return;
	}
	m_eventStarted = false;
	Event& endedEvent = m_events[m_events.size() - 1];
	endedEvent.ElapsedTime = m_clock.getElapsedTime().asMilliseconds() - m_eventStartTime;
}

void Profiler::DrawImGui()
{
	if (ImGui::CollapsingHeader("Profiler")) 
	{
		for (const Event& event : m_events)
		{
			event.DrawImGui();
		}
	}
}

void Profiler::Reset()
{
	m_eventStarted = false;
	m_events.clear();
	m_clock.restart();
	m_eventStartTime = 0.0f;
}
