#pragma once

#include <array>

#include "sssf/gsm/physics/AABB.h"
#include "sssf/gsm/physics/PhysicalProperties.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace physics {
        using std::array;

        // THESE ARE THE SWEEP AND PRUNE ORDERINGS
        enum CollisionEdge {
          LEFT_EDGE = 0,
          RIGHT_EDGE,
          TOP_EDGE,
          BOTTOM_EDGE
        };

		//TODO: Create Replacement
        /// <remarks>
        /// When you start to use Box2D, you might not find this class useful
        /// any more; you could strip it down to just a wrapper around a b2Body.
        /// Hell, you could even just scrap this entirely and make AnimatedSprite
        /// the top-most class of game objects.
        /// </remarks>
        class CollidableObject {
        public:
          CollidableObject() {}
          CollidableObject(const CollidableObject&) = delete;
          CollidableObject& operator=(const CollidableObject&) = delete;
          virtual ~CollidableObject() {}

          /**
           * Updates the position and dimensions of this dynamic object's swept
           * shape based on how much time is left during this frame.
           */
          void updateSweptShape(const float percentageOfFrameTimeRemaining);

          // INLINED METHODS
          bool isCurrentlyCollidable() const { return currentlyCollidable; }
          bool isOnTileThisFrame() const { return onTileThisFrame; }
          bool wasOnTileLastFrame() const { return onTileLastFrame; }
          AABB& getBoundingVolume() { return boundingVolume; }
          const AABB& getBoundingVolume() const { return boundingVolume; }
          AABB& getSweptShape() { return sweptShape; }
          const AABB& getSweptShape() const { return sweptShape; }
          PhysicalProperties& getPhysicalProperties() { return pp; }
          const PhysicalProperties& getPhysicalProperties() const { return pp; }
          CollisionEdge getCollisionEdge() const { return collisionEdge; }
          void setCollisionEdge(const CollisionEdge edge) { collisionEdge = edge; }

          unsigned int getSweepAndPruneIndex(const unsigned int ordering) const {
            return sweepAndPruneIndices[ordering];
          }

          void setSweepAndPruneIndex(const unsigned int ordering,
            const unsigned int index) {
            sweepAndPruneIndices[ordering] = index;
          }

          void setCurrentlyCollidable(const bool on) { currentlyCollidable = on; }
          void setOnTileThisFrame(const bool on) { onTileThisFrame = on; }
          void setOnTileLastFrame(const bool on) { onTileLastFrame = on; }

          void advanceOnTileStatus() {
            onTileLastFrame = onTileThisFrame;
            onTileThisFrame = false;
          }
        protected:
          AABB boundingVolume;
          AABB sweptShape;
          PhysicalProperties pp;
          array<unsigned int, 4> sweepAndPruneIndices;
          bool currentlyCollidable : 1;
          bool onTileThisFrame : 1;
          bool onTileLastFrame : 1;
          CollisionEdge collisionEdge : 2;
        };
      }
    }
  }
}
