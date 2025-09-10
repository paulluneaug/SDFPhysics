#pragma once
#include <SFML/Graphics.hpp>

#include "Utils/ImGuiExtension.h"

template<typename TFloatType>
class Triangle
{
public:
	sf::Vector2<TFloatType> A;
	sf::Vector2<TFloatType> B;
	sf::Vector2<TFloatType> C;

private:
	sf::ConvexShape m_shape;

public:
	Triangle(sf::Vector2<TFloatType> a, sf::Vector2<TFloatType> b, sf::Vector2<TFloatType> c):
		A(a),
		B(b),
		C(c)
	{
		m_shape.setPointCount(3);
	}

	void Draw(sf::RenderWindow& target, sf::Color color)
	{
		UpdateShape();
		m_shape.setFillColor(color);
		target.draw(m_shape);
	}

	void DrawImGui()
	{
		ImGuiExtension::GenericDragFloat2("Vertex A", &A);
		ImGuiExtension::GenericDragFloat2("Vertex B", &B);
		ImGuiExtension::GenericDragFloat2("Vertex C", &C);
	}

	void UpdateShape()
	{
		m_shape.setPoint(0, { static_cast<float>(A.x),static_cast<float>(A.y) });
		m_shape.setPoint(1, { static_cast<float>(B.x),static_cast<float>(B.y) });
		m_shape.setPoint(2, { static_cast<float>(C.x),static_cast<float>(C.y) });
	}

	constexpr sf::Vector2<TFloatType> GetClosestPoint(const sf::Vector2<TFloatType>& point) const
	{
		if (IsPointInTriangle(point))
		{
			return point;
		}

		const sf::Vector2<TFloatType> closestFromAB = GetClosestPointOnEdge(A, B, point);
		const sf::Vector2<TFloatType> closestFromAC = GetClosestPointOnEdge(A, C, point);
		const sf::Vector2<TFloatType> closestFromBC = GetClosestPointOnEdge(B, C, point);

		const TFloatType sqrDistanceFromAB = (point - closestFromAB).lengthSquared();
		const TFloatType sqrDistanceFromAC = (point - closestFromAC).lengthSquared();
		const TFloatType sqrDistanceFromBC = (point - closestFromBC).lengthSquared();

		if (sqrDistanceFromAB < sqrDistanceFromAC && sqrDistanceFromAB < sqrDistanceFromBC)
		{
			return closestFromAB;
		}

		if (sqrDistanceFromAC < sqrDistanceFromBC)
		{
			return closestFromAC;
		}

		return closestFromBC;
	}

	constexpr sf::Vector2<TFloatType> GetAbsoluteCoordinates(const sf::Vector3<TFloatType>& barycentricCoordinates) const
	{
		return 
			barycentricCoordinates.x * A + 
			barycentricCoordinates.y * B + 
			barycentricCoordinates.z * C;
	}

	constexpr sf::Vector3<TFloatType> GetBarycentricCoordinates(const sf::Vector2<TFloatType>& absoluteCoordinates) const
	{
		const sf::Vector2<TFloatType> v0 = B - A;
		const sf::Vector2<TFloatType> v1 = C - A;
		const sf::Vector2<TFloatType> v2 = absoluteCoordinates - A;

		const TFloatType d00 = v0.dot(v0);
		const TFloatType d01 = v0.dot(v1);
		const TFloatType d11 = v1.dot(v1);
		const TFloatType d20 = v2.dot(v0);
		const TFloatType d21 = v2.dot(v1);
		const TFloatType denom = d00 * d11 - d01 * d01;
		const TFloatType v = (d11 * d20 - d01 * d21) / denom;
		const TFloatType w = (d00 * d21 - d01 * d20) / denom;
		const TFloatType u = 1.0f - v - w;

		return sf::Vector3<TFloatType>(u, v, w);
	}

	constexpr bool IsPointInTriangle(const sf::Vector2<TFloatType>& absoluteCoordinates) const
	{
		return IsPointInTriangle(GetBarycentricCoordinates(absoluteCoordinates));
	}

	constexpr bool IsPointInTriangle(const sf::Vector3<TFloatType>& barycentricCoordinates) const
	{
		return barycentricCoordinates.x >= static_cast<TFloatType>(0.0) 
			&& barycentricCoordinates.y >= static_cast<TFloatType>(0.0) 
			&& barycentricCoordinates.x + barycentricCoordinates.y <= static_cast<TFloatType>(1.0);
	}


private:
	sf::Vector2<TFloatType> GetClosestPointOnEdge(const sf::Vector2<TFloatType>& vertexU, const sf::Vector2<TFloatType>& vertexV, const sf::Vector2<TFloatType>& point) const
	{
		sf::Vector2<TFloatType> uv = vertexV - vertexU;
		sf::Vector2<TFloatType> up = point - vertexU;

		TFloatType uvSqrLength = uv.dot(uv);

		if (uvSqrLength == 0)
		{
			return vertexU;
		}

		TFloatType t = up.dot(uv) / uvSqrLength;
		if (t < 0)
		{
			return vertexU;
		}
		if (t > 1)
		{
			return vertexV;
		}
		return vertexU + t * uv;

	}
};