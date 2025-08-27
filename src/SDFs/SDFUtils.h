#pragma once
#include <algorithm>

#include "../Alias.h"

using namespace Alias;

template<typename TFloatType>
class SDFUtils
{
public:
	static TFloatType EvalOffset;
private:
	constexpr static TFloatType PI = static_cast<TFloatType>(3.1415926535);
	constexpr static TFloatType TAU = 2 * PI;
	constexpr static uint32_t ITERATIONS_COUNT = 20;

public:

	static sf::Vector2<TFloatType> ComputeGradient(SDF_RPtr<TFloatType> sdf, const sf::Vector2<TFloatType>& position)
	{
		TFloatType magnitude = sdf->Evaluate(position);

		sf::Vector2<TFloatType> direction = ComputeGradientDirection(sdf, position);

		TFloatType magnitudeFactor = magnitude / direction.length();
		return { direction.x * magnitudeFactor, direction.y * magnitudeFactor };
	}

private:
	static sf::Vector2<TFloatType> ComputeGradientDirection(SDF_RPtr<TFloatType> sdf, const sf::Vector2<TFloatType>& position)
	{
		constexpr TFloatType angle0 = static_cast<TFloatType>(0);
		constexpr TFloatType angle1 = TAU / static_cast<TFloatType>(3);
		constexpr TFloatType angle2 = 2 * TAU / static_cast<TFloatType>(3);

		TFloatType value0 = sdf->Evaluate(GetGradientVector(angle0, position));
		TFloatType value1 = sdf->Evaluate(GetGradientVector(angle1, position));
		TFloatType value2 = sdf->Evaluate(GetGradientVector(angle2, position));

		TFloatType sum01 = value0 + value1;
		TFloatType sum02 = value0 + value2;
		TFloatType sum12 = value1 + value2;

		TFloatType minSum = std::min(std::min(sum01, sum02), sum12);

		TFloatType minAngle, minValue, maxAngle, maxValue;
		if (minSum == sum01)
		{
			minAngle = angle0;
			minValue = value0;
			maxAngle = angle1;
			maxValue = value1;
		}
		else if (minSum == sum02)
		{
			minAngle = angle0;
			minValue = value0;
			maxAngle = angle2;
			maxValue = value2;
		}
		else
		{
			minAngle = angle1;
			minValue = value1;
			maxAngle = angle2;
			maxValue = value2;
		}

		for (uint32_t i = 0; i < ITERATIONS_COUNT; ++i)
		{
			DirectionBinarySearchStep(sdf, position, minAngle, minValue, maxAngle, maxValue);
		}

		TFloatType resultAngle = ModTau((minAngle + maxAngle) / 2);
		return { std::cos(resultAngle), std::sin(resultAngle) };
	}

	constexpr static sf::Vector2<TFloatType> GetGradientVector(TFloatType angle, const sf::Vector2<TFloatType>& position)
	{
		return { position.x + std::cos(angle) * EvalOffset, position.y + std::sin(angle) * EvalOffset };
	}

	static void DirectionBinarySearchStep(SDF_RPtr<TFloatType> sdf, const sf::Vector2<TFloatType>& position, TFloatType& minAngle, TFloatType& minValue, TFloatType& maxAngle, TFloatType& maxValue)
	{
		TFloatType middleAngle = ModTau((minAngle + maxAngle) / 2);
		TFloatType middleValue = sdf->Evaluate(GetGradientVector(middleAngle, position));

		if (minValue + middleValue < maxValue + middleValue)
		{
			maxAngle = middleAngle;
			maxValue = middleValue;
		}
		else
		{
			minAngle = middleAngle;
			minValue = middleValue;
		}
	}

	constexpr static TFloatType ModTau(TFloatType val)
	{
		return val < 0 ? val + TAU : val;
	}
};
