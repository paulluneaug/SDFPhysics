#pragma once
#include <algorithm>
#include <vector>

#include "../Alias.h"
#include "../Triangle.h"
#include "../Math.h"



using namespace Alias;

template<typename TFloatType>
class SDFUtils
{
private:
	static constexpr TFloatType EVAL_OFFSET = static_cast<TFloatType>(0.005);
	static constexpr TFloatType PI = static_cast<TFloatType>(3.1415926535);
	static constexpr TFloatType TAU = 2 * PI;
	static constexpr uint32_t GRADIENT_ITERATIONS_COUNT = 8;

	static constexpr uint32_t DESCENT_ITERATION_COUNT = 64;
	static constexpr TFloatType DESCENT_LEARNING_RATE = static_cast<TFloatType>(0.1);

public:

	static constexpr sf::Vector2<TFloatType> ComputeGradient(SDF_RPtr<TFloatType> sdf, const sf::Vector2<TFloatType>& position, bool absolute = true)
	{
#if false
		TFloatType sdfValueAtPosition = sdf->Evaluate(position);
		TFloatType offset = Math::Max(EVAL_OFFSET, sdfValueAtPosition / 20 * EVAL_OFFSET);

		sf::Vector2<TFloatType> v0 =
		{
			sdf->Evaluate({position.x + offset, position.y}),
			sdf->Evaluate({position.x, position.y + offset})
		};

		sf::Vector2<TFloatType> v1 =
		{
			sdf->Evaluate({position.x - offset, position.y}),
			sdf->Evaluate({position.x, position.y - offset})
		};
		sf::Vector2<TFloatType> delta = v0 - v1;
		return delta.lengthSquared() == 0.0 ? Math::Vector2Zero<TFloatType> : delta.normalized();
#else
		constexpr TFloatType angle0 = static_cast<TFloatType>(TAU / 6);
		constexpr TFloatType angle1 = angle0 + TAU / static_cast<TFloatType>(3);
		constexpr TFloatType angle2 = angle0 + 2 * TAU / static_cast<TFloatType>(3);

		TFloatType sdfValueAtPosition = sdf->Evaluate(position);

		TFloatType value0 = std::abs(sdf->Evaluate(GetGradientVector(angle0, position, sdfValueAtPosition)));
		TFloatType value1 = std::abs(sdf->Evaluate(GetGradientVector(angle1, position, sdfValueAtPosition)));
		TFloatType value2 = std::abs(sdf->Evaluate(GetGradientVector(angle2, position, sdfValueAtPosition)));

		value0 = absolute ? std::abs(value0) : value0;
		value1 = absolute ? std::abs(value1) : value1;
		value2 = absolute ? std::abs(value2) : value2;

		TFloatType sum01 = value0 + value1;
		TFloatType sum02 = value0 + value2;
		TFloatType sum12 = value1 + value2;

		TFloatType maxSum = std::max(std::max(sum01, sum02), sum12);

		TFloatType minAngle, minValue, maxAngle, maxValue;
		if (maxSum == sum01)
		{
			minAngle = angle0;
			minValue = value0;
			maxAngle = angle1;
			maxValue = value1;
		}
		else if (maxSum == sum02)
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

		for (uint32_t i = 0; i < GRADIENT_ITERATIONS_COUNT; ++i)
		{
			DirectionBinarySearchStep(sdf, position, minAngle, minValue, maxAngle, maxValue, sdfValueAtPosition, absolute);
		}

		TFloatType resultAngle = GetMiddleAngle(minAngle, maxAngle);
		return { std::cos(resultAngle), std::sin(resultAngle) };
#endif
	}
	
	static constexpr std::vector<sf::Vector2<TFloatType>> FindClosestPointOfTriangle(SDF_RPtr<TFloatType> sdf, const Triangle<TFloatType>& triangle)
	{
		std::vector<sf::Vector2<TFloatType>> result;
		result.reserve(DESCENT_ITERATION_COUNT + 1);

		TFloatType third = static_cast<TFloatType>(1.0 / 3.0);
		sf::Vector2<TFloatType> closestPoint = triangle.GetAbsoluteCoordinates({ third , third , third });

		result.push_back(closestPoint);

		for (uint32_t i = 0u; i < DESCENT_ITERATION_COUNT; ++i)
		{
			DescentStep(sdf, triangle, closestPoint);
			result.push_back(closestPoint);
		}

		return result;
	}

private:

	static void DescentStep(SDF_RPtr<TFloatType> sdf, const Triangle<TFloatType>& triangle, sf::Vector2<TFloatType>& closestPoint)
	{
		sf::Vector2<TFloatType> descentDirection = ComputeGradient(sdf, closestPoint, true);
		TFloatType sdfValue = sdf->Evaluate(closestPoint);
		sf::Vector2<TFloatType> step = closestPoint - descentDirection * (DESCENT_LEARNING_RATE * std::max(std::abs(sdfValue), static_cast<TFloatType>(100))* Math::Sign(sdfValue));
		closestPoint = triangle.GetClosestPoint(step);
	}

	static constexpr sf::Vector2<TFloatType> GetGradientVector(TFloatType angle, const sf::Vector2<TFloatType>& position, TFloatType valueAtPosition)
	{
		TFloatType offset = EVAL_OFFSET;// Math::Max(EVAL_OFFSET, valueAtPosition / 20 * EVAL_OFFSET);
		return { position.x + std::cos(angle) * offset, position.y + std::sin(angle) * offset };
	}

	static constexpr void DirectionBinarySearchStep(SDF_RPtr<TFloatType> sdf, const sf::Vector2<TFloatType>& position, TFloatType& minAngle, TFloatType& minValue, TFloatType& maxAngle, TFloatType& maxValue, TFloatType sdfValueAtPosition, bool absolute)
	{
		TFloatType middleAngle = GetMiddleAngle(minAngle, maxAngle);
		TFloatType sdfAtMiddleAngle = sdf->Evaluate(GetGradientVector(middleAngle, position, sdfValueAtPosition));
		TFloatType middleValue = absolute ? std::abs(sdfAtMiddleAngle) : sdfAtMiddleAngle;

		if (minValue + middleValue > maxValue + middleValue)
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

	static constexpr TFloatType ModTau(TFloatType val)
	{
		return val < 0 ? val + TAU : val;
	}

	static constexpr TFloatType GetMiddleAngle(TFloatType angle0, TFloatType angle1)
	{
		TFloatType middleAngle = ModTau((angle0 + angle1) / 2);
		if (std::abs(angle0 - angle1) > PI)
		{
			return middleAngle - PI;
		}
		return middleAngle;
	}
};
