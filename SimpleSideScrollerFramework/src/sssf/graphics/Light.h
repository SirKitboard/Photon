#pragma once


#include "Box2D/Box2D.h"
#include <SFML/Graphics.hpp>
#include "sssf_VS/stdafx.h"




namespace cse380 {
	namespace sssf {
		
		namespace game { class Game; }
		
		namespace graphics {
			using game::Game;


			class Light
			{
			public:
				Light(Game* game, b2Vec2 position, float reach, float angle, float angleSpread, bool active);
				~Light() {}
				
				sf::VertexArray computeAndGetLightShape();

				b2Vec2 getPosition() { return position; }
				void setPosition(b2Vec2 position) { this->position = position; }
				float getReach() { return reach; }
				void setReach(float reach) { this->reach = reach; }
				float getAngle() { return angle; }
				void setAngle(float angle) { this->angle = normalizeAngle(angle); }
				float getAngleSpread() { return 2.0f*halfAngleSpread; }
				void setAngleSpread(float angleSpread) { this->halfAngleSpread = normalizeAngle(angleSpread/2.0f); }
				bool isActive() { return active; }
				void setActive(bool active) { this->active = active; }
				float pointRectDist(b2Vec2 pointVec);


			private:
				float normalizeAngle(float angle);//approved
				bool angleIsLess(b2Vec2 center, b2Vec2 a, b2Vec2 b);
				bool angleIsLessFinal(b2Vec2 center, b2Vec2 a, b2Vec2 b);
				float getPointAngle(b2Vec2 center, b2Vec2 point);
				bool pointInAngleRange(b2Vec2 center, float angle, float halfAngleSpread, b2Vec2 point);
				b2Vec2 generatePointFromAngle(b2Vec2 center, float reach, float angle);

				void prePopulatePoints(std::vector<b2Vec2>* points, float angle, float halfAngleSpread, int resolution);
				void sortPoints(std::vector<b2Vec2>* points, int head, int tail);
				void cleanPoints(std::vector<b2Vec2>* points);
				float distance(b2Vec2 a, b2Vec2 b);
				sf::Vector2f b2WorldToViewport(b2Vec2 point);
				

				Game* game;
				b2Vec2 position;
				float reach;
				float angle;
				float halfAngleSpread;
				bool active;

			};
		}

	}
}

