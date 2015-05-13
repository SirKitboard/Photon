/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutDataLoader.h

    This class provides a custom importer for the BugginOutGame to import
    game data, gui data, and world (i.e. level
#include <unordered_map>) data.
    */
#pragma once

#include <string>

#ifndef CROSS
#include <windows.h>
#endif

#include "sssf/data_loader/GameDataLoader.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace gui { class GameGUI; }
    namespace graphics {
      class GameGraphics;
      class TextureManager;
    }
    namespace gsm {
      namespace sprite {
        class AnimatedSpriteType;
      }
    }
  }
  namespace bugginout {
    using std::unordered_map;
    using std::wstring;

    using sssf::game::Game;
    using sssf::gui::GameGUI;
    using sssf::gsm::sprite::AnimatedSpriteType;
    using namespace sssf::graphics;


    class BugginOutDataLoader : public sssf::data::GameDataLoader {
    private:
#ifndef CROSS
      // WE NEED THESE GUYS TO INIT OUR WINDOWS WINDOW
      HINSTANCE hInstance;
      int nCmdShow;
#endif
    public:
      BugginOutDataLoader() {}
      ~BugginOutDataLoader() {}

#ifndef CROSS
      // INLINED MUTATOR METHOD
      void initWinHandle(HINSTANCE initHInstance, int initNCmdShow) {
        hInstance = initHInstance;
        nCmdShow = initNCmdShow;
      }
#endif // CROSS

      // THESE SHOULD BE LOADING DATA FROM FILES. THEY
      // ARE ALL DEFINED IN BugginOutDataLoader.cpp
      void loadGame(Game*, const wstring&) override;
      void loadGUI(Game*, const wstring&) override;
      void loadWorld(Game*, const wstring&) override;
    private:
      // THESE ARE HARD-CODED EXAMPLES OF GUI DATA LOADING
      void hardCodedLoadGUIExample(Game* game);

      // SOME HELPER METHODS FOR SETTING UP THE GAME
      void initColors(GameGraphics*, unordered_map<wstring, wstring>&);
      void initViewport(GameGUI&, unordered_map<wstring, wstring>&);
      void makeRandomJumpingBot(Game*, AnimatedSpriteType*, float, float);
	  void makeLightBot(Game*, AnimatedSpriteType*, float, float);

      // AND SOME HELPER METHODS FOR LOADING THE GUI
      void initCursor(GameGUI&, TextureManager*);
      void initSplashScreen(Game*, GameGUI&, TextureManager*);
      void initMainMenu(GameGUI&, TextureManager*);
      void initInGameGUI(Game*) override;
	  void initWinGUI(Game*) override;
	  void initLoseGUI(Game*) override;
	  void loadLevel1(Game* game) override;
	  void loadLevel2(Game* game) override;
	  void loadLevel3(Game* game) override;
	  void loadLevelHelper(Game*, int) override;
	  

    };
  }
}