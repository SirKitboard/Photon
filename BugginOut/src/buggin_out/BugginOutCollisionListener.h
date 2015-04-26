#pragma once

#include "sssf/gsm/physics/CollisionListener.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace gsm {
      namespace physics { class Collision; }
    }
  }

  namespace bugginout {
    using sssf::game::Game;
    using namespace sssf::gsm::physics;

    class BugginOutCollisionListener : public CollisionListener {
    public:
      void respondToCollision(Game *, Collision *) override;
    };
  }
}