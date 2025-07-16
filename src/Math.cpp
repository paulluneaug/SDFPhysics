#include "Math.h"

#include <algorithm>

template<typename T>
sf::Vector2<T> Math::Abs(const sf::Vector2<T>& v)
{
	return sf::Vector2<T>{std::abs(v.x), std::abs(v.y)};
}

template<typename T>
sf::Vector2<T> Math::Min(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
{
	return sf::Vector2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
}

template<typename T>
sf::Vector2<T> Math::Max(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
{
	return sf::Vector2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
}
