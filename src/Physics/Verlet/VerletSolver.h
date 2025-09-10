#pragma once
#include "VerletObject.h"

#include "../../SDFs/SDF.h"
#include "../../SDFs//SDFUtils.h"

template<typename TFloatType>
class VerletSolver
{
	typedef VerletObject<TFloatType> VerletObj;

private:
	uint32_t m_subSteps = 8;
	sf::Vector2<TFloatType> m_gravity = { static_cast<TFloatType>(0.0f), static_cast <TFloatType>(1000.0f) };
	std::vector<VerletObj> m_objects;

	SDF<TFloatType>* m_sceneSDF;

public:
	VerletSolver(SDF<TFloatType>* sceneSDF) :
		m_sceneSDF(sceneSDF)
	{

	};

	VerletObj& AddObject(sf::Vector2<TFloatType> position, float radius)
	{
		return m_objects.emplace_back(position, radius);
	}

	void Update(float deltaTime)
	{
		const float stepDeltaTime = deltaTime / static_cast<float>(m_subSteps);

		for (uint32_t i = 0u; i < m_subSteps; ++i)
		{
			ApplyGravity();
			CheckCollisions(stepDeltaTime);
			ApplyConstraint();
			UpdateObjects(stepDeltaTime);
		}
	}

	const std::vector<VerletObj>& GetObjects() const
	{
		return m_objects;
	}

private:
	void ApplyGravity()
	{
		for (VerletObj& obj : m_objects)
		{
			obj.AddForce(m_gravity);
		}
	}

	void CheckCollisions(float deltaTime)
	{
		const TFloatType responseCoef = static_cast<TFloatType>(0.75);
		const uint64_t objectsCount = m_objects.size();

		for (uint64_t i = 0u; i < objectsCount; ++i)
		{
			VerletObj& object1 = m_objects[i];

			for (uint64_t k = i + 1; k < objectsCount; ++k)
			{
				VerletObj& object2 = m_objects[k];

				const sf::Vector2<TFloatType> offset = object1.Position - object2.Position;
				const TFloatType sqrDistance = offset.x * offset.x + offset.y * offset.y;
				const TFloatType minDistance = object1.Radius + object2.Radius;

				if (sqrDistance < minDistance * minDistance)
				{
					const TFloatType dist = static_cast<TFloatType>(sqrt(sqrDistance));
					const sf::Vector2<TFloatType> n = offset / dist;

					const TFloatType massRatio1 = object1.Radius / minDistance;
					const TFloatType massRatio2 = object2.Radius / minDistance;

					const TFloatType delta = 0.5f * responseCoef * (dist - minDistance);

					object1.Position -= n * (massRatio2 * delta);
					object2.Position += n * (massRatio1 * delta);
				}
			}
		}
	}

	void ApplyConstraint()
	{
		for (VerletObj& obj : m_objects)
		{
			TFloatType distance = m_sceneSDF->Evaluate(obj.Position);
			if (distance >= obj.Radius)
			{
				continue;
			}
			TFloatType penetration = obj.Radius - distance;
			const sf::Vector2<TFloatType> normal = SDFUtils<TFloatType>::ComputeGradient(m_sceneSDF, obj.Position);
			obj.Position += normal * (penetration);
		}
	}

	void UpdateObjects(float deltaTime)
	{
		for (VerletObj& obj : m_objects)
		{
			obj.Update(deltaTime);
		}
	}
};

