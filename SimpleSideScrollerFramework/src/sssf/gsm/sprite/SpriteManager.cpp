/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    SpriteManager.cpp

    See SpriteManager.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/sprite/SpriteManager.h"

#include "sssf/gsm/ai/Bot.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/game/Game.h"
#include "sssf/gsm/physics/PhysicalProperties.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/gsm/sprite/AnimatedSprite.h"
#include "sssf/gsm/sprite/AnimatedSpriteType.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/Viewport.h"
#include "sssf/graphics/TextureManager.h"
namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace sprite {

        using gsm::state::GameStateManager;
        using gui::GameGUI;
        using gui::Viewport;
        using physics::PhysicalProperties;
        using namespace graphics;

        SpriteManager::~SpriteManager() {
          this->unloadSprites();
        }

        void SpriteManager::addSpriteToRenderList(
          const AnimatedSprite& sprite,
          RenderList& renderList,
          const Viewport& viewport) const {
          // GET THE SPRITE TYPE INFO FOR THIS SPRITE
          const AnimatedSpriteType* spriteType = sprite.getSpriteType();
          const PhysicalProperties& pp = sprite.getPhysicalProperties();

          // IS THE SPRITE VIEWABLE?
          if (viewport.areWorldCoordinatesInViewport(
            pp.getX(),
            pp.getY(),
            spriteType->getTextureWidth(),
            spriteType->getTextureHeight())) {
            // SINCE IT'S VIEWABLE, ADD IT TO THE RENDER LIST
            renderList.addRenderItem(sprite.getCurrentImageID(),
              round(pp.getX() - viewport.getViewportX()),
              round(pp.getY() - viewport.getViewportY()),
              round(pp.getZ()),
              sprite.getAlpha(),
              spriteType->getTextureWidth(),
              spriteType->getTextureHeight(),
			  pp.getRotation() * 180 / PI);
          }
        }

        void SpriteManager::addSpriteItemsToRenderList(Game* game) const {
          GameStateManager& gsm = game->getGSM();
          GameGUI& gui = game->getGUI();

          if (gsm.isWorldRenderable()) {
            GameGraphics* graphics = game->getGraphics();
            RenderList& renderList = graphics->getWorldRenderList();
            Viewport& viewport = gui.getViewport();
			RenderList& lightList = graphics->getLightRenderList();
            // ADD THE PLAYER SPRITE
            addSpriteToRenderList(player, renderList, viewport);
			const PhysicalProperties& pp = player.getPhysicalProperties();
			int xpos = pp.getX();
			int ypos = pp.getY();
			if (pp.getVelocityX() < -0.01) {
				lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/Light.png"),
					round(pp.getX() - viewport.getViewportX()),
					round(pp.getY() - viewport.getViewportY())+150,
					round(pp.getZ()),
					255,
					300,
					300,
					180);
			}
			else if (pp.getVelocityY() < -0.01) {
				lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/Light.png"),
					round(pp.getX() - viewport.getViewportX())-150,
					round(pp.getY() - viewport.getViewportY()),
					round(pp.getZ()),
					255,
					300,
					300,
					270);
			}
			else if (pp.getVelocityY() > 0.01) {
				lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/Light.png"),
					round(pp.getX() - viewport.getViewportX()) + 150,
					round(pp.getY() - viewport.getViewportY()),
					round(pp.getZ()),
					255,
					300,
					300,
					90);
			}
			else {
				lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/Light.png"),
					round(pp.getX() - viewport.getViewportX()),
					round(pp.getY() - viewport.getViewportY())-150,
					round(pp.getZ()),
					255,
					300,
					300,
					0);
			}
            // NOW ADD THE REST OF THE SPRITES
            for (const Bot* bot : bots) {
              addSpriteToRenderList(*bot, renderList, viewport);
			  const PhysicalProperties& pp = bot->getPhysicalProperties();
			  int xpos = pp.getX();
			  int ypos = pp.getY();
			  if (pp.getVelocityX() < -0.01) {
				  lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/LightRed.png"),
					  round(pp.getX() - viewport.getViewportX()),
					  round(pp.getY() - viewport.getViewportY()) + 150,
					  round(pp.getZ()),
					  255,
					  300,
					  300,
					  180);
			  }
			  else if (pp.getVelocityY() < -0.01) {
				  lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/LightRed.png"),
					  round(pp.getX() - viewport.getViewportX()) - 150,
					  round(pp.getY() - viewport.getViewportY()),
					  round(pp.getZ()),
					  255,
					  300,
					  300,
					  270);
			  }
			  else if (pp.getVelocityY() > 0.01) {
				  lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/LightRed.png"),
					  round(pp.getX() - viewport.getViewportX()) + 150,
					  round(pp.getY() - viewport.getViewportY()),
					  round(pp.getZ()),
					  255,
					  300,
					  300,
					  90);
			  }
			  else {
				  lightList.addRenderItem(graphics->getGUITextureManager()->loadTexture(L"data/gui/overlays/LightRed.png"),
					  round(pp.getX() - viewport.getViewportX()),
					  round(pp.getY() - viewport.getViewportY()) - 150,
					  round(pp.getZ()),
					  255,
					  300,
					  300,
					  0);
			  }
            }
          }
        }

        void SpriteManager::addBot(Bot* botToAdd) {
          bots.push_back(botToAdd);
        }

        void SpriteManager::addSpriteType(AnimatedSpriteType* type) {
          spriteTypes[type->getSpriteTypeName()] = type;
        }

        void SpriteManager::clearSprites() {
          bots.clear();
          recyclableBots.clear();
        }

        void SpriteManager::unloadSprites() {
          for (auto& type : this->spriteTypes) {
            delete type.second;
          }

          this->spriteTypes.clear();
          this->clearSprites();
        }

        Bot* SpriteManager::removeBot(Bot* botToRemove) {
          this->recyclableBots.recycleBot(botToRemove);
          return nullptr;
        }

        void SpriteManager::update(Game* game) {
          // UPDATE THE PLAYER SPRITE
			pathfinder->updatePath(&player);
          player.updateSprite();

          // NOW UPDATE THE REST OF THE SPRITES
          for (Bot* bot : bots) {
            bot->think(game);
//            bot->updateSprite();
          }
        }
      }
    }
  }
}
