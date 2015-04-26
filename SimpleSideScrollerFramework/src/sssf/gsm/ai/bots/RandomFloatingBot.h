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
          
          class RandomFloatingBot : public Bot {
          public:
            RandomFloatingBot(physics::Physics& physics,
              unsigned int initMin,
              unsigned int initMax,
              unsigned int initMaxVelocity);
            virtual ~RandomFloatingBot() {}
            void initBot(unsigned int initMin,
              unsigned int initMax,
              unsigned int initMaxVelocity);
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

            // THIS PRIVATE CONSTRUCTOR IS ONLY USED FOR CLONING
            RandomFloatingBot(unsigned int initMin,
              unsigned int initMax,
              unsigned int initMaxVelocity);
          };
        }
      }
    }
  }
}
