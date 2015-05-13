/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameStateManager.h
    */
#pragma once

#include <string>
#include <vector>

#include "sssf/gsm/state/GameState.h"
#include "sssf/gsm/physics/Physics.h"
#include "sssf/gsm/world/World.h"
#include "sssf/gsm/sprite/SpriteManager.h"

static const int NO_LEVEL_LOADED = 0;

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace gsm {

      namespace world { class World; }

      namespace state {
        using std::wstring;
        using std::vector;

        using physics::Physics;
        using sprite::SpriteManager;
        using world::World;
        using game::Game;

        /// <summary>
        /// <para>
        /// Manages the static data for the game level being used. This means
        /// all game backgrounds, which are stored and manipulated in WorldLayer
        /// objects. A given level can have a game background rendered using
        /// multiple layers, some which can be tiled, others that can be sparse,
        /// and others that can be isometric.
        /// </para>
        /// <para>
        /// This class stores these layers and ensures they are rendered in the
        /// proper order, low index to high. Layers that need to be drawn first
        /// (the back-most layer), should be added first.
        /// </para>
        /// </summary>
        class GameStateManager {
        public:
          /**
           * Default Constructor, it starts the app at the splash screen with no
           * level loaded.
           */
          GameStateManager();

          /**
           * This destructor should inform the game world and sprite managers to
           * clean up all their allocated memory.
           */
          ~GameStateManager() {}

          // No copying allowed!
          GameStateManager(const GameStateManager&) = delete;
          GameStateManager& operator=(const GameStateManager&) = delete;

          // INLINED ACCESSOR METHODS
          GameState getCurrentGameState() const { return currentGameState; }

          unsigned int getCurrentLevel() const { return currentLevel; }
          unsigned int getNumLevels() const { return levelNames.size(); }

          Physics& getPhysics() { return physics; }
          const Physics& getPhysics() const { return physics; }

          SpriteManager& getSpriteManager() { return spriteManager; }
          const SpriteManager& getSpriteManager() const { return spriteManager; }

          World& getWorld() { return world; }
          const World& getWorld() const { return world; }

		  vector<b2Body*>& getWalls() { return walls; }
		  const vector<b2Body*>& getWalls() const { return walls; }

		  void clearBodies()
          {
			  walls.clear();
			  sentries.clear();
          }

		  

		  vector<b2Body*>& getSentries() { return sentries; }
		  const vector<b2Body*>& getSentries() const { return sentries; }

          const wstring& getCurrentLevelName() const { return levelNames[currentLevel]; }

          // METHODS FOR TESTING THE CURRENT GAME STATE


          /**
           * Tests if this application is going to continue running for another
           * frame. This will return true if the game app is not closing, false
           * otherwise. Note that when the currentGameState becomes GS_EXIT_GAME,
           * this will return true, and the game loop will end.
           */
          bool isAppActive() const {
            return currentGameState != GameState::GS_EXIT_GAME;
          }

          /**
           * Tests if this application is currently at the splash screen. This
           * will dictate what to render, but also how to respond to user input.
           */
          bool isAtSplashScreen() const {
            return currentGameState == GameState::GS_SPLASH_SCREEN;
          }

          /**
           * Tests if the game is running right now, which means we have to
           * execute all game logic.
           */
          bool isGameInProgress() const {
            return currentGameState == GameState::GS_GAME_IN_PROGRESS;
          }

          /**
           * Tests if the game is loading a level right now.
           */
          bool isGameLevelLoading() const {
            return currentGameState == GameState::GS_STARTING_LEVEL;
          }

          /**
           * Tests if the game world should be rendered or not. Note that even if
           * the game is paused, you'll likely still render the game.
           */
          bool isWorldRenderable() const {
            return ((currentGameState == GameState::GS_GAME_IN_PROGRESS)
              || (currentGameState == GameState::GS_PAUSED)
              || (currentGameState == GameState::GS_GAME_OVER));
          }

          // METHODS FOR TRANSITIONING TO OTHER GAME STATES

          /**
           * Transitions the game from the level loading to the actual game.
           */
          void goToGame() {
            currentGameState = GameState::GS_GAME_IN_PROGRESS;
          }

          /**
           * Transitions the game from the main menu to the level loading.
           */
          void goToLoadLevel() {
            currentGameState = GameState::GS_STARTING_LEVEL;
          }

          /**
           * Transitions the game from the splash screen to the main menu.
           */
          void goToMainMenu() {
            this->unloadCurrentLevel();
            currentGameState = GameState::GS_MAIN_MENU;
          }

		  void goToHelp() {
			  this->unloadCurrentLevel();
			  currentGameState = GameState::GS_MENU_HELP_MENU;
          }

		  void goToAbout() {
			  this->unloadCurrentLevel();
			  currentGameState = GameState::GS_MENU_ABOUT_MENU;
          }
          // METHODS DEFINED in GameStateManager.cpp

          /**
           * Adds all world and sprite items in the viewport to the render list
           */
          void addGameRenderItemsToRenderList(Game* game) const;

          /**
           * Adds a level file name to the vector of all the level file names.
           * Storing these file names lets us easily load a desired level at any time.
           */
          void addLevel(const wstring&, const wstring&);

          /**
           * Note that when our game app starts, we load all the level file names
           * and this should never change. This method looks through those names
           * and gets the index in the vector for a given level name. Note that
           * we're using a little iterator/pointer arithmetic here since vectors
           * guarantee they will store their contents in a continuous block of memory.
           */
          unsigned int getLevelNum(const wstring& levelName) const;

          /**
           * Called when the user wants to quit the application. Updates the game
           * state such that all world resources are released and the game loop
           * does not iterate again.
           */
          void shutdown();

          /**
           * Changes the current level. This method should be called before
           * loading all the data from a level file.
           */
          void loadLevel(Game* game, unsigned int levelNum);

          /**
           * An overloaded version of our loadLevel method that uses the name of
           * the level rather than its index. Some games may have non-linear
           * levels, and so the developers may prefer to hold onto the level names
           * rather than numbers.
           */
          void loadLevel(Game* game, const wstring& levelName);

          /**
           * Removes all data from the World, recovering all used memory. It
           * should be called first when a level is unloaded or changed. If it's
           * not called, an application runs the risk of having memory leaking,
           * i.e. extra data sitting around that may slow the progam down. Or, if
           * the app thinks a level is still active, it might add items to the
           * render list using image ids that have already been cleared from the
           * GameGraphics' texture manager for the world. That would likely result
           * in an exception.
           */
          void unloadCurrentLevel();

          /**
           * Should be called once per frame. Updates both the sprites and the
           * game world. Note that even though the game world is for static data,
           * should the user wish to put anything dynamic there (like a non-collidable
           * moving layer), the updateWorld method is called.
           */
          void update(Game* game);
        private:
          // THE CURRENT GAME STATE
          GameState currentGameState;

          // THE CURRENT LEVEL IS THE ONE CURRENTLY BEING PLAYED. NOTE THAT IF
          // THE CURRENT LEVEL IS 0, THEN IT MEANS THERE IS NO ACTIVE LEVEL LOADED,
          // LIKE WHEN WE'RE AT THE MAIN MENU
          unsigned int currentLevel;

          // THESE VECTORS STORE THE NAMES OF EACH LEVEL AND THE RELATIVE
          // PATH AND FILE NAME OF EACH DATA INPUT FILE FOR ALL GAME LEVELS.
          // NOTE THAT WE'LL LOAD THESE VECTORS WITH THIS DATA WHEN THE GAME STARTS UP,
          // BUT WE'LL ONLY LOAD THE LEVELS INTO THE GAME WORLD AS EACH LEVEL IS PLAYED
          // NOTE THAT THE LEVEL NAME AT INDEX i IN THE FIRST VECTOR CORRESPONDS
          // TO THE PATH AND FILE NAME AT INDEX i IN THE SECOND ONE
          vector<wstring> levelNames;
          vector<wstring> levelFileNamesWithRelativePath;

          // FOR MANAGING STATIC GAME WORLD DATA, i.e. BACKGROUND IMAGES,
          // TILES, PLATFORMS, etc. BASICALLY THINGS THAT ARE NOT ANIMATED
          // AND DO NOT MOVE
          World world;

		  //LIST ALL THE BODIES IN THE WORLD
		  vector<b2Body*> walls;
		  vector<b2Body*> sentries;

          // FOR MANAGING DYNAMIC GAME OBJECTS FOR CURRENT LEVEL
          // NOTE THAT WE CALL THE DYNAMIC OBJECTS "SPRITES"
          SpriteManager spriteManager;

          // FOR DOING ALL COLLISION DETECTION AND RESOLUTION
          Physics physics;
        };
      }
    }
  }
}
