#pragma once
#include <SFML/Graphics.hpp>

class VerletObject
{
private:
	sf::Vector2f m_position;
	sf::Vector2f m_previousPosition;
	sf::Vector2f m_acceleration;

	float m_radius;

public:
	VerletObject(sf::Vector2f& startPosition, float radius) :
		m_position(startPosition),
		m_previousPosition(startPosition),
		m_acceleration({0.0f, 0.0f}),
		m_radius(radius)
	{
	}

	void Update(float deltaTime) 
	{

	}

	void AddForce(sf::Vector2f& force) 
	{
		m_acceleration += force;
	}


	void SetVelocity(sf::Vector2f velocity, float deltaTime)
	{
		m_previousPosition = m_position - (velocity * deltaTime);
	}

	void AddVelocity(sf::Vector2f velocity, float deltaTime)
	{
		m_previousPosition -= velocity * deltaTime;
	}

	sf::Vector2f GetVelocity(float deltaTime) const
	{
		return (m_position - m_previousPosition) / deltaTime;
	}
};

