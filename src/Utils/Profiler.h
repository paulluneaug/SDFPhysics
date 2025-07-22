#pragma once

#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <vector>

class Profiler
{
	class Event 
	{
	public:
		int ElapsedTime;

	private:
		const char* m_eventName;

	public:
		Event(const char* name) :
			m_eventName(name),
			ElapsedTime(0)
		{

		}

		void DrawImGui() const
		{
			ImGui::Text("%s : %i ms", m_eventName, ElapsedTime);
		}
	};

private:
	sf::Clock m_clock;

	std::vector<Event> m_events;

	bool m_eventStarted;
	int m_eventStartTime;

public:
	Profiler();

	void StartEvent(const char* eventName);
	void EndEvent();

	void DrawImGui();
	void Reset();
};

