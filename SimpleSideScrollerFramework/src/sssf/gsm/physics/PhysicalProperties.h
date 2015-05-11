#pragma once

#include <cmath>
//#include "sssf/game/Game.h"
#include <Box2D/Box2D.h>

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace physics {

		  //TODO: Create Replacement
		  /// <remarks>
        /// When you start using Box2D, you will have literally no use for this
        /// class; all of this information is contained in the b2Body, b2Fixture,
        /// b2BodyDef, and b2FixtureDef classes.
        /// </remarks>
        class PhysicalProperties {
        public:
          PhysicalProperties() {}
          // INLINED METHODS
		  void initBody(b2World* gameWorld) {
			  b2BodyDef playerBodyDef;
			  playerBodyDef.type = b2_dynamicBody;
			  playerBodyDef.position.Set(100,100);
			  body = gameWorld->CreateBody(&playerBodyDef);
			  body->SetFixedRotation(true);
			  b2PolygonShape playerBox;
			  playerBox.SetAsBox(32.0f, 32.0f);
			  b2FixtureDef fixtureDef;
			  fixtureDef.shape = &playerBox;
			  fixtureDef.density = 1.0f;
			  fixtureDef.friction = 0.3f;
			  body->CreateFixture(&fixtureDef);
			  body->SetActive(true);
          }

		  float getMass() const { return body->GetMass(); }

		  float getX() const { return body->GetPosition().x; }
		  float getY() const { return body->GetPosition().y; }
		

		  float getVelocityX() const { return body->GetLinearVelocity().x; }
		  float getVelocityY() const { return body->GetLinearVelocity().y; }

		  float getRotation() const { return body->GetAngle(); }

          void setMass(const float initMass) { 
			  b2MassData* data;
			  body->GetMassData(data);

			  data->mass = initMass;

			  body->SetMassData(data);
		  }

          void setPosition(const float initX, const float initY) {
			  b2Vec2 pos;
			  pos.x = initX;
			  pos.y = initY;

			  float32 angle = body->GetAngle();

			  body->SetTransform(pos, angle);
          }

		  void setPosition(b2Vec2 pos){
			  body->SetTransform(pos, body->GetAngle());
		  }

          void setVelocity(const float initVx, const float initVy) {
			  b2Vec2 vel;
			  vel.x = initVx;
			  vel.y = initVy;

			  body->SetLinearVelocity(vel);
          }

		  void setVelocity(b2Vec2 vel){
			  body->SetLinearVelocity(vel);
		  }

		  

		  void setUserData(void* data) {
			  body->SetUserData(data);
          }

		  void* getUserData() {
			  return body->GetUserData();
          }

          void applyForce(const float forceX, const float forceY, const float posX, const float posY, bool wake) {
			  b2Vec2 force;
			  force.x = forceX;
			  force.y = forceY;
			  b2Vec2 pos;
			  pos.x = posX;
			  pos.y = posY;
			  body->ApplyForce(force, pos, wake);
		  }

		  void applyForce(b2Vec2 force, b2Vec2 pos, bool wake){
			  body->ApplyForce(force, pos, wake);
		  }


		  //METHONDS TO BE DELETED
		  float getZ() const { return 0.f; }
		  
		  void applyAcceleration() {

		  }

		  void setAcceleration(float a, float b) {
	          
          }

		  void incVelocity(float a, float b) {

		  }

		  void setX(float a){}
		  void setY(float b){}

        private:
			b2Body* body;
        };
      }
    }
  }
}
