#include "Box2D/Box2D.h"

class RaycastCallback : public b2RayCastCallback
{
public:
	RaycastCallback() : m_fixture(NULL) {
		m_fixture = nullptr;
		m_fraction = 1.0f;
	}

	float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction) {
		if (fraction == 0.0f){
			return 1;
		}
		m_fixture = fixture;
		m_point = point;
		m_normal = normal;
		m_fraction = fraction;
		return fraction;
		
		/*
		if (fraction <= m_fraction||m_fixture == nullptr)
		{
			m_fixture = fixture;
			m_point = point;
			m_normal = normal;
			m_fraction = fraction;
		}
		return fraction;
		*/
	}

	b2Fixture* m_fixture;
	b2Vec2 m_point;
	b2Vec2 m_normal;
	float32 m_fraction;

};