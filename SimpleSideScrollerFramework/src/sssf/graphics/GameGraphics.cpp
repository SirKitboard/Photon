/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameGraphics.cpp

    See GameGraphics.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/graphics/GameGraphics.h"

#include "sssf/game/Game.h"
#include "sssf/graphics/RenderList.h"
#include "sssf/graphics/TextureManager.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/text/GameText.h"

const unsigned int DEFAULT_INIT_GUI_RENDER_LIST_SIZE = 100;
const unsigned int DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE = 1000;

namespace cse380 {
  namespace sssf {
    namespace graphics {
      using gsm::state::GameStateManager;
      using gui::GameGUI;

      GameGraphics::GameGraphics() :
        guiRenderList(DEFAULT_INIT_GUI_RENDER_LIST_SIZE),
        worldRenderList(DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE),
		lights(DEFAULT_INIT_GUI_RENDER_LIST_SIZE),
        guiTextureManager(nullptr),
        worldTextureManager(nullptr),
        debugTextShouldBeRendered(false) {}

      GameGraphics::~GameGraphics() {
        if (guiTextureManager) {
          delete guiTextureManager;
        }

        if (worldTextureManager) {
          delete worldTextureManager;
        }
      }

      void GameGraphics::clearWorldTextures() {
        // CLEAR LEVEL DATA STRUCURES
        worldTextureManager->clear();
        worldRenderList.clear();
      }

      void GameGraphics::fillRenderLists(Game* game) {
        // GENERATE RENDER LISTS FOR GAME WORLD AND GUI
        GameStateManager& gsm = game->getGSM();
        GameGUI& gui = game->getGUI();

        gsm.addGameRenderItemsToRenderList(game);
        gui.addRenderItemsToRenderList(game);
      }

      void GameGraphics::init(const uint16_t w, const uint16_t h) {
        // INIT SCREEN DIMENSIONS
        screenWidth = w;
        screenHeight = h;

        if (guiTextureManager) {
          delete guiTextureManager;
        }

        if (worldTextureManager) {
          delete worldTextureManager;
        }
        // Don't forget to clean up any possible old texture managers, else you'll
        // cause a real bad memory leak

        // GUI TEXTURES (like buttons, cursor, etc.)
        guiRenderList = RenderList(DEFAULT_INIT_GUI_RENDER_LIST_SIZE);
        guiTextureManager = createTextureManager();

        // LEVEL TEXTURES (like sprites, tiles, particles, etc.)
        worldRenderList = RenderList(DEFAULT_INIT_LEVEL_RENDER_LIST_SIZE);
        worldTextureManager = createTextureManager();

		lights = RenderList(RenderList(DEFAULT_INIT_GUI_RENDER_LIST_SIZE));
      }

      void GameGraphics::renderText(const GameText& text) {
        // FOR NOW WE ONLY USE TEXT RENDERING FOR DEBUG TEXT, BUT
        // IT COULD ALSO BE USED FOR GAME TEXT LIKE HEALTH STATS
        if (debugTextShouldBeRendered) {
          int numTextObjects = text.getNumTextObjectsToDraw();
          for (int i = 0; i < numTextObjects; i++) {
            const TextToDraw& textToDraw = text.getTextToDrawAtIndex(i);
            renderTextToDraw(textToDraw);
          }
        }
      }
    }
  }
}
