#pragma once

#include <array>

#include "sssf/gsm/physics/AABB.h"
#include "sssf/gsm/physics/PhysicalProperties.h"
#include <sssf/audio/GameAudio.h>
#include "sssf/game/Game.h"

namespace cse380 {
	namespace sssf {
		namespace gsm {
			namespace physics {
				class CollisionHandler : public b2ContactListener {
					Game* game;
				public:
					CollisionHandler(Game* game) {
						this->game = game;
					}
					void BeginContact(b2Contact* contact) override{
						void* userDataA = contact->GetFixtureA()->GetBody()->GetUserData();
						void* userDataB = contact->GetFixtureB()->GetBody()->GetUserData();
						if (userDataA && userDataB) {
							char* a = static_cast<char*>(userDataA);
							char* b = static_cast<char*>(userDataB);
							if (*a == 'p' && *b == 'w') {
								game->getAudio()->playSound(1);
							}
							else if (*b == 'p' && *a == 'w') {
								game->getAudio()->playSound(1);
							}
							else if (*a == 'p' && *b == 'b') {
								game->getAudio()->playSound(1);
								game->levelLose();
							}
							else if (*b == 'p' && *a == 'b') {
								game->getAudio()->playSound(2);
								game->levelLose();
							}
						}

					}

				};
			}
		}
	}
}