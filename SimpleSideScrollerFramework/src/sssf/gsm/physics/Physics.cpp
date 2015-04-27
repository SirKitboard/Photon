/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Physics.cpp

    See Physics.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/physics/Physics.h"

#include "sssf/gsm/sprite/AnimatedSprite.h"
#include "sssf/gsm/sprite/AnimatedSpriteType.h"
#include "sssf/gsm/physics/Collision.h"
#include "sssf/gsm/physics/CollidableObject.h"
#include "sssf/gsm/physics/CollisionListener.h"
#include "sssf/game/Game.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gsm/world/SparseLayer.h"
#include "sssf/gsm/world/TiledLayer.h"
#include "sssf/gsm/world/Tile.h"
#include "sssf/gsm/world/World.h"
#include "sssf/gsm/world/WorldLayer.h"

#undef min
#undef max
// Don't need these macros

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace physics {
        using gsm::state::GameStateManager;
        using gsm::sprite::AnimatedSprite;
        using gsm::world::WorldLayer;

        // Helper functions (NOT class methods)
        void determineXAxisCollisionEdge(const AABB&, const AABB&,
          CollisionEdge&, CollisionEdge&);

        void determineYAxisCollisionEdge(const AABB&, const AABB&,
          CollisionEdge&, CollisionEdge&);
        float calculateTimeToImpact(float, float, float, float, float);
        void performCollisionResponse(Collision* collision);

        /// Helper function for getting an edge coordinate.
        float getSide(const CollidableObject& co, CollisionEdge ordering);
        float calculateTimeUntilCollision(
          CollidableObject* co1,
          CollidableObject* co2,
          CollisionEdge& co1Edge,
          CollisionEdge& co2Edge,
          float bufferBetweenObjects
          );
        // End helper function declarations

        Physics::Physics() :
          gravity(DEFAULT_GRAVITY),
          activated(true) {
          // DEFAULT GRAVITY IS 1.0f
          gravity = DEFAULT_GRAVITY;
          activated = true;

          // POPULATE THEM WITH 1000 OBJECTS TO SHARE
          // WHY 1000? BECAUSE I HAD TO PICK SOME NUMBER BIG ENOUGH
          // THAT WE'LL NEVER REACH, NOT THE MOST EFFICIENT TECHNIQUE,
          // BUT WE CAN CUSTOMIZE IT VIA A BUDGET IF WE KNEW WHAT
          // THE GAME'S NEEDS ARE
//          for (int i = 0; i < 1000; i++) {
//            // THESE ARE DUMMY OBJECTS USED FOR TILES, SINCE
//            // TILES DON'T KNOW THEIR OWN LOCATIONS
//            CollidableObject* co = new CollidableObject();
//            co->setCurrentlyCollidable(true);
//            co->getPhysicalProperties().setVelocity(0.0f, 0.0f);
//            //recycledCollidableObjectsList.push_back(co);
//
//            // THESE ARE ALL THE COLLISIONS WE'LL USE
//            Collision* c = new Collision();
//            recycledCollisions.push_back(c);
//          }

          // NOW MAKE THE SWEEP AND PRUNE VECTORS. AGAIN, THESE GUYS KEEP ALL
          // SPRITE OBJECTS SORTED BY LEFT AND RIGHT SWEPT SHAPE EDGES SUCH THAT
          // WE MAY EASILY TEST FOR NEIGHBOR TO NEIGHBOR SPRITE COLLISIONS RATHER
          // THAN DO SOME CRAZY N! COMPUTATION
          sortedSweptShapes[LEFT_EDGE] = vector<CollidableObject*>();
          sortedSweptShapes[RIGHT_EDGE] = vector<CollidableObject*>();
        }

        Physics::~Physics() {
          this->clear();
        }

        void Physics::addCollidableObject(CollidableObject* collidableObjectToAdd) {
          spriteToTileCollisionsThisFrame[collidableObjectToAdd] = unordered_set<Tile*>();
          sortedSweptShapes[LEFT_EDGE].push_back(collidableObjectToAdd);
          sortedSweptShapes[RIGHT_EDGE].push_back(collidableObjectToAdd);
        }

        void Physics::removeCollidableObject(CollidableObject*
          collidableObjectToRemove) {
          auto tileSetToRemoveIt = spriteToTileCollisionsThisFrame.find(
            collidableObjectToRemove
            );
          spriteToTileCollisionsThisFrame.erase(tileSetToRemoveIt);
          removeCollidableObject(LEFT_EDGE, collidableObjectToRemove);
          removeCollidableObject(RIGHT_EDGE, collidableObjectToRemove);
        }

        void Physics::addTileCollision(CollidableObject* dynamicObject, Tile* tile,
          float tileX, float tileY, float tileWidth, float tileHeight) {
          // IF WE'VE ALREADY HANDLED A COLLISION BETWEEN THESE TWO OBJECTS THIS
          // FRAME THEN IGNORE IT
          unordered_set<Tile*>& doTiles = spriteToTileCollisionsThisFrame[dynamicObject];
          if (doTiles.find(tile) != doTiles.end())
            return;

          // GET A DUMMY COLLIABLE OBJECT TO USE FOR THE TILE
          CollidableObject* tileInfoForCollision = recycledCollidableObjectsList.back();

          // FILL IT WITH DATA
          AABB& bv = tileInfoForCollision->getBoundingVolume();
          bv.setCenter(tileX + (tileWidth / 2), tileY + (tileHeight / 2));
          bv.setSize(tileWidth, tileHeight);

          // FIRST WE'RE GOING TO DO A MORE NARROW CHECK TO SEE IF THE dynamicObject
          // REALLY DOES COLLIDE WITH THE TILE. TO DO SO WE'LL CALCULATE THE TIME
          // OF COLLISION. IF IT HAPPENS AFTER THIS FRAME IS OVER (> 1), THEN WE
          // WILL IGNORE IT
          CollisionEdge co1Edge, co2Edge;
          float timeUntilCollision = calculateTimeUntilCollision(dynamicObject,
            tileInfoForCollision, co1Edge, co2Edge, 0.0f);
          if (timeUntilCollision > 1.0f)
            return;

          // IF IT MADE IT HERE, A COLLISION IS HAPPENING
          // AND REMOVE IT FROM THE RECYCLING CENTER
          recycledCollidableObjectsList.pop_back();

          // NOW LET'S MAKE A COLLISION FOR THE TILE-SPRITE
          Collision* collisionToAdd = recycledCollisions.back();
          collisionToAdd->setCO1(dynamicObject);
          collisionToAdd->setCO2(tileInfoForCollision);
          collisionToAdd->setCO1Edge(co1Edge);
          collisionToAdd->setCO2Edge(co2Edge);
          collisionToAdd->setCollisionWithTile(true);
          collisionToAdd->setTimeOfCollision(timeUntilCollision);
          collisionToAdd->setTile(tile);
          recycledCollisions.pop_back();
          activeCollisions.push_back(collisionToAdd);
        }

        void Physics::update(Game* game) {
			float32 timestep = 1.0f / 60.0f;
			int32 velIt = 6;
			int32 posit = 2;
			int xvel = game->getGSM().getSpriteManager().getPlayer().getPhysicalProperties().getVelocityX();
			int yvel = game->getGSM().getSpriteManager().getPlayer().getPhysicalProperties().getVelocityY();
			game->getGameWorld()->Step(timestep,velIt,posit);
//          // WE'LL USE A CONTINUOUS COLLISION SYSTEM TO ENSURE TEMPORAL
//          // COHERENCE, WHICH MEANS WE'LL MAKE SURE COLLISIONS ARE RESOLVED
//          // IN THE ORDER WITH WHICH THEY HAPPEN. WE DON'T WANT GAME EVENTS
//          // TO APPEAR TO HAPPEN IN THE WRONG ORDER. WE'LL TRY TO MAKE IT
//          // A LITTLE MORE EFFICIENT BY EMPLOYING A VARIATION ON THE
//          // SWEEP AND PRUNE ALGORITHM FOR DYNAMIC-DYNAMIC OBJECT COLLISIONS
//
//          // IN CASE WE'RE DOING THE ONE UPDATE STEP AT A TIME
//          // THIS MAKES SURE THE UPDATE DOESN'T GET CALLED AGAIN
//          // NEXT FRAME WITHOUT THE USER EXPLICITY REQUESTING IT
//          // BY PRESSING THE 'T' KEY (for Time sTep)
//          activatedForSingleUpdate = false;
//
//          // WE'LL NEED THE WORLD TO ACCESS THE SPRITES AND WORLD LAYERS
//          GameStateManager& gsm = game->getGSM();
//          World& world = gsm.getWorld();
//
//          // NOTE THAT WE MAKE SURE THE activeCollisions VECTOR IS
//          // EMPTIED BEFORE THIS METHOD EXITS, SO WE CAN ASSUME
//          // IT'S EMPTY NOW. activeCollisions CONTAINS ALL THE COLLISIONS
//          // DETECTED SO FAR THIS FRAME. THESE ARE THE THINGS WE MUST
//          // RESOLVE.
//
//          // START THE CLOCK AT 0, THAT MEANS 0% OF THE WAY THROUGH THE FRAME.
//          // NOTE THAT TIME 0 IS THE MOST DANGEROUS TIME FOR DETECTING COLLISIONS
//          // BECAUSE THEY CAN BE EASILY OVERLOOKED. THE SAME FOR SIMULTANEOUS
//          // COLLISIONS. TO MINIMIZE RIGID BODY PENETRATION, SUCH CIRCUMSTANCES
//          // ARE TYPICALLY HANDLED AS SPECIAL CASES
//          currentCollisionTime = 0.0f;
//
//          // FIRST WE NEED TO DO COLLISION TESTING PREP WORK FOR SPRITES
//          // APPLY ACCELERATION AND GRAVITY TO VELOCITY
//          // INIT TILE COLLISION INFO
//          // SET ON TILE LAST FRAME USING ON TILE THIS FRAME
//          // SET ON TILE THIS FRAME TO FALSE
//          // GET COLLISIONS WITH ALL TILES TO HAPPEN DURING THIS FRAME
//          // PUT THESE COLLISIONS INTO A SORTABLE DATA STRUCTURE
//          // INIT SPRITE'S SWEPT SHAPE INFO
//
//          // FOR ALL SPRITES, INCLUDING THE BOTS AND PLAYER
//          for (CollidableObject* sprite : sortedSweptShapes[LEFT_EDGE]) {
//            prepSpriteForCollisionTesting(world, *sprite);
//            //getAllTileCollisionsForAGivenSprite(world, *sprite, 1.0f);
//          }
//
//          // PREPARE FOR SPRITE-SPRITE COLLISION TESTING
//
//          // SWEEP AND PRUNE DATA STRUCTURES PREP WORK
//          // SORT S_AND_P VECTOR SORTED BY START X OF SWEPT SHAPE
//          // SORT S_AND_P VECTOR SORTED BY END X OF SWEPT SHAPE
//          // WE DON'T NEED THE Y-AXIS SORTED, BUT WOULD IF THIS
//          // WERE A 3D SYSTEM TO SAVE ON COMPARISONS.
//
//          // WE'RE USING std::sort AND ARE PROVIDING A CUSTOM MEANS FOR COMPARISON
//          sort(sortedSweptShapes[LEFT_EDGE].begin(),
//            sortedSweptShapes[LEFT_EDGE].end(), SweptShapesComparitorByLeft());
//          sort(sortedSweptShapes[RIGHT_EDGE].begin(),
//            sortedSweptShapes[RIGHT_EDGE].end(), SweptShapesComparitorByRight());
//
//          // RECORD SORTED POSITIONS WITH EACH SPRITE. THEY NEED TO KNOW WHERE
//          // THEY ARE IN THOSE DATA STRUCTURES SUCH THAT WE CAN JUMP INTO
//          // THOSE DATA STRUCTURES TO TEST COLLISIONS WITH NEIGHBORS
//          updateSweptShapeIndices();
//
//          // YOU'LL NEED TO TEST FOR SPRITE-TO-SPRITE COLLISIONS HERE
//
//          // *** LOOP STARTS HERE. WE'LL DO THIS UNTIL THERE ARE NO
//          // MORE COLLISIONS TO RESOLVE FOR THIS FRAME
//          while (activeCollisions.size() > 0) {
//            // SORT COLLISION OBJECTS BY TIME OF COLLISION
//            // NOTE THAT I'M JUST EMPLOYING THE STL'S List
//            // CLASS' SORT METHOD BY PROVIDING MY OWN
//            // MEANS FOR COMPARING Collision OBJECTS
//            activeCollisions.sort();
//
//            // GET FIRST COLLISION - NOTE THAT WE HAVE THE COLLISIONS SORTED
//            // IN DESCENDING ORDER, SO TO TAKE THE EARLIEST ONE, WE REMOVE IT
//            // FOM THE BACK OF THE SORTED LIST
//            Collision* earliestCollision = activeCollisions.back();
//            activeCollisions.pop_back();
//            float collisionTime = earliestCollision->getTimeOfCollision();
//
//            // MOVE ALL SPRITES UP TO TIME OF FIRST COLLISION USING
//            // APPROPRIATELY SCALED VELOCITIES
//            moveAllSpritesUpToBufferedTimeOfCollision(earliestCollision);
//
//            // AND ADVANCE COLLISION TIME
//            currentCollisionTime = collisionTime;
//
//            // AND UPDATE THE VELOCITIES OF THE SPRITE(S) INVOLVED IN THE COLLISION
//            performCollisionResponse(earliestCollision);
//
//            // EXECUTE COLLISION EVENT CODE
//            // TEST TO SEE TYPES OF OBJECTS AND APPROPRIATE RESPONSE
//            // ACCORDING TO CUSTOMIZED COLLISION EVENT HANDLER
//            collisionListener->respondToCollision(game, earliestCollision);
//
//            // FOR THE TWO OBJECTS INVOLVED IN THE COLLISION REMOVE ALL OTHER
//            // COLLISIONS INVOLVING THEM SINCE THEY ARE NOW OBSOLETE. THE REASON
//            // BEING THE OBJECT COLLISION NOW LIKELY HAS A DIFFERENT VECTOR
//
//            // UPDATE THEIR SWEPT SHAPES AND TEST THEM AGAINST TILES AGAIN
//            CollidableObject* co1 = earliestCollision->getCO1();
//            CollidableObject* co2 = earliestCollision->getCO2();
//            removeActiveCOCollisions(co1);
//            co1->updateSweptShape(1.0f - currentCollisionTime);
//            getAllTileCollisionsForAGivenSprite(world, *co1, 1.0f - currentCollisionTime);
//
//            // ONLY DO IT FOR THE SECOND ONE IF IT'S NOT A TILE
//            if (!earliestCollision->isCollisionWithTile()) {
//              removeActiveCOCollisions(co2);
//              co2->updateSweptShape(1.0f - currentCollisionTime);
//              getAllTileCollisionsForAGivenSprite(world, *co2, 1.0f - currentCollisionTime);
//            }
//            else {
//              spriteToTileCollisionsThisFrame[co1].insert(earliestCollision->getTile());
//              recycledCollidableObjectsList.push_back(co2);
//            }
//
//            // NOW WE NEED TO SEE IF THE SPRITES INVOLVED IN THE JUST
//            // RESOLVED COLLISION ARE GOING TO BE INVOLVED IN ANY MORE
//            // WITH OTHER SPRITES BUT WE DON'T WANT TO CHECK ALL OF THEM,
//            // WE ONLY WANT TO CHECK NEIGHBORS, BUT FIRST WE HAVE TO
//            // MAKE SURE THE SPRITE(S) THAT WE JUST RESOLVED ARE IN THEIR
//            // PROPER SWEPT SHAPE LOCATIONS WITHOUT HAVING TO RESORT EVERTYHING
//
//            // IF IT WAS ONLY ONE SPRITE WITH A TILE THIS IS EASY TO DO
//            if (earliestCollision->isCollisionWithTile()) {
//              reorderCollidableObject(co1);
//            }
//            // YOU'LL HAVE TO WORRY ABOUT REORDERING STUFF FOR COLLISIONS
//            // BETWEEN TWO SPRITES
//
//            // NOW TEST NEIGHBORS OF SPRITES INVOLVED IN RESOLVED COLLISION
//            // AGAINST NEIGHBORS IN SWEPT SHAPE DATA STRUCTURES. YOU'LL HAVE
//            // TO FIGURE OUT HOW TO DO THIS AND HOW TO RESOLVE SUCH COLLISIONS
//
//            // RECYCLE THE COLLISION SINCE WE'RE NOW DONE WITH IT
//            recycledCollisions.push_back(earliestCollision);
//          }
//
//          // APPLY THE REMAINING TIME TO MOVE THE SPRITES. NOTE THAT
//          // THIS IS ACTUALLY A VERY RISKY, TRICKY STEP BECAUSE IT COULD
//          // MOVE OBJECTS ALMOST TO THE POINT OF COLLISION, WHICH MAY THEN
//          // BE DETECTED ALMOST AT TIME 0 NEXT FRAME. THERE ARE OTHER TRICKY
//          // ISSUES RELATED TO OUR BUFFER AS WELL, SO WE CHEAT A LITTLE HERE
//          // AND SCALE THE TIME REMAINING DOWN A LITTE
//          if (currentCollisionTime < 1.0f)
//            moveAllSpritesToEndOfFrame();
//
//          // INIT TILE COLLISION INFO
//          // SET ON TILE LAST FRAME USING ON TILE THIS FRAME
//          // SET ON TILE THIS FRAME TO FALSE
//          for (CollidableObject* sprite : sortedSweptShapes[LEFT_EDGE]) {
//            sprite->advanceOnTileStatus();
//          }
//
//          // WE'RE NOT GOING TO ALLOW MULTIPLE COLLISIONS TO HAPPEN IN A FRAME
//          // BETWEEN THE SAME TWO OBJECTS
//          spriteToTileCollisionsThisFrame.clear();
        }

        bool Physics::willSpriteCollideOnTile(const CollidableObject& co,
          const AABB& tileAABB) const {
          float yDiff = tileAABB.getTop() - co.getBoundingVolume().getBottom() -
            co.getPhysicalProperties().getVelocityY();
          return (yDiff < 0.0f);
        }

        void Physics::clear() {
          // WE ARE DONE WITH ALL OF THESE
          for (CollidableObject* co : recycledCollidableObjectsList) {
            delete co;
          }

          // AND THESE
          for (Collision* c : recycledCollisions) {
            delete c;
          }

          for (Collision* c : activeCollisions) {
            delete c;
          }
        }

        // PRIVATE HELPER METHODS

        void Physics::moveAllSpritesToEndOfFrame() {
          float endOfFrameTime = 1.0f;
          float timeStepToEnd = endOfFrameTime - currentCollisionTime;

          // MAKE SURE NONE OF THE SPRITES WILL BE MOVED PAST OUR BUFFER,
          // SO LET'S CHECK AND SEE WHAT THE FIRST TIME WILL BE WHERE SPRITES
          // WILL BE WITHIN THAT BUFFER AND MOVE EVERYTHING TO THEN

          moveAllSpritesUpByTimeStep(timeStepToEnd * 0.85f);
        }

        void Physics::moveAllSpritesUpByTimeStep(const float timeStep) {
          // GO THROUGH ALL THE SPRITES
          for (CollidableObject* sprite : sortedSweptShapes[LEFT_EDGE]) {
            if ((timeStep > 0.0f) && ((timeStep + currentCollisionTime) <= 1.0f)) {

              // SCALE THEIR VELOCITIES USING THE TIME STEP
              float incX = sprite->getPhysicalProperties().getVelocityX() * timeStep;
              float incY = sprite->getPhysicalProperties().getVelocityY() * timeStep;
              float distance = hypot(incX, incY);

              // AND MOVE THEM
              sprite->getBoundingVolume().incCenter(incX, incY);

              // MAKE SURE THEY GET RENDERED WHERE WE JUST MOVED THEM
              static_cast<AnimatedSprite*>(sprite)->correctToTightBoundingVolume();
            }
            // AND GO TO THE NEXT SPRITE
          }
        }

        void Physics::moveAllSpritesUpToBufferedTimeOfCollision(Collision* collision) {
          CollisionEdge co1Edge, co2Edge;
          float correctedCollisionTime = calculateTimeUntilCollision(collision->getCO1(),
            collision->getCO2(), co1Edge, co2Edge, BUFFER_BETWEEN_OBJECTS);

          // IF THE correctedCollisionTime is 0, WE HAVE TO BE CAREFUL
          // BECAUSE IT IS LIKELY A SIMULTAENOUS COLLISION WITH
          // ANOTHER ONE
          if (std::fabs(correctedCollisionTime) < EPSILON) {
            collision->setCO1Edge(co1Edge);
            collision->setCO2Edge(co2Edge);
            moveAllSpritesUpByTimeStep(correctedCollisionTime);
          }
        }

        void Physics::removeCollidableObject(CollisionEdge ordering,
          CollidableObject* co) {
          vector<CollidableObject*>& sweptShapes = sortedSweptShapes[ordering];
          unsigned int index = co->getSweepAndPruneIndex(ordering);
          sweptShapes.erase(sweptShapes.begin() + index);
        }

        void Physics::updateSweptShapeIndices() {
          updateSweptShapeIndices(sortedSweptShapes[LEFT_EDGE], LEFT_EDGE);
          updateSweptShapeIndices(sortedSweptShapes[RIGHT_EDGE], RIGHT_EDGE);
        }

        void Physics::updateSweptShapeIndices(vector<CollidableObject*>& sweptShapes,
          CollisionEdge ordering) {
          unsigned int counter = 0;
          for (CollidableObject* co : sweptShapes) {
            co->setSweepAndPruneIndex(ordering, counter);
            counter++;
          }
        }

        void Physics::prepSpriteForCollisionTesting(World& world,
          CollidableObject& sprite) {
          // THIS GUY HAS ALL THE PHYSICS STUFF FOR THE SPRITE
          PhysicalProperties& pp = sprite.getPhysicalProperties();

          // APPLY ACCELERATION
          pp.applyAcceleration();

          // APPLY GRAVITY
          pp.incVelocity(0.0f, gravity);

          // NOW, IF THE SPRITE WAS ON A TILE LAST FRAME LOOK AHEAD
          // TO SEE IF IT IS ON A TILE NOW. IF IT IS, UNDO GRAVITY.
          // THIS HELPS US AVOID SOME PROBLEMS
          if (sprite.wasOnTileLastFrame()) {
            // FIRST MAKE SURE IT'S SWEPT SHAPE ACCOUNTS FOR GRAVITY
            sprite.updateSweptShape(1.0f);

            // WE'LL LOOK THROUGH THE COLLIDABLE LAYERS
            vector<WorldLayer*>& layers = world.getLayers();
            for (WorldLayer* layer : layers) {
              if (layer->hasCollidableTiles()) {
                bool test = layer->willSpriteCollideOnTile(*this, sprite);
                if (test) {
                  sprite.setOnTileThisFrame(true);
                  pp.setVelocity(pp.getVelocityX(), 0.0f);
                  sprite.updateSweptShape(1.0f);
                  return;
                }
                else
                  std::cout << "What Happened?" << std::endl;
              }
            }
          }

          // INIT THE SWEPT SHAPE USING THE NEWLY APPLIED
          // VELOCITY. NOTE THAT 100% OF THE FRAME TIME
          // IS LEFT, DENOTED BY 1.0f
          sprite.updateSweptShape(1.0f);
        }

        float calculateTimeToImpact(float lower, float lowerV,
          float upper, float upperV,
          float bufferBetweenObjects) {
          float distance = (upper - lower) - bufferBetweenObjects;

          // IT'S ALREADY COLLIDING ON THIS AXIS
          if (distance < 0.0f)
            return 0.0f;
          // WE NEED A POSITIVE VELOCITY DIFFERENCE, SO
          // WE'LL SUBTRACT THE MIN FROM THE MAX
          float velocityDifference = std::max(lowerV, upperV) - std::min(lowerV, upperV);
          if (velocityDifference == 0.0f) {
            // IT WON'T BE COLLIDING
            return 2.0f;
          }
          float timeUntilOverlaps = distance / velocityDifference;
          return timeUntilOverlaps;
        }

        void Physics::getAllTileCollisionsForAGivenSprite(World& world,
          CollidableObject& sprite,
          float percentageOfFrameRemaining) {

          for (WorldLayer* layer : world.getLayers()) {
            if (layer->hasCollidableTiles())
              layer->findTileCollisionsForSprite(*this, sprite);
          }
        }

        float calculateTimeUntilCollision(CollidableObject* co1,
          CollidableObject* co2,
          CollisionEdge& co1Edge,
          CollisionEdge& co2Edge,
          float bufferBetweenObjects) {
          AABB& aabb1 = co1->getBoundingVolume();
          AABB& aabb2 = co2->getBoundingVolume();

          AABB* left, *right, *top, *bottom;
          float leftVx, rightVx, topVy, bottomVy;

          if (aabb1.getLeft() <= aabb2.getLeft()) {
            left = &aabb1;
            leftVx = co1->getPhysicalProperties().getVelocityX();
            right = &aabb2;
            rightVx = co2->getPhysicalProperties().getVelocityX();
          }
          else {
            left = &aabb2;
            leftVx = co2->getPhysicalProperties().getVelocityX();
            right = &aabb1;
            rightVx = co1->getPhysicalProperties().getVelocityX();
          }
          if (aabb1.getTop() <= aabb2.getTop()) {
            top = &aabb1;
            topVy = co1->getPhysicalProperties().getVelocityY();
            bottom = &aabb2;
            bottomVy = co2->getPhysicalProperties().getVelocityY();
          }
          else {
            top = &aabb2;
            topVy = co2->getPhysicalProperties().getVelocityY();
            bottom = &aabb1;
            bottomVy = co1->getPhysicalProperties().getVelocityY();
          }

          float startTimeOfCollisionX = calculateTimeToImpact(left->getRight(), leftVx,
            right->getLeft(), rightVx, bufferBetweenObjects);
          float endTimeOfCollisionX = calculateTimeToImpact(left->getLeft(), leftVx,
            right->getRight(), rightVx, bufferBetweenObjects);
          float startTimeOfCollisionY = calculateTimeToImpact(top->getBottom(), topVy,
            bottom->getTop(), bottomVy, bufferBetweenObjects);
          float endTimeOfCollisionY = calculateTimeToImpact(top->getTop(), topVy,
            bottom->getBottom(), bottomVy, bufferBetweenObjects);

          // WE NOW HAVE ALL THE INFO TO DETERMINE WHEN THESE TWO AABBs WILL COLLIDE
          // WE NEED TO FIGURE OUT THE FIRST TIME WHEN X AND Y COLLISIONS ARE HAPPENING

          // FIRST, IF EITHER START TIME IS > 1, THERE IS NO COLLISION
          if ((startTimeOfCollisionX > 1.0f) || (startTimeOfCollisionY > 1.0f))
            return 2.0f;

          // NEXT IF THE START TIME OF ONE AXIS HAPPENS AFTER
          // THE END TIME OF THE OTHER AXIS, THERE IS NO COLLISION
          if ((startTimeOfCollisionX > endTimeOfCollisionY)
            || (startTimeOfCollisionY > endTimeOfCollisionX))
            return 2.0f;

          // THERE MUST BE A COLLISION, THOUGH IT MAY HAPPEN IN NEGATIVE TIME, WHICH MEANS
          // THE OBJECTS ARE ALREADY COLLIDING

          // SINCE WE KNOW THEY ARE COLLIDING, WE JUST HAVE TO FIGURE OUT WHEN.
          // FIRST LET'S CONSIDER WHERE THEY ARE COLLIDING ON BOTH AXES
          // AT THE SAME TIME, WHICH MAY BE TIME 0. FOR THIS SITUATION, WE HAVE
          // TO DETERMINE THE "BEST" SURFACE ON WHICH TO RESOLVE
          if (fabs(startTimeOfCollisionX - startTimeOfCollisionY) < EPSILON) {
            // FIRST CALCULATE THE COLLISION TIME X & Y FOR BOTH OBJECTS
            float xLeft = left->getRight() + (leftVx * startTimeOfCollisionX);
            float xRight = right->getLeft() + (rightVx * startTimeOfCollisionX);
            float xDiff = xRight - xLeft;
            float yTop = top->getBottom() + (bottomVy * startTimeOfCollisionY);
            float yBottom = bottom->getTop() + (topVy * startTimeOfCollisionY);
            float yDiff = yBottom - yTop;
            if (xDiff > yDiff) {
              // IF IT'S A LEFT TO RIGHT COLLISION BUT THE CENTERS
              // ARE NOT MOVING TOWARDS EACH OTHER, IGNORE IT
              if (fabs(rightVx - leftVx) < EPSILON)
                return 2.0f;
              determineXAxisCollisionEdge(aabb1, aabb2, co1Edge, co2Edge);
              return startTimeOfCollisionX;
            }
            else {
              determineYAxisCollisionEdge(aabb1, aabb2, co1Edge, co2Edge);
              return startTimeOfCollisionY;
            }
          }
          // THE COLLISION TIME IS SIMPLY
          // THE LARGER OF THE TWO START TIMES
          else if (startTimeOfCollisionX > startTimeOfCollisionY) {
            // IF IT'S A LEFT TO RIGHT COLLISION BUT THE CENTERS
            // ARE NOT MOVING TOWARDS EACH OTHER, IGNORE IT
            if (fabs(rightVx - leftVx) < EPSILON)
              return 2.0f;

            determineXAxisCollisionEdge(aabb1, aabb2, co1Edge, co2Edge);
            return startTimeOfCollisionX;
          }
          else {
            // COLLISION HAPPENS ON THE Y AXIS, IS IT TOP OR BOTTOM?
            determineYAxisCollisionEdge(aabb1, aabb2, co1Edge, co2Edge);

            // THERE IS ONE MORE TRICKY THING TO CHECK. WE NEED TO MAKE
            // SURE THE SPRITE IS NOT NOW "ON" A SPRITE THAT IS A SIDE
            // WALL. SO, WE'LL NEED TO CHECK AND SEE IF THERE IS A TILE ABOVE
            // IT THAT WE ARE ALSO COLLIDING WITH ON THE SAME SIDE. IF THERE
            // IS, THEN WE'LL SAY IT'S AN X-AXIS COLLISION
            if (co1Edge == BOTTOM_EDGE) {
              // IS THERE A COLLIDABLE CELL ABOVE IT?

            }

            // IT'S DEFINITELY A Y-AXIS COLLISION
            return startTimeOfCollisionY;
          }
        }

        void performCollisionResponse(Collision* collision) {
          CollidableObject* co1 = collision->getCO1();
          CollidableObject* co2 = collision->getCO2();
          PhysicalProperties& pp1 = co1->getPhysicalProperties();
          PhysicalProperties& pp2 = co2->getPhysicalProperties();

          // IT'S FINAL, WE KNOW WHICH SIDE
          co1->setCollisionEdge(collision->getCO1Edge());
          co2->setCollisionEdge(collision->getCO2Edge());

          // IF COLLISION IS WITH A TILE WE HAVE A SPECIAL CASE BECAUSE
          // ONLY ONE OF THE COLLIDABLE OBJECTS NEEDS A RESPONSE
          if (collision->isCollisionWithTile()) {
            // IF THE COLLISION IS WITH A TILE BELOW THE SPRITE
            // SET ON TILE THIS FRAME TO TRUE
            if (collision->getCO1Edge() == BOTTOM_EDGE) {
              co1->setOnTileThisFrame(true);
              pp1.setVelocity(pp1.getVelocityX(), 0.0f);
              // AND CORRECT THE SPRITE
              float tileTop = co2->getBoundingVolume().getTop();
              co1->getBoundingVolume().setCenterY(tileTop -
                (co1->getBoundingVolume().getHeight() / 2) - NUDGE_VELOCITY);
            }
            else if (collision->getCO1Edge() == LEFT_EDGE) {
              pp1.setVelocity(NUDGE_VELOCITY, pp1.getVelocityY());
            }
            else if (collision->getCO1Edge() == RIGHT_EDGE) {
              pp1.setVelocity(-NUDGE_VELOCITY, pp1.getVelocityY());
            }
            else {
              pp1.setVelocity(pp1.getVelocityX(), NUDGE_VELOCITY);
            }
          }

          // YOU'LL NEED TO HANDLE SPRITE-TO-SPRITE COLLISIONS


          // MAKE SURE SPRITES ON TILES REMAIN ON TILES
          if (co1->isOnTileThisFrame())
            pp1.setVelocity(pp1.getVelocityX(), 0.0f);
          if (co2->isOnTileThisFrame())
            pp2.setVelocity(pp2.getVelocityY(), 0.0f);
        }

        void Physics::removeActiveCOCollisions(CollidableObject* co) {
          auto coIt = activeCollisions.begin();
          while (coIt != activeCollisions.end()) {
            // WE'LL NEED THIS TO REMOVE THE ELEMENT
            auto tempIt = coIt;

            // BUT WE'LL MOVE THE ITERATOR ALONG NOW FOR THE NEXT LOOP
            coIt++;

            // GET THE COLLISION WE MAY NEED TO REMOVE
            if (tempIt != activeCollisions.end()) {
              Collision* c = (*tempIt);

              // AND TEST IT
              if ((co == c->getCO1()) || (co == c->getCO2())) {
                // RECYCLE THE COLLIDABLE OBJECTS IF NECESSARY
                if (c->isCollisionWithTile()) {
                  recycledCollidableObjectsList.push_back(c->getCO2());
                }

                // PUT IT BACK IN THE RECYCLING BIN
                recycledCollisions.push_back(c);

                // AND REMOVE IT FROM THE LIST
                activeCollisions.erase(tempIt);
              }
            }
          }
        }

        void Physics::reorderSweptShape(CollidableObject* co, CollisionEdge ordering,
          bool increasingDirection) {
          // HERE WE'LL MAKE SURE THIS COLLIDABLE
          // OBJECT IS IN IT'S PROPER PLACE IN THE SWEPT SHAPE
          // DATA STRUCTURE SPECIFIED BY ORDERING IN EITHER THE
          // INCREASING OR DECREASING DIRECTION
          vector<CollidableObject*>& sweptShapes = sortedSweptShapes[ordering];

          // FIRST ON THE LEFT-EDGE SORTED VECTOR
          unsigned int coIndex = co->getSweepAndPruneIndex(ordering);
          unsigned int testIndex = coIndex - 1;
          unsigned int limitIndex = 0;
          if (increasingDirection) {
            testIndex = coIndex + 1;
            limitIndex = sweptShapes.size() - 1;
          }
          CollidableObject* temp;

          if ((0 <= testIndex) && (testIndex <= sweptShapes.size() - 1)) {
            bool limitReached = false;

            // MOVE IT UP OR DOWN THE DATA STRUCTURE AS FAR AS IT NEEDS TO GO
            while (!limitReached) {
              CollidableObject* testCO = sweptShapes.at(testIndex);
              float coSide = getSide(*co, ordering);
              float testSide = getSide(*testCO, ordering);
              if (increasingDirection) {
                if (coSide < testSide)
                  limitReached = true;
                else {
                  temp = testCO;
                  sweptShapes[testIndex] = co;
                  sweptShapes[coIndex] = temp;
                  co->setSweepAndPruneIndex(ordering, testIndex);
                  temp->setSweepAndPruneIndex(ordering, coIndex);
                  testIndex++;
                  coIndex++;
                  if (coIndex == (sweptShapes.size() - 1))
                    limitReached = true;
                }
              }
              else {
                if (coSide > testSide)
                  limitReached = true;
                else {
                  temp = testCO;
                  sweptShapes[testIndex] = co;
                  sweptShapes[coIndex] = temp;
                  co->setSweepAndPruneIndex(ordering, testIndex);
                  temp->setSweepAndPruneIndex(ordering, coIndex);
                  testIndex--;
                  coIndex--;
                  if (coIndex == 0)
                    limitReached = true;
                }
              }
            }
          }
        }

        float getSide(const CollidableObject& co, CollisionEdge ordering) {
          switch (ordering) {
          case LEFT_EDGE:
            return co.getSweptShape().getLeft();
          case RIGHT_EDGE:
            return co.getSweptShape().getRight();
          case TOP_EDGE:
            return co.getSweptShape().getTop();
          case BOTTOM_EDGE:
            return co.getSweptShape().getTop();
          default:
            return -1;
          }
        }

        void Physics::reorderCollidableObject(CollidableObject* co) {
          bool increasing = false;
          unsigned int counter = 0;
          while (counter < 4) {
            unsigned int ordering = counter / 2;
            reorderSweptShape(co, static_cast<CollisionEdge>(ordering), increasing);
            increasing = !increasing;
            counter++;
          }
        }

        /**
         * Helps us ensure we know which direction to push back when a collision happens.
         */
        void determineXAxisCollisionEdge(
          const AABB& aabb1,
          const AABB& aabb2,
          CollisionEdge& co1Edge,
          CollisionEdge& co2Edge) {
          // COLLISION HAPPENS ON THE X AXIS, IS IT LEFT OR RIGHT?
          if (aabb1.getCenterX() < aabb2.getCenterX()) {
            co1Edge = RIGHT_EDGE;
            co2Edge = LEFT_EDGE;
          }
          else {
            co1Edge = LEFT_EDGE;
            co2Edge = RIGHT_EDGE;
          }
        }

        /**
         * Helper method for implementing walking and jumping on tile surfaces.
         */
        void determineYAxisCollisionEdge(
          const AABB& aabb1,
          const AABB& aabb2,
          CollisionEdge& co1Edge,
          CollisionEdge& co2Edge) {
          // COLLISION HAPPENS ON THE YAXIS, IS IT TOP OR BOTTOM?
          if (aabb1.getCenterY() < aabb2.getCenterY()) {
            co1Edge = BOTTOM_EDGE;
            co2Edge = TOP_EDGE;
          }
          else {
            co1Edge = TOP_EDGE;
            co2Edge = BOTTOM_EDGE;
          }
        }

        bool CompareCollision(const Collision* a, const Collision* b) {
          return b->getTimeOfCollision() < a->getTimeOfCollision();
        }
      }
    }
  }
}
