/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    SpriteManager.h
    */

#pragma once

#include <unordered_map>
#include <vector>


#include "sssf/gsm/sprite/AnimatedSprite.h"
#include "sssf/gsm/ai/BotRecycler.h"
#include <sssf/gsm/ai/pathfinding/GridPathfinder.h>

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }
    namespace graphics { class RenderList; }
    namespace gui { class Viewport; }

    namespace gsm {

      namespace ai { class Bot; }

      namespace sprite {
        using std::vector;
        using std::unordered_map;

        class AnimatedSpriteType;
        using ai::Bot;
        using ai::BotRecycler;
        using gui::Viewport;
        using graphics::RenderList;

        /// <summary>
        /// Manages all of the sprites in a given game level, including the
        /// player's.
        /// </summary>
        class SpriteManager {
        public:
          // NOTHING TO INIT OR DESTROY
          ~SpriteManager();
          SpriteManager() {}

          // No copying allowed!
          SpriteManager(const SpriteManager&) = delete;
          SpriteManager& operator=(const SpriteManager&) = delete;

          // INLINED ACCESSOR METHODS
          size_t getNumberOfSprites() const { return bots.size(); }

          AnimatedSprite& getPlayer() { return player; }
          const AnimatedSprite& getPlayer() const { return player; }

          vector<Bot*>::iterator begin() { return bots.begin(); }
          vector<Bot*>::const_iterator cbegin() const { return bots.cbegin(); }
          vector<Bot*>::iterator end() { return bots.end(); }
          vector<Bot*>::const_iterator cend() const { return bots.cend(); }

		  void checkWin(Game*);

		  ai::pathfinding::GridPathfinder* getPathFinder() { return pathfinder; }
		  void				setPathfinder(ai::pathfinding::GridPathfinder *initPathfinder) { pathfinder = initPathfinder; }
          // ^ These allow SpriteManager to be used in foreach loops

          /// <summary>
          /// Adds a new sprite to this sprite manager. Once a sprite is added
          /// it can be scheduled for rendering.
          /// </summary>
          void addBot(Bot* botToAdd);

          /// <summary>
          /// Adds all visible sprites to the render list. This method
          /// should be called each frame.
          /// </summary>
          void addSpriteItemsToRenderList(game::Game* game) const;

          /// <summary>
          /// <para>
          /// Adds a new sprite type. Many different sprites can share one sprite
          /// type. For example, there may be a "Bunny" sprite with its own
          /// properties (animation speed, images to load, etc.). Then there
          /// might be 100 different Bunnies each with its own state, but with
          /// everything described by the AnimatedSpriteType in common.
          /// </para>
          /// <para>
          /// The AnimatedSpriteType will become managed by this sprite manager;
          /// don't delete it explicitly, or else your game may crash.
          /// </para>
          /// </summary>
          void addSpriteType(AnimatedSpriteType* spriteTypeToAdd);

          /// <summary>
          /// Adds the given sprite to the given renderlist if said sprite is
          /// within the given viewport.
          /// </suummary>
          void addSpriteToRenderList(const AnimatedSprite&, RenderList&, const Viewport&) const;

          /// Removes all sprites from the game world.
          void clearSprites();

          /// Returns the sprite type that corresponds to the given index.
          AnimatedSpriteType* getSpriteType(const wstring& type) {
            return (spriteTypes.count(type)) ? spriteTypes[type] : nullptr;
          }

          const AnimatedSpriteType* getSpriteType(const wstring& type) const {
            return (spriteTypes.count(type)) ? spriteTypes.at(type) : nullptr;
          }

          Bot* removeBot(Bot* botToRemove);

		  void clearBots() {
			  bots.clear();
		  }

          /// <summary>
          /// Removes all animated sprite types from memory. Any other resources
          /// owned by the sprite manager should be deleted here, too. This will
          /// also remove all sprites in the game world.
          /// </summary>
          void unloadSprites();

          /// <summary>
          /// Calls the update method for each sprite. Should be called once
          /// per frame.
          /// </summary>
		  void incr(int incr) {
			  int x = player.getPhysicalProperties().getX();
			  int y = player.getPhysicalProperties().getY();
			  player.getPhysicalProperties().setPosition(x + incr, y);
		  }

          void update(game::Game* game);

		  vector<Bot*> getBots() { return bots; }

        private:
          // NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
          // SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
          unordered_map<wstring, AnimatedSpriteType*> spriteTypes;

          // THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
          // THAT MOVES AROUND AND IS NOT THE PLAYER
          vector<Bot*> bots;

          // AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
          AnimatedSprite player;

          // THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
          // WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US
          BotRecycler recyclableBots;

	        ai::pathfinding::GridPathfinder *pathfinder;
        };
      }
    }
  }
}
