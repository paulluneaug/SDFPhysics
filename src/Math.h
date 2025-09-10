#pragma once
#include <SFML/Graphics.hpp>

#include <algorithm>

class Math
{
public:
	template<typename T>
	static constexpr T PI = static_cast<T>(3.1415926536);

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
		const TFloat point5 = static_cast<TFloat>(0.5);
		const TFloat one = static_cast<TFloat>(1.0);
		const TFloat two = static_cast<TFloat>(2.0);
		const TFloat three = static_cast<TFloat>(3.0);
		const TFloat four = static_cast<TFloat>(4.0);
		const TFloat sqrt05 = static_cast<TFloat>(sqrt(0.5));
		const TFloat thirteen = static_cast<TFloat>(13.0);
#define SMinMethod 0
#if SMinMethod == 0

		k *= four;
		TFloat h = std::max(k - std::abs(a - b), zero) / k;
		return std::min(a, b) - h * h * k * (one / four);
#elif SMinMethod == 1
		k *= one / (one - sqrt05);
		TFloat h = std::max(k - std::abs(a - b), zero) / k;
		const TFloat b2 = thirteen / four - four * sqrt05;
		const TFloat b3 = three / four - one * sqrt05;
		return std::min(a, b) - k * h * h * (h * b3 * (h - four) + b2);
#elif SMinMethod == 2
		k *= one / (one - sqrt05);
		TFloat h = std::max(k - std::abs(a - b), zero) / k;
		return std::min(a, b) - k * point5 * (one + h - std::sqrt(one - h * (h - two)));
#endif
	}

	template<typename TVal>
	static constexpr TVal Sign(const TVal value)
	{
		return value >= static_cast<TVal>(0) ? static_cast < TVal>(1) : static_cast<TVal>(-1);
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

	template<typename TVal>
	static constexpr TVal Clamp(const TVal v, const TVal min, TVal max)
	{
		if (v < min)
		{
			return min;
		}

		if (v > max)
		{
			return max;
		}
		return v;
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
