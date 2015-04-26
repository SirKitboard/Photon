/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    World.cpp

    See World.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/world/World.h"

#include "sssf/game/Game.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/gsm/world/WorldLayer.h"
#include "sssf/gsm/physics/Physics.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/graphics/RenderList.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/Viewport.h"


namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace world {

        using gsm::state::GameStateManager;
        using gui::GameGUI;
        using gui::Viewport;
        using graphics::GameGraphics;
        using graphics::RenderList;

        World::World(const int w, const int h) : worldWidth(w), worldHeight(h) {
			//getPhysicalWorld();
		    //physicalWorld = &world2;
			//	getPhysicalWorld();
        }
        World::~World() {
          unloadWorld();
        }

        void World::addLayer(WorldLayer* layerToAdd) {
          layers.push_back(layerToAdd);
        }

        void World::addWorldRenderItemsToRenderList(Game* game) const {
          GameStateManager& gsm = game->getGSM();
          GameGUI& gui = game->getGUI();

          if (gsm.isWorldRenderable()) {
            GameGraphics* graphics = game->getGraphics();
            RenderList& renderList = graphics->getWorldRenderList();
            Viewport& viewport = gui.getViewport();

            for (const WorldLayer* layer : layers) {
              layer->addRenderItemsToRenderList(renderList, viewport);
            }
          }
        }

        void World::unloadWorld() {
          for (WorldLayer* layer : layers) {
            delete layer;
          }

          layers.clear();
          worldWidth = 0;
          worldHeight = 0;
        }

        void World::update(Game* game) {
          // NOTE THAT THIS METHOD IS NOT IMPLEMENTED BUT COULD BE
          // SHOULD YOU WISH TO ADD ANY NON-COLLIDABLE LAYERS WITH
          // DYNAMIC CONTENT OR PARTICLE SYSTEMS
        }
      }
    }
  }
}
