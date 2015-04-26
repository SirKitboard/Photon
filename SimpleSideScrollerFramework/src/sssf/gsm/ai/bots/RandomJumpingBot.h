#pragma once

#include <cstdint>

#include "sssf/gsm/ai/Bot.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace physics { class Physics; }
      namespace ai {
        namespace bots {
          class RandomJumpingBot : public Bot {
          public:
			  RandomJumpingBot(b2World* gameWorld, physics::Physics& physics,
              unsigned int initMin,
              unsigned int initMax,
              unsigned int initMaxVelocity);
            virtual ~RandomJumpingBot() {}
            void initBot(unsigned int initMin,
              unsigned int initMax,
              unsigned int initMaxVelocity);
            void    pickRandomCyclesInRange();
            void    pickRandomJump(physics::Physics& physics);
            void    think(game::Game* game) override;
            size_t type() const override { return TYPE; }

            static const size_t TYPE = 1;

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
            unsigned int cyclesRemainingBeforeThinking;
            unsigned int minCyclesBeforeThinking;
            unsigned int maxCyclesBeforeThinking;
            unsigned int maxVelocity;
            unsigned int animationRandomizer;

            // This private constructor is only to be used for cloning bots, note
            // that this does not setup the velocity for this bot.
            RandomJumpingBot(unsigned int initMin,
              unsigned int initMax,
              unsigned int initMaxVelocity);
          };
        }
      }
    }
  }
}
