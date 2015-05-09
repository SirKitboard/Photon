/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameGUI.cpp

    See GameGUI.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gui/GameGUI.h"

#include "sssf/game/Game.h"
#include "sssf/gsm/state/GameState.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/Cursor.h"
#include "sssf/gui/ScreenGUI.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/graphics/RenderList.h"
#include "sssf/input/ButtonEventHandler.h"

namespace cse380 {
  namespace sssf {
    namespace gui {
      using game::Game;
      using gsm::state::GameStateManager;
      using namespace graphics;

      GameGUI::GameGUI() : customCursor(nullptr) {}

      GameGUI::~GameGUI() {
        delete customCursor;

        for (auto& screen : this->screens) {
          delete screen.second;
        }
      }

      void GameGUI::addRenderItemsToRenderList(Game* game) const {
        GameStateManager& gsm = game->getGSM();
        GameState gameState = gsm.getCurrentGameState();

        GameGraphics* graphics = game->getGraphics();
        RenderList& guiRenderList = graphics->getGUIRenderList();

        if (gsm.isAppActive()) {
          // CASCADE THIS CALL TO THE APPROPRIATE SCREEN
          screens.at(static_cast<uint8_t>(gameState))->addRenderItemsToRenderList(guiRenderList);
        }

        // CASCADE THIS CALL TO THE CURSOR, IF THERE IS ONE
        //if (customCursor != nullptr)
          //customCursor->addRenderItemToRenderList(guiRenderList);

      }

      bool GameGUI::checkCurrentScreenForAction(Game* game) {
        // IF THE GAME STATE DOESN'T HAVE A CORRESPONDING,
        // SCREEN DON'T DO ANYTHING
        GameStateManager& gsm = game->getGSM();
        GameState gameState = gsm.getCurrentGameState();
        ScreenGUI* gui = screens[static_cast<uint8_t>(gameState)];
        if (gui) {
          // CASCADE THE CHECK TO THE CURRENT SCREEN
          return gui->fireButtonCommand(game);
        }
        return false;
      }

      void GameGUI::registerButtonEventHandler(ButtonEventHandler* eventHandler) {
        for (auto& gui : screens) {
          // NOTE THAT THE "second" VALUE IN A MAP ELEMENT IS THE DATA,
          // THE "first" WOULD BE THE KEY
          gui.second->registerButtonEventHandler(eventHandler);
        }
      }

      void GameGUI::updateGUIState(long mouseX, long mouseY, GameState gameState) {
        ScreenGUI* screen = screens[static_cast<uint8_t>(gameState)];
        if (screen) {
          // LET THE CURRENT SCREEN UPDATE IT'S BUTTON STATES
          screen->updateAllButtons(mouseX, mouseY);
        }
      }
    }
  }
}
