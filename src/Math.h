#pragma once
#include <SFML/Graphics.hpp>

#include <algorithm>

class Math
{
public:
	// Vector2
	template<typename T>
	static constexpr sf::Vector2<T> Vector2Zero = sf::Vector2<T>{};

	template<typename T>
	static constexpr sf::Vector2<T> Abs(const sf::Vector2<T>& v)
	{
		return sf::Vector2<T>{std::abs(v.x), std::abs(v.y)};
	}

	template<typename T>
	static constexpr sf::Vector2<T> Min(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return sf::Vector2<T>(std::min(a.x, b.x), std::min(a.y, b.y));
	}

	template<typename T>
	static constexpr sf::Vector2<T> Max(const sf::Vector2<T>& a, const sf::Vector2<T>& b)
	{
		return sf::Vector2<T>(std::max(a.x, b.x), std::max(a.y, b.y));
	}

	// Float
	template<typename TFloat>
	static constexpr float SmoothMin(const TFloat a, const TFloat b, TFloat k)
	{
		const TFloat zero = static_cast<TFloat>(0.0);
		const TFloat one = static_cast<TFloat>(1.0);
		const TFloat four = static_cast<TFloat>(4.0);

		k *= four;
		TFloat h = std::max(k - std::abs(a - b), zero) / k;
		return std::min(a, b) - h * h * k * (one / four);
	}

	template<typename TVal>
	static constexpr float Sign(const TVal value)
	{
		return value >= static_cast<TVal>(0) ? 1 : -1;
	}

	template<typename TVal>
	static constexpr TVal Min(const TVal val0, const TVal val1)
	{
		return val0 < val1 ? val0 : val1;
	}

	template<typename TVal>
	static constexpr TVal Max(const TVal val0, const TVal val1)
	{
		return val0 > val1 ? val0 : val1;
	}

	template<typename TVal, typename TFactor>
	static TVal Lerp(TVal a, TVal b, TFactor t)
	{
		return a + t * (b - a);
	}

	template<typename TVal>
	static TVal InverseLerp(TVal input, TVal rangeMin, TVal rangeMax)
	{
		return (input - rangeMin) / (rangeMax - rangeMin);
	}
};
