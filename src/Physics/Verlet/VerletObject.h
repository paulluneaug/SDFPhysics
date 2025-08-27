#pragma once
#include <SFML/Graphics.hpp>


template<typename TFloatType>
class VerletObject
{
public:
	sf::Vector2<TFloatType> Position;
	sf::Vector2<TFloatType> PreviousPosition;
	float Radius;
private:
	sf::Vector2<TFloatType> m_acceleration;


public:
	VerletObject(sf::Vector2<TFloatType>& startPosition, float radius) :
		Position(startPosition),
		PreviousPosition(startPosition),
		Radius(radius),
		m_acceleration({0.0f, 0.0f})
	{
	}

	void Update(TFloatType deltaTime) 
	{
		const sf::Vector2<TFloatType> positionDelta = Position - PreviousPosition;
		PreviousPosition = Position;
		Position = Position + positionDelta + m_acceleration * (deltaTime * deltaTime);

		m_acceleration = {};
	}

	void AddForce(sf::Vector2<TFloatType>& force)
	{
		m_acceleration += force;
	}


	void SetVelocity(sf::Vector2<TFloatType> velocity, TFloatType deltaTime)
	{
		PreviousPosition = Position - (velocity * deltaTime);
	}

	void AddVelocity(sf::Vector2<TFloatType> velocity, TFloatType deltaTime)
	{
		PreviousPosition -= velocity * deltaTime;
	}

	sf::Vector2<TFloatType> GetVelocity(TFloatType deltaTime) const
	{
		return (Position - PreviousPosition) / deltaTime;
	}
};

