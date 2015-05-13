/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutButtonEventHandler.cpp

    See BugginOutButtonEventHandler.h for a class description.
    */

#include "buggin_out_VS/stdafx.h"
#include "buggin_out/BugginOutButtonEventHandler.h"

#include "buggin_out/BugginOut.h"
#include "sssf/game/Game.h"
#include "sssf/gsm/state/GameStateManager.h"

namespace cse380 {
  namespace bugginout {
    using sssf::game::Game;
    void BugginOutButtonEventHandler::handleButtonEvents(
      Game* game,
      const wstring& command
      ) {
      // THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
      // SO LET'S SHUTDOWN THE ENTIRE APPLICATION
      if (command == W_EXIT_COMMAND) {
        game->shutdown();
      }
      // THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
      // SCREEN, SO LET'S GO TO THE MAIN MENU
      else if (command == W_GO_TO_MM_COMMAND) {
        sssf::gsm::state::GameStateManager& gsm = game->getGSM();
        gsm.goToMainMenu();
      }
      // THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
      // SO LET'S START THE GAME FROM THE FIRST LEVEL
      else if (command == W_START_COMMAND) {
        game->setCurrentLevel(W_LEVEL_1_DIR, W_LEVEL_1_NAME);
        game->startGame();
      }

	  else if (command == W_HELP_COMMAND) {
		  sssf::gsm::state::GameStateManager& gsm = game->getGSM();
		  gsm.goToHelp();
	  }

	  else if (command == W_ABOUT_COMMAND) {
		  sssf::gsm::state::GameStateManager& gsm = game->getGSM();
		  gsm.goToAbout();
	  }
      // THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
      // SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
      else if (command == W_QUIT_COMMAND) {
        game->quitGame();
      }

	  else if (command == W_RESUME_COMMAND) {
		  game->togglePause();
	  }

	  else if (command == W_NEXT_LEVEL_COMMAND) {
		  game->nextLevel();
	  }
	  else if (command == W_RELOAD_LEVEL_COMMAND) {
		  game->reloadLevel();
	  }
    }
  }
}