#pragma once

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }

    namespace gsm {
      namespace physics {
        using sssf::game::Game;
        class Collision;

		//TODO: Create Replacement
        /// <remarks>
        /// When you start using Box2D, you will no longer need this class; you
        /// will instead be using b2CollisionListener.
        /// </remarks>
        class CollisionListener {
        public:
          virtual ~CollisionListener() {}
          virtual void respondToCollision(Game*, Collision*) = 0;
        };
      }
    }
  }
}
