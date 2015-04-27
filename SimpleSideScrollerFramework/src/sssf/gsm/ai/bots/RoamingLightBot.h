#pragma once

// PREPROCESSOR INCLUDE STATEMENTS

#include <cstdint>
#include <random>

#include "sssf/gsm/ai/Bot.h"

namespace cse380 {
	namespace sssf {

		namespace game { class Game; }
		namespace gsm {

			namespace physics { class Physics; }

			namespace ai {
				namespace bots {
					using std::uniform_real_distribution;

					class RoamingLightBot : public Bot {
					public:
						RoamingLightBot(game::Game* game,
							unsigned int initMin,
							unsigned int initMax,
							unsigned int initMaxVelocity,
							unsigned int direction);
						virtual ~RoamingLightBot() {}
						void initBot(unsigned int initMin,
							unsigned int initMax,
							unsigned int initMaxVelocity,
							unsigned int direction);
						void    pickRandomCyclesInRange();
						void    pickRandomVelocity(physics::Physics& physics);
						void    think(game::Game* game) override;
						size_t type() const override { return TYPE; }

						static const size_t TYPE = 2;

						// INLINED METHODS
						unsigned int getCyclesRemainingBeforeThinking() const {
							return cyclesRemainingBeforeThinking;
						}
						unsigned int getMinCyclesBeforeThinking() const {
							return minCyclesBeforeThinking;
						}
						unsigned int getMaxCyclesBeforeThinking() const {
							return maxCyclesBeforeThinking;
						}
					protected:
						Bot*    clone() const override;
					private:
						static uniform_real_distribution<float> _random;
						unsigned int cyclesRemainingBeforeThinking;
						unsigned int minCyclesBeforeThinking;
						unsigned int maxCyclesBeforeThinking;
						unsigned int maxVelocity;
						unsigned int direction;

						// THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
						RoamingLightBot(unsigned int initMin,
							unsigned int initMax,
							unsigned int initMaxVelocity,
							unsigned int direction);
					};
				}
			}
		}
	}
}
