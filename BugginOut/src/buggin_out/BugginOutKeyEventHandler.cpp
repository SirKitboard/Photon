/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutKeyEventHandler.cpp

    See BugginOutKeyEventHandler.h for a class description.
    */

#include "buggin_out_VS/stdafx.h"
#include "buggin_out/BugginOutKeyEventHandler.h"

#include "buggin_out/BugginOut.h"
#include "sssf/game/Game.h"
#include "sssf/game/WStringTable.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/gsm/physics/Physics.h"
#include "sssf/gsm/physics/PhysicalProperties.h"
#include "sssf/gsm/sprite/AnimatedSprite.h"
#include "sssf/gsm/state/GameState.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/Cursor.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/input/GameInput.h"
#include "sssf/timer/GameTimer.h"
#include "sssf/audio/GameAudio.h"

#ifdef CROSS
#include "sssf/platforms/Cross/StandardTimer.h"
#else
#include "sssf/platforms/Windows/WindowsTimer.h"
#endif
#include <sssf/data_loader/GameDataLoader.h>

namespace cse380 {
  namespace bugginout {
    using sssf::input::GameInput;
    using sssf::game::Game;
    using sssf::gsm::state::GameStateManager;
    using sssf::gsm::sprite::AnimatedSprite;
    using sssf::gsm::physics::PhysicalProperties;
    using sssf::gui::Cursor;
    using sssf::gui::Viewport;
    using sssf::time::GameTimer;

    /*
        handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
        gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
        key states since the last frame, which can allow us to respond to key presses, including when keys
        are held down for multiple frames.
        */
    void BugginOutKeyEventHandler::handleKeyEvents(Game* game) {
      // WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
      GameInput* input = game->getInput();

      // LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
      GameStateManager& gsm = game->getGSM();
      AnimatedSprite& player = gsm.getSpriteManager().getPlayer();
      PhysicalProperties& pp = player.getPhysicalProperties();
      Viewport& viewport = game->getGUI().getViewport();

      // IF THE GAME IS IN PROGRESS
      if (gsm.isGameInProgress()) {
	      if (game->getPaused()) {
		      player.setCurrentState(IDLE);
		      // WASD KEY PRESSES WILL CONTROL THE PLAYER
		      // SO WE'LL UPDATE THE PLAYER VELOCITY WHEN THESE KEYS ARE
		      // PRESSED, THAT WAY PHYSICS CAN CORRECT AS NEEDED
		      float vX = pp.getVelocityX();
		      float vY = pp.getVelocityY();

		      // YOU MIGHT WANT TO UNCOMMENT THIS FOR SOME TESTING,
		      // BUT IN THIS ASSIGNMENT, THE USER MOVES VIA MOUSE BUTTON PRESSES
		      if (input->isKeyDown(A_KEY)) {
			      vX = -PLAYER_SPEED;
			      //player.setCurrentState(ATTACKING_LEFT);
		      }
		      else if (input->isKeyDown(D_KEY)) {
			      vX = PLAYER_SPEED;
			      //player.setCurrentState(ATTACKING_RIGHT);
		      }
		      else {
			      vX = 0.0f;
			      player.setCurrentState(IDLE);
		      }
		      if (input->isKeyDown(W_KEY)) {
			      vY = -PLAYER_SPEED;
			      //player.setCurrentState(ATTACKING_RIGHT);
		      }
		      else if (input->isKeyDown(S_KEY)) {
			      vY = PLAYER_SPEED;
			      //player.setCurrentState(ATTACKING_RIGHT);
		      }
		      else {
			      vY = 0.0f;
			      player.setCurrentState(IDLE);
		      }
		      if (input->isKeyDownForFirstTime(G_KEY)) {
			      viewport.toggleDebugView();
			      game->getGraphics()->toggleDebugTextShouldBeRendered();
		      }

		      if (input->isKeyDownForFirstTime(P_KEY)) {
			      gsm.getPhysics().togglePhysics();
		      }
		      if (input->isKeyDownForFirstTime(T_KEY)) {
			      gsm.getPhysics().activateForSingleUpdate();
		      }
			  if (input->isKeyDownForFirstTime(SPACE_KEY))
			  {
				  gsm.getSpriteManager().getPlayer().lightsOn = !gsm.getSpriteManager().getPlayer().lightsOn;
			  }

		      // NOW SET THE ACTUAL PLAYER VELOCITY
		      pp.setVelocity(vX*1000000, vY*1000000);

		      bool viewportMoved = false;
		      float viewportVx = 0.0f;
		      float viewportVy = 0.0f;
		      if (input->isKeyDown(UP_KEY)) {
			      viewportVy -= MAX_VIEWPORT_AXIS_VELOCITY;
			      viewportMoved = true;
		      }
		      if (input->isKeyDown(DOWN_KEY)) {
			      viewportVy += MAX_VIEWPORT_AXIS_VELOCITY;
			      viewportMoved = true;
		      }
		      if (input->isKeyDown(LEFT_KEY)) {
			      viewportVx -= MAX_VIEWPORT_AXIS_VELOCITY;
			      viewportMoved = true;
		      }
		      if (input->isKeyDown(RIGHT_KEY)) {
			      viewportVx += MAX_VIEWPORT_AXIS_VELOCITY;
			      viewportMoved = true;
		      }

		      if (viewportMoved)
			      viewport.moveViewport((int)floor(viewportVx + 0.5f), (int)floor(viewportVy + 0.5f),
			                            game->getGSM().getWorld().getWorldWidth(),
			                            game->getGSM().getWorld().getWorldHeight());
	      }
		if (input->isKeyDownForFirstTime(ESC_KEY)) {
			game->getDataLoader()->initInGameGUI(game);
			game->togglePause();
		}

      }

      if ((input->isKeyDownForFirstTime(C_KEY)) && input->isKeyDown(SHIFT_KEY)) {
        Cursor* cursor = game->getGUI().getCursor();
        unsigned int id = cursor->getActiveCursorID();
        id++;
        if (id == cursor->getNumCursorIDs())
          id = 0;
        cursor->setActiveCursorID(id);
      }

	  if (input->isKeyDownForFirstTime(P_KEY)) {
		  sssf::gsm::ai::pathfinding::GridPathfinder *pathfinder = gsm.getSpriteManager().getPathFinder();
		  pathfinder->mapPath(&player, 1000.0, 1000.0);
	  }

	  if ((input->isKeyDown(RCTRL_KEY) || input->isKeyDown(LCTRL_KEY)) && input->isKeyDownForFirstTime(ONE_KEY)) {
		  game->getDataLoader()->loadLevel1(game);
	  }
	  else if ((input->isKeyDown(RCTRL_KEY) || input->isKeyDown(LCTRL_KEY)) && input->isKeyDownForFirstTime(TWO_KEY)) {
		  game->getDataLoader()->loadLevel2(game);
	  }
	  else if ((input->isKeyDown(RCTRL_KEY) || input->isKeyDown(LCTRL_KEY)) && input->isKeyDownForFirstTime(THREE_KEY)) {
		  game->getDataLoader()->loadLevel3(game);
	  }

	  else if ((input->isKeyDown(LCTRL_KEY) || input->isKeyDown(RCTRL_KEY)) && input->isKeyDownForFirstTime(A_KEY)) {
		  game->getAudio()->playSound(0);
	  }
	  else if ((input->isKeyDown(LCTRL_KEY) || input->isKeyDown(RCTRL_KEY)) && input->isKeyDownForFirstTime(W_KEY)) {
		  game->levelWin();
	  }
	  else if ((input->isKeyDown(LCTRL_KEY) || input->isKeyDown(RCTRL_KEY)) && input->isKeyDownForFirstTime(L_KEY)) {
		  game->levelLose();
	  }
	  

      // LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
      GameTimer* timer = game->getTimer();
      int fps = timer->getTargetFPS();

      // THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
      // DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
      // A BETTER PLAYER EXPERIENCE
      if (input->isKeyDown(HOME_KEY) && (fps < MAX_FPS))
        timer->setTargetFPS(fps + FPS_INC);

      // THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
      else if (input->isKeyDown(END_KEY) && (fps > MIN_FPS))
        timer->setTargetFPS(fps - FPS_INC);
    }
  }
}