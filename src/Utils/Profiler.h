#pragma once

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <vector>

class Profiler
{
	class Event 
	{
	public:
		Event* ParentEvent;
		int Depth;

	private:
		const char* m_eventName;

		int m_startTime;
		int m_elapsedTime;

		bool m_ended;


	public:
		Event(const char* name, int startTime, Event* parentEvent, int depth) :
			ParentEvent(parentEvent),
			Depth(depth),
			m_eventName(name),
			m_startTime(startTime),
			m_elapsedTime(0),
			m_ended(false)
		{

		}

		void EndEvent(int endTime) 
		{
			m_ended = true;
			m_elapsedTime = endTime - m_startTime;
		}

		void DrawImGui() const
		{
			if (!m_ended) 
			{
				return;
			}
			ImGui::Text("%s : %i ms", m_eventName, m_elapsedTime);
		}
	};

private:
	sf::Clock m_clock;

	std::vector<Event> m_events;

	int m_depth;
	Event* m_currentEvent;

public:
	Profiler();

	void StartEvent(const char* eventName);
	void EndEvent();

	void DrawImGui();
	void Reset();
};

