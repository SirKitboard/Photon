#pragma once

#include <Box2D/Box2D.h>

class Box2DWorld {
private:
	b2World *physicalWorld;
public:
	Box2DWorld() {
		b2Vec2 gravity(0.0f, 0.0f);
		physicalWorld = new b2World(gravity);
	}
	b2World* getWorld() {
		return physicalWorld;
	}
};
		