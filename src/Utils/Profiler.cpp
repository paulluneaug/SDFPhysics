#include "Profiler.h"

Profiler::Profiler() :
	m_depth(0),
	m_currentEvent(nullptr)
{
	Reset();
}

void Profiler::StartEvent(const char* eventName)
{
	m_currentEvent = &m_events.emplace_back(eventName, m_clock.getElapsedTime().asMilliseconds(), m_currentEvent, m_depth);
	m_depth++;
}

void Profiler::EndEvent()
{
	if (m_depth <= 0)
	{
		return;
	}
	m_currentEvent->EndEvent(m_clock.getElapsedTime().asMilliseconds());

	m_currentEvent = m_currentEvent->ParentEvent;
	m_depth--;
}

void Profiler::DrawImGui()
{
	if (ImGui::CollapsingHeader("Profiler", ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentDepth = 0;
		for (const Event& event : m_events)
		{
			int eventDepth = event.Depth;
			while (currentDepth > eventDepth)
			{
				currentDepth--;
				ImGui::Unindent();
			}

			while (currentDepth < eventDepth)
			{
				currentDepth++;
				ImGui::Indent();
			}

			event.DrawImGui();
		}
	}
}

void Profiler::Reset()
{
	m_events.clear();
	m_clock.restart();
	m_depth = 0;
	m_currentEvent = nullptr;
}
