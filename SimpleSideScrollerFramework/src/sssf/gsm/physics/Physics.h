/*
    Author: Richard McKenna

    Physics.h
    */

#pragma once

#include <list>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "sssf/gsm/physics/CollidableObject.h"

const float EPSILON = 0.00001f;
const float DEFAULT_GRAVITY = 0.3f;
const float BUFFER_BETWEEN_OBJECTS = 0.2f;
const float NUDGE_VELOCITY = 0.3f;
const float ENERGY_LOSS = 0.95f;

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }

    namespace gsm {

      namespace world {
        struct Tile;
        class World;
      }
      namespace physics {
        class Collision;
		class CollisionListener;

        using std::list;
        using std::unordered_map;
        using std::unordered_set;
        using std::vector;

        using world::Tile;
        using world::World;

		using game::Game;

		//TODO: Create Replacement
        /// <summary>
        /// Manages all game collisions and physics, including collision
        /// detection and resolution. Each frame we will update the collided
        /// objects velocities and positions accordingly.
        /// </summary>
        /// <remarks>
        /// When you start using Box2D, this class as it is will be almost
        /// completely gutted; you'll likely want to make this a wrapper
        /// around a b2World.
        /// </remarks>
        class Physics {
        public:
          /**
           * Initializes the data structures and loads the necessary ones with
           * recyclable objects for collision tests.
           */
          Physics();

          /// Deletes all the recyclable objects.
          ~Physics();

          // No copying allowed!
          Physics(const Physics&) = delete;
          Physics& operator=(const Physics&) = delete;

          // INLINED GET/SET METHODS
          float getGravity() const { return gravity; }

          CollisionListener* getCollisionListener() {
            return collisionListener;
          }

          const CollisionListener* getCollisionListener() const {
            return collisionListener;
          }

          bool isActivated() const { return activated; }
          bool isActivatedForSingleUpdate() const { return activatedForSingleUpdate; }
          void activateForSingleUpdate(const bool on = true) {
            activatedForSingleUpdate = on;
          }

          void setGravity(const float g) { gravity = g; }

          void setCollisionListener(CollisionListener* cl) { collisionListener = cl; }

          /**
           * Informs the physics system of additional dynamic, collidable object
           * to keep track of. Note that this method should not be used mid-collision,
           * as a result of a collition event, since it does not update all the
           * swept shape positions. Also note that it does not test to see if the
           * added object overlaps an object already in the game.
           */
          void addCollidableObject(CollidableObject*);

          /**
           * Called when a collision is detected between a world tile and a sprite,
           * this records that collision so it can be resolved later if needed.
           */
          void addTileCollision(
            CollidableObject* dynamicObject,
            Tile* tile,
            const float tileX,
            const float tileY,
            const float tileWidth,
            const float tileHeight
            );

          /**
           * Removes a dynamic, collidable object from the physics system. Note
           * that this method should not be used mid-collision, as a result of a
           * collision event, since it does not update all the swept shape
           * positions. Also note that it does not test to see if the added object
           * overlaps an object already in the game.
           */
          void removeCollidableObject(CollidableObject*);

          void togglePhysics() { activated = !activated; }

          /**
           * This is where all game physics starts each frame. It is called each
           * frame by the game state manager after player input and AI have been
           * processed. It updates the physical state of all dynamic objects in
           * the game and moves all objects to their end of frame positions,
           * updates all necessary object velocities, and calls all collision event handlers.
           */
          void update(Game*);

          /**
           * This method helps us employ walking and jumping on/from tiles. It is
           * a look-ahead function which basically tells us if the sprite will
           * collide on top of the tile's AABB.
           */
          bool willSpriteCollideOnTile(const CollidableObject&, const AABB&) const;

          void clear();
        private:
          // THIS IS APPLIED TO EVERY SPRITE EVERY FRAME
          float gravity;

          // THIS IS THE TIME IN THIS FRAME THAT HAS PASSED SO FAR,
          // NOT THAT ALL GAME OBJECTS WILL BE BROUGHT UP TO THIS MOMENT
          float currentCollisionTime;

          // THIS IS THE CUSTOM EVENT HANDLER FOR PROVIDING GAME-SPECIFIC
          // EVENTS TO COLLISIONS BETWEEN GAME OBJECTS
          CollisionListener* collisionListener;

          vector<Collision*> recycledCollisions;
          vector<CollidableObject*> recycledCollidableObjectsList;

          // WE USE A LIST BECAUSE WE'LL NEED TO PERIODICALLY REMOVE
          // FROM THE CENTER, AND WHILE WE DO SORT IT, WE DON'T SEARCH
          // IT FOR SINGLE ELEMENTS
          list<Collision*> activeCollisions;

          // SWEEP AND PRUNE STUFF
          unordered_map<unsigned int, vector<CollidableObject*>> sortedSweptShapes;

          // THIS MAKES SURE WE AREN'T CHECKING A SLIGHLY PENETRATING COLLISION
          // PAIR OVER AND OVER AGAIN. THAT SCENARIO IS VERY HARD TO AVOID
          // IN A CONTINUOUS SYSTEM
          unordered_map<CollidableObject*, unordered_set<Tile*>> spriteToTileCollisionsThisFrame;

          // USED FOR TESTING PHYSICS BY TURNING IT ON AND OFF IN VARIOUS WAYS
          bool activated : 1;
          bool activatedForSingleUpdate : 1;

          // HELPER METHODS DEFINED INSIDE Physics.cpp - ADD MORE AS YOU SEE FIT

          /**
           * This tricky bugger moves all the sprites to the end of this frame's
           * time. This is tricky because it can produce problems to occur early
           * the next frame if we're not careful. To some degree this is a blind
           * move, so we scale the time a little to try to prevent errors. In
           * reality we should still check after moving sprites if they are in a
           * collided state (this is all due to simultaeous collision problems)
           * and undo such problems, but that vastly complicates things. We'll see
           * how Box2D deals with this.
           */
          void moveAllSpritesToEndOfFrame();

          /**
           * Moves all sprites ahead per their current velocities using the
           * timeStep variable.
           */
          void moveAllSpritesUpByTimeStep(const float);

          /**
           * Done each frame before collision testing, it updates the "on tile"
           * states and then applies acceleration and gravity to the sprite's
           * velocity. Then it initializes the swept shape for the sprite.
           */
          void prepSpriteForCollisionTesting(World& world, CollidableObject& sprite);

          /**
           * When we resolve a collision involving a sprite other collisions
           * involving it are then obsolete, so this method removes them.
           */
          void removeActiveCOCollisions(CollidableObject* co);

          /**
           * Makes sure the co sprite is in its proper sorted location in the
           * ordering sweep and prune data structure (i.e. the one for LEFT_EDGE,
           * RIGHT_EDGE, etc.), looking only in either the increasing or
           * decreasing direction (according to the bool argument).
           */
          void reorderSweptShape(CollidableObject*, CollisionEdge, bool);

          /**
           * Helper method for making sure the sprite is in its correct sweep and
           * prune data structures location.
           */
          void reorderCollidableObject(CollidableObject* co);

          void getAllTileCollisionsForAGivenSprite(
            World& world,
            CollidableObject& sprite,
            float percentageOfFrameRemaining
            );

          void updateSweptShapeIndices(
            vector<CollidableObject*>& sweptShapes,
            CollisionEdge ordering
            );

          void moveAllSpritesUpToBufferedTimeOfCollision(Collision* collision);
          void retrieveSpriteCollisions();
          void removeCollidableObject(CollisionEdge ordering, CollidableObject* co);
          void updateSweptShapeIndices();
        };

        /*
            Provides a custom criteria for sorting for our swepts shapes when
            we want the sorted by left edge.
            */
        struct SweptShapesComparitorByLeft {
          inline bool operator()(CollidableObject* a, CollidableObject* b) const {
            AABB& aSH = a->getSweptShape();
            AABB& bSH = b->getSweptShape();
            return aSH.getLeft() < bSH.getLeft();
          }
        };

        /*
            Provides a custom criteria for sorting for our swepts shapes when
            we want the sorted by right edge.
            */
        struct SweptShapesComparitorByRight {
          inline bool operator()(CollidableObject* a, CollidableObject* b) const {
            AABB& aSH = a->getSweptShape();
            AABB& bSH = b->getSweptShape();
            return aSH.getRight() < bSH.getRight();
          }
        };
      }
    }
  }
}
