/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Game.cpp
    */
#include "sssf_VS/stdafx.h"
#include "sssf/game/Game.h"

#include "sssf/data_loader/GameDataLoader.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/input/GameInput.h"
#include "sssf/os/GameOS.h"
#include "sssf/text/GameText.h"
#include "sssf/text/TextGenerator.h"
#include "sssf/timer/GameTimer.h"
#include <LuaPlusFramework/LuaFunction.h>
#include <LuaPlusFramework/LuaObject.h>


namespace cse380 {
  namespace sssf {
    namespace game {

      using input::GameInput;
      using graphics::GameGraphics;
      using text::TextGenerator;

      Game::Game() {
        // NOTE THAT graphics, input, os, & timer
        // ARE CUSTOM GAME OBJECTS. DEPENDING ON WHAT TECHNOLOGY
        // IS TO BE USED THESE OBJECT SHOULD BE CONSTRUCTED
        // AND THEN FED TO THIS Game USING THE init METHOD
		  b2Vec2 gravity(0.0f, 0.0f);
		  gameWorld = new b2World(gravity);
		  luaState = LuaPlus::LuaState::Create();
		  int result = luaState->DoFile("LuaTest.lua");
      }

      Game::~Game() {
		  paused = false;
        // THE GAME AND PLATFORM STUFF SHOULD BE DELETED
        // BY WHOEVER CREATED THEM
      }

      void Game::initPlatformPlugins(GameGraphics* initGraphics,
        GameInput* initInput,
        GameOS* initOS,
        GameTimer* initTimer) {
        // INITIALIZE ALL OF THE GAME'S CUSTOM OBJECTS
        graphics = initGraphics;
        input = initInput;
        os = initOS;
        timer = initTimer;
      }

      void Game::reloadAllDevices() {
        graphics->reloadGraphics();

        // WE MIGHT ADD MORE LATER
      }

      void Game::runGameLoop() {
        // FIRST PROFILE?
        bool firstTimeThroughLoop = true;

        // LET'S START THE TIMER FROM SCRATCH
        timer->resetTimer();

        // KEEP RENDERING UNTIL SOMEONE PULLS THE PLUG
        while (gsm.isAppActive()) {
          // MOVE ALONG WINDOWS MESSAGES, THIS ALLOWS
          // US TO GET USER INPUT
          os->processOSMessages();

          // GET USER INPUT AND UPDATE GAME, GUI, OR PLAYER
          // STATE OR WHATEVER IS NECESSARY
          input->processInput(this);

          // IT IS POSSIBLE THE USER REQUESTED A SHUTDOWN
          // OF THE APP, SO WE NEED TO CHECK AGAIN
          if (gsm.isAppActive()) {
            // USE THE INPUT TO UPDATE THE GAME
            processGameData();
			
            // AND RENDER THE GAME
            graphics->renderGame(this);
          }
        }
      }

      void Game::processGameData() {
        // WE ONLY PERFORM GAME LOGIC IF THE GAME IS IN PROGRESS
        if (gsm.isGameInProgress()) {
//			LuaPlus::LuaFunction<void> luaRandom = luaState->GetGlobal("generateDirection");
//			luaRandom();
//	        LuaPlus::LuaObject inrement_object = luaState->GetGlobal("increment");
//			int incre = inrement_object.GetInteger();
//			this->getGSM().getSpriteManager().incr(incre);
          gsm.update(this);
        }
        else if (gsm.isGameLevelLoading()) {
          gsm.goToGame();
        }

        // UPDATE TEXT
        TextGenerator* tg = text.getTextGenerator();
        tg->updateText(this);

        // BUILD THE RENDER LISTS
        graphics->fillRenderLists(this);

        // KEEP THE FRAME RATE CONSISTENT
        timer->timeGameLoop();
      }

      void Game::quitGame() {
        // CLEAN UP ALL THE WORLD TEXTURES
        graphics->clearWorldTextures();

        // WE'RE GOING BACK TO THE MAIN MENU
        gsm.goToMainMenu();
      }

      void Game::shutdown() {
        // MAKE SURE THE GAME LOOP ENDS AND THAT THE GSM CLEANS
        // UP ALL THE MEMORY IT IS USING
        gsm.shutdown();

        // CLEAR OUT AND RELEASE THE GPU
        graphics->shutdown();

        // RELEASE GAME CONTROLLERS IF NECESSARY
        input->shutdown();

        // WE MAY SHUTDOWN OTHER THINGS HERE LIKE SOUND & MUSIC
        // RESOURCES IN THE FUTURE
        // AND KILL THE WINDOW
        os->shutdown();
      }

      void Game::startGame() {
        // AND UPDATE THE GAME STATE OF COURSE
		  togglePause();
        gsm.goToLoadLevel();   // NOTE THAT CURRENTLY THERE IS NO LEVEL FILE,
        // THAT'S ONE THING YOU'LL BE DOING
        dataLoader->loadWorld(this, currentLevelDir + currentLevelFileName);
      }
    }
  }
}
