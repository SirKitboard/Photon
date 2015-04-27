/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameStateManager.cpp

    See GameStateManager.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/state/GameStateManager.h"

#include "sssf/data_loader/GameDataLoader.h"
#include "sssf/game/Game.h"
#include "sssf/gsm/state/GameState.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace state {
        using game::Game;

        GameStateManager::GameStateManager() :
          currentGameState(GameState::GS_SPLASH_SCREEN),
          currentLevel(NO_LEVEL_LOADED) {}

        void GameStateManager::addGameRenderItemsToRenderList(Game* game) const {
          // FIRST THE STATIC WORLD
          world.addWorldRenderItemsToRenderList(game);

          // THEN THE SPRITE MANAGER
          spriteManager.addSpriteItemsToRenderList(game);
        }

        void GameStateManager::addLevel(const wstring& levelToAddName,
          const wstring& levelToAddFileNameAndPath) {
          levelNames.push_back(levelToAddName);
          levelFileNamesWithRelativePath.push_back(levelToAddFileNameAndPath);
        }

        unsigned int GameStateManager::getLevelNum(const wstring& levelName) const {
          return find(levelNames.begin(), levelNames.end(),
            levelName) - levelNames.begin();
        }

        void GameStateManager::loadLevel(Game* game, unsigned int initLevel) {
          if ((initLevel > NO_LEVEL_LOADED) && (initLevel <= levelNames.size())) {
            if (currentLevel != NO_LEVEL_LOADED)
              unloadCurrentLevel();
            currentLevel = initLevel;
            wstring& fileToLoad = levelFileNamesWithRelativePath[currentLevel];
            data::GameDataLoader* dataLoader = game->getDataLoader();
            dataLoader->loadWorld(game, fileToLoad);
          }
        }

        void GameStateManager::loadLevel(Game* game, const wstring& levelName) {
          unsigned int levelIndex = getLevelNum(levelName);
          loadLevel(game, levelIndex);
        }

        void GameStateManager::shutdown() {
          // MAKE SURE THE GAME LOOP DOESN'T GO AROUND AGAIN
          currentGameState = GameState::GS_EXIT_GAME;

          // CLEAR LEFT OVER DATA
          if (isWorldRenderable())
            unloadCurrentLevel();
        }

        void GameStateManager::unloadCurrentLevel() {
          spriteManager.clearSprites();
          spriteManager.unloadSprites();
          world.unloadWorld();
        }

        void GameStateManager::update(Game* game) {
          spriteManager.update(game);
          world.update(game);
          if (true) {
            physics.update(game);
          }
        }
      }
    }
  }
}