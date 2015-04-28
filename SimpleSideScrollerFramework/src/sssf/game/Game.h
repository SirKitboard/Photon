/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Game.h
    */

#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include <string>

#include "sssf/gsm/state/GameState.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/text/GameText.h"
#include <LuaPlusFramework/LuaState.h>


static const float PI = 3.1415926535897932384626433f;

// FORWARD DECLARATIONS OF CLASSES USED BY GAME

namespace cse380 {
  namespace sssf {

    namespace data { class GameDataLoader; }
    namespace graphics { class GameGraphics; }
    namespace input { class GameInput; }
    namespace os { class GameOS; }
    namespace time { class GameTimer; }
    namespace gui { class GameGUI; }

    namespace game {
      using std::wstring;
      using gsm::state::GameStateManager;
      using graphics::GameGraphics;
      using input::GameInput;
      using gui::GameGUI;
      using data::GameDataLoader;
      using os::GameOS;
      using text::GameText;
      using time::GameTimer;

      /// <summary>
      /// <para>
      /// The central object of the game. Plays two important roles:
      /// </para>
      /// <para>
      /// First, Game manages the game data and communicates with external
      /// technologies like graphics cards and input devices. The objects that
      /// most directly manage such data can can be accessed via inlined public
      /// methods (getGraphics(), getInput(), getTimer(), etc.).
      /// </para>
      /// <para>
      /// Second, Game runs the game loop, calling the appropriate methods in
      /// the other major objects as needed. The runGameLoop method runs once
      /// per frame, constantly invoking all necessary methods to run the game.
      /// </para>
      /// <para>
      /// This class is deliberately technology-agnostic, for portability reasons.
      /// You may wish to use this engine with some technology besides DirectX
      /// (such as OpenGL, or maybe even that new Vulkan thing).
      /// </para>
      /// </summary>
      class Game {
      public:
        // METHODS DEFINED IN Game.cpp
        /**
         * Begins the construction and loading of all major game objects. This
         * method is only called once, when the application is first started.
         *
         * @note This constructor will not initialize the custom game objects:
         * graphics, input, os, & timer. These must be constructed separately
         * and fed to this object using the init method.
         */
        Game();

        /**
         * If Game is deleted, the application is closing, so everything should
         * be cleaned up.
         */
        virtual ~Game();

        // No copying this class!
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

        // INLINED ACCESSOR METHODS - THE REAL THINGS
        GameStateManager& getGSM() { return gsm; }
        const GameStateManager& getGSM() const { return gsm; }

        GameGUI& getGUI() { return gui; }
        const GameGUI& getGUI() const { return gui; }

        GameText& getText() { return text; }
        const GameText& getText() const { return text; }

        // INLINED ACCESSOR METHODS - ACTUALLY SUB-CLASSES
        GameDataLoader* getDataLoader() { return dataLoader; }
        const GameDataLoader* getDataLoader() const { return dataLoader; }

        GameGraphics* getGraphics() { return graphics; }
        const GameGraphics* getGraphics() const { return graphics; }

        GameInput* getInput() { return input; }
        const GameInput* getInput() const { return input; }

        GameOS* getOS() { return os; }
        const GameOS* getOS() const { return os; }

        GameTimer* getTimer() { return timer; }
        const GameTimer* getTimer() const { return timer; }

		LuaPlus::LuaState* getLuaState() { return luaState; }

		
		
		//LuaPlus::LuaState* getLuaState() { return luaPState; }

		b2World* getGameWorld() { return gameWorld; }

        // AND FOR GETTING THE NAME OF THE FILE USED FOR LOADING THIS LEVEL
        const wstring& getCurrentLevelFileName() const {
          return currentLevelFileName;
        }

        // INLINED MUTATOR METHODS
        void setDataLoader(GameDataLoader* initDL) {
          dataLoader = initDL;
        }

        void setCurrentLevelFileName(const wstring& initCurrentLevelFileName) {
          currentLevelFileName = initCurrentLevelFileName;
        }

        void setCurrentLevelDirectory(const wstring& dir) {
          currentLevelDir = dir;
        }

        void setCurrentLevel(const wstring& dir, const wstring& name) {
          currentLevelDir = dir;
          currentLevelFileName = name;
        }

        /// <summary>
        /// Feeds this Game the technology-specific objects that manage game or
        /// system-specific resources (textures, timing, input, etc.).
        /// </summary>
        void initPlatformPlugins(GameGraphics*, GameInput*, GameOS*, GameTimer*);

        /**
         * Directs game logic to be executed or not, depending on the game state,
         * it additionally triggers the building or render lists and game loop
         * timing each frame.
         */
        void processGameData();
        void quitApp();

        /**
         * Forces the unloading of all game data loaded for the current level
         * and then return the user to the main menu.
         */
        void quitGame();

        /**
         * Windows applications must cooperate with other running applications,
         * so when someone hits ALT-TAB and switches from a full-screen game, it
         * might lose ownership of the graphics card. This method can be called
         * when a full-screen application retains ownership of all necessary
         * resources such that all necessary data (like textures, sound, music,
         * etc.) can be reloaded.
         */
        void reloadAllDevices();

        /**
         * This is the game loop management method. It runs continuously while
         * the game is active. Once per frame it instructs the major game
         * objects to get user input, record user input, update the GUI state,
         * update the sprites' states using AI and input, perform collision
         * detection and resolution (physics), render the screen, etc.
         *
         * This loop is timed such that everything is kept to a consistent
         * framerate, thus the game should run consistently on all machines.
         */
        void runGameLoop();

        /// <summary>
        /// Releases all allocated resources and terminates the game.
        /// </summary>
        void shutdown();

        // Loads the first level of the game and start play.
        void startGame();

      private:
        // THE GameStateManager DEALS WITH ALL THE GAME-PLAY AND GAME-WORLD
        // STUFF, LIKE AI, PHYSICS, AND GAME WORLD CONSTRUCTION. IT ALSO KNOWS
        // THE CURRENT GAME STATE
        GameStateManager gsm;

        // MANAGES ALL THE GUIS
        GameGUI gui;

        // MANAGES TEXT FOR RENDERING AND FILE I/O
        GameText text;

        // THESE GAME OBJECTS ALL REQUIRE CUSTOM DEFINITIONS, EITHER BECAUSE
        // THEY DEAL WITH PLATFORM-SPECIFIC THINGS, OR GAME-SPECIFIC THINGS
        GameDataLoader* dataLoader;
        GameGraphics* graphics;
        GameInput* input;
        GameOS* os;
        GameTimer* timer;
		b2World *gameWorld;
		LuaPlus::LuaState* luaState;
		
        // THIS KEEPS TRACK OF THE NAME OF THE LEVEL FILE THAT'S BEING USED
        wstring currentLevelFileName;
        wstring currentLevelDir;
      };
    }
  }
}
