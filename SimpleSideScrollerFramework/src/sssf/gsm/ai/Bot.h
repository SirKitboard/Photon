#pragma once

#include <cstdint>

#include "sssf/gsm/sprite/AnimatedSprite.h"

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }

    namespace gsm {
      namespace ai {

        /// <summary>
        /// A game object that thinks and makes decisions, like how to move,
        /// how to use items, etc. You should probably make a subclass for each
        /// type of AI/NPC/enemy/etc.
        /// </summary>
        /// <remarks>
        /// Of course, that doesn't mean that you should use subclasses for games;
        /// ask Jesse and he'll rant on and on about what you should do instead.
        /// </remarks>
        class Bot : public sprite::AnimatedSprite {
        public:
          Bot() {}
          virtual ~Bot() {}

          // TO BE DEFINED BY BOT AI CLASSES
          virtual void think(game::Game* game) = 0;

          /// <summary>
          /// Every subclass should return its own constant value (i.e. every
          /// RandomJumpingBot might return 5, every RandomFloatingBot might
          /// return 37, etc.).  Identifying bot types this way is faster than
          /// using dynamic_cast or similar.
          /// </summary>
          virtual size_t type() const = 0;

        protected:
          virtual Bot* clone() const = 0;

          friend class BotRecycler;
        };
      }
    }
  }
}
