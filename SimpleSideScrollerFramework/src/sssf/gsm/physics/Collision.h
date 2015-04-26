#pragma once

namespace cse380 {
  namespace sssf {
    namespace gsm {

      namespace world { struct Tile; }

      namespace physics {

        class CollidableObject;

		//TODO: Create Replacement
        /// <remarks>
        /// When you start using Box2D, you will have no further need for this
        /// class; you'll be using b2Collision instead.
        /// </remarks>
        class Collision {
        private:
          CollidableObject* co1;
          CollidableObject* co2;
          Tile* tile;
          float timeOfCollision;
          float startTimeOfXCollision;
          float startTimeOfYCollision;
          CollisionEdge co1Edge : 2;
          CollisionEdge co2Edge : 2;
          bool collisionWithTile : 1;
        public:
          Collision() {}
          ~Collision() {}

          // INLINED METHODS
          CollidableObject* getCO1() { return co1; }
          const CollidableObject* getCO1() const { return co1; }

          CollidableObject* getCO2() { return co2; }
          const CollidableObject* getCO2() const { return co2; }

          CollisionEdge getCO1Edge() const { return co1Edge;  }
          CollisionEdge getCO2Edge() const {  return co2Edge;  }

          Tile* getTile() { return tile; }
          const Tile* getTile() const { return tile; }

          float getTimeOfCollision() const { return timeOfCollision; }
          float getStartTimeOfXCollision() const { return startTimeOfXCollision; }
          float getStartTimeOfYCollision() const { return startTimeOfYCollision; }

          bool isCollisionWithTile() const { return collisionWithTile; }

          void setCO1(CollidableObject* initCO1) { co1 = initCO1; }
          void setCO2(CollidableObject* initCO2) { co2 = initCO2; }

          void setCO1Edge(const CollisionEdge edge) { co1Edge = edge; }
          void setCO2Edge(const CollisionEdge edge) { co2Edge = edge;  }

          void setCollisionWithTile(const bool tile) { collisionWithTile = tile; }

          void setTile(Tile* initTile) { tile = initTile; }

          void setTimeOfCollision(const float time) { timeOfCollision = time; }
          void setStartTimeOfXCollision(const float time) {  startTimeOfXCollision = time; }
          void setStartTimeOfYCollision(const float time) { startTimeOfYCollision = time; }

          bool operator<(const Collision& other) const {
            return this->getTimeOfCollision() < other.getTimeOfCollision();
          }
        };

        struct CollisionComparitor {
          inline bool operator()(const Collision* a, const Collision* b) const {
            return b->getTimeOfCollision() < a->getTimeOfCollision();
          }
        };
      }
    }
  }
}
