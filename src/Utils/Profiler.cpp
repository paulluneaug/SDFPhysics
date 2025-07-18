#include "Profiler.h"

Profiler::Profiler()
{
	Reset();
}

void Profiler::StartEvent(const char* eventName)
{
	m_events.emplace_back(eventName);
}

void Profiler::EndEvent()
{
}

void Profiler::DrawImGui()
{
	for (const Event& event : m_events) 
	{
		event.DrawImGui();
	}
}

void Profiler::Reset()
{
	EndEvent();
	m_events.clear();
}
