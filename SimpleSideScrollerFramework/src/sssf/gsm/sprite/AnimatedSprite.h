/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    AnimatedSprite.h
    */

#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include "sssf/gsm/physics/CollidableObject.h"
#include <sssf/gsm/ai/pathfinding/PathNode.h>

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace gsm {
      namespace sprite {
        using std::function;
        using std::wstring;
        using game::Game;
		using std::list;

        class AnimatedSpriteType;

        /// <summary>
        /// Represents a sprite that can be used to animate a game character
        /// or object.
        /// </summary>
        class AnimatedSprite : public physics::CollidableObject {
        public:
			bool lightsOn;
          /// <summary>
          /// Typedef for a function that's called whenever a sprite finishes
          /// its current animation and loops back. The parameters represent the
          /// sprite and the name of its the current state. You can create these
          /// with lambda function syntax (or possibly with just function pointers,
          /// but I'm not entirely sure). If you need anything else, just capture
          /// it within the lambda (this is called a closure; JavaScript uses
          /// them ALL the time).
          /// </summary>
          typedef function<void(AnimatedSprite&, const wstring&)> AnimationEndListener;
          AnimatedSprite();

          /// <summary>
          /// Nothing to clean up. We don't want to clean up the sprite type
          /// because other sprites may still be using it.
          /// </summary>
          virtual ~AnimatedSprite() {}

          // INLINED ACCESSOR METHODS
          uint8_t getAlpha() const { return alpha; }
          const wstring& getCurrentState() const { return currentState; }
          size_t getFrameIndex() const { return frameIndex; }
          AnimatedSpriteType* getSpriteType() { return spriteType; }
          const AnimatedSpriteType* getSpriteType() const { return spriteType; }

          // INLINED MUTATOR METHODS
          void setAlpha(const uint8_t a) { alpha = a; }
          void setSpriteType(AnimatedSpriteType* type) { spriteType = type; }

          /// <summary>
          /// Changes the animation frame for this sprite if it's been displayed
          /// for long enough.
          /// </summary>
          void changeFrame();

          /// <summary>
          /// Retrieves the image id that corresponds to this sprite's current
          /// state and frame index.
          /// </summary>
          size_t getCurrentImageID() const;

          /// <summary>
          /// Sets this sprite to the newState state and starts its animation
          /// sequence from the beginning.
          /// </summary>
          void setCurrentState(const wstring& newState);

          /// <summary>
          /// You can add a function that's called whenever a sprite finishes
          /// an animation. Make sure the function runs quickly, though, as it
          /// could be called very frequently. Set this to nullptr to disable it.
          /// </summary>
          void setAnimationEndListener(const AnimationEndListener& listener) {
            this->_onStateFinished = listener;
          }

		  void advanceCurrentPathNode()
		  {
			  currentPathNode++;
		  }
		  void resetCurrentPathNode()
		  {
			  currentPathNode = currentPathToFollow.begin();
		  }
		  void clearPath()
		  {
			  currentPathToFollow.clear();
			  currentPathNode = currentPathToFollow.end();
		  }

          /// <summary>
          /// Advances the animation counter appropriately per the animation speed.
          /// Also updates the sprite location per the current velocity. Should
          /// be called once per frame for each sprite.
          /// </summary>
          void updateSprite();

		  //TODO: Create Replacement
          // This method will be very different, or perhaps even irrelevant,
          // when you start using Box2D.
          void affixTightAABBBoundingVolume();
          void correctToTightBoundingVolume();
		  list<PathNode>*		getCurrentPathToFollow() { return &currentPathToFollow; }
		  list<PathNode>::iterator getCurrentPathNode() { return currentPathNode; }
		  bool hasReachedDestination()
		  {
			  return currentPathNode == currentPathToFollow.end();
		  }

        protected:
          // SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
          // OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
          AnimatedSpriteType* spriteType;

          // TRANSPARENCY/OPACITY
          uint8_t alpha;

          // THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE
          // IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
          // WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
          wstring currentState;

          // THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
          size_t frameIndex;

          // USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
          unsigned int animationCounter;

          AnimationEndListener _onStateFinished;
	        std::list<PathNode> currentPathToFollow;
		  std::list<PathNode>::iterator currentPathNode;
        };
      }
    }
  }
}
