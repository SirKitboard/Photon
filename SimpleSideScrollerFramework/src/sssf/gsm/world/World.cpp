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
			RenderList& tileList = graphics->getTileRenderList();
            Viewport& viewport = gui.getViewport();

            for (const WorldLayer* layer : layers) {
              layer->addRenderItemsToRenderList(renderList, tileList,viewport);
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

		bool World::isInsideCollidableTile(int centerX, int centerY)
		{
			vector<WorldLayer*>::iterator it = layers.begin();
			while (it != layers.end())
			{
				WorldLayer *layer = (*it);
				if (layer->hasCollidableTiles())
				{
					if (layer->isInsideCollidableTile(centerX, centerY))
						return true;
				}
				it++;
			}
			return false;
		}

		bool World::overlapsCollidableTiles(int centerX, int centerY, int nodeWidth, int nodeHeight)
		{
			vector<WorldLayer*>::iterator it = layers.begin();
			while (it != layers.end())
			{
				WorldLayer *layer = (*it);
				if (layer->hasCollidableTiles())
				{
					physics::AABB aabb;
					aabb.setCenterX((float)centerX);
					aabb.setCenterY((float)centerY);
					aabb.setWidth((float)nodeWidth);
					aabb.setHeight((float)nodeHeight);
					bool overlaps = layer->overlapsCollidableTile(aabb);
					if (overlaps)
						return true;
				}
				it++;
			}
			return false;
		}
		int	World::getCollidableGridColumns()
		{
			int maxColumns = 0;
			vector<WorldLayer*>::iterator it = layers.begin();
			while (it != layers.end())
			{
				WorldLayer *layer = (*it);
				if (layer->hasCollidableTiles())
				{
					int numColumns = layer->getColumns();
					if (numColumns > maxColumns)
						maxColumns = numColumns;
				}
				it++;
			}
			return maxColumns;
		}

		int World::getCollidableGridRows()
		{
			int maxRows = 0;
			vector<WorldLayer*>::iterator it = layers.begin();
			while (it != layers.end())
			{
				WorldLayer *layer = (*it);
				if (layer->hasCollidableTiles())
				{
					int numRows = layer->getRows();
					if (numRows > maxRows)
						maxRows = numRows;
				}
				it++;
			}
			return maxRows;
		}
      }
    }
  }
}
