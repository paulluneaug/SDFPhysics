#pragma once
#include <SFML/Graphics.hpp>

class Math
{
public:
	// Vector2
	template<typename T>
	sf::Vector2<T> Abs(const sf::Vector2<T>& v);
	template<typename T>
	sf::Vector2<T> Min(const sf::Vector2<T>& a, const sf::Vector2<T>& b);
	template<typename T>
	sf::Vector2<T> Max(const sf::Vector2<T>& a, const sf::Vector2<T>& b);

	template<typename T>
	static constexpr sf::Vector2<T> Vector2Zero = sf::Vector2<T>{};
};
