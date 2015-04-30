/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutGame.cpp

    This is a test game application, a game that demonstrates use of the
    SideScrollerFramework to make a little scrolling, interactive demo. It
    demonstrates the rendering of images and text as well as responding to
    key presses and button cicks. Students should make their own named game
    applictions using a similar pattern, gradually adding other components,
    like additional gui controls, tiles, sprites, ai, and physics.
    */

#include "buggin_out_VS/stdafx.h"
#include "buggin_out/BugginOut.h"

// BugginOut GAME INCLUDES
#include "buggin_out/BugginOutButtonEventHandler.h"
#include "buggin_out/BugginOutCollisionListener.h"
#include "buggin_out/BugginOutDataLoader.h"
#include "buggin_out/BugginOutKeyEventHandler.h"
#include "buggin_out/BugginOutTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf/game/Game.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/gsm/state/GameState.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gui/Button.h"
#include "sssf/gui/Cursor.h"
#include "sssf/gui/OverlayImage.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/gui/ScreenGUI.h"
#include "sssf/input/GameInput.h"
#include "sssf/os/GameOS.h"
#include "sssf/text/GameText.h"
#include "LuaPlusPlus/LuaPlusFramework/LuaState.h"
#include "sssf/gsm/physics/Physics.h"
#include <SFML/Audio.hpp>

#ifdef CROSS
// SFML INCLUDES
#include "sssf/platforms/Cross/SFMLTextureManager.h"
#include "sssf/platforms/Cross/SFMLOS.h"
#include "sssf/platforms/Cross/SFMLGraphics.h"
#include "sssf/platforms/Cross/SFMLInput.h"

// C++ has built-in timing functionality now
#include "sssf/platforms/Cross/StandardTimer.h"

#else
// WINDOWS PLATFORM INCLUDES
#include "sssf/platforms/Windows/WindowsOS.h"
#include "sssf/platforms/Windows/WindowsInput.h"
#include "sssf/platforms/Windows/WindowsTimer.h"

// DIRECTX INCLUDES
#include "sssf/platforms/DirectX/DirectXGraphics.h"
#include "sssf/platforms/DirectX/DirectXTextureManager.h"
#endif // WIN32

#include "StandardWindowsLogger.h"

#ifdef CROSS
int main(int argc, char** argv)
#else
/*
    WinMain - This is the application's starting point. In this method we will construct a Game object,
    then initialize all the platform-dependent technologies, then construct all the custom data for our
    game, and then initialize the Game with our custom data. We'll then start the game loop.
    */
int WINAPI WinMain(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdShow)

#endif
{
  using namespace cse380::bugginout;
  using cse380::sssf::game::Game;
  using cse380::sssf::graphics::GameGraphics;
  using cse380::sssf::gui::GameGUI;

  StandardWindowsLogger EnableWindowsLogging;

  // CREATE THE GAME
  Game bugginOutGame;


  // FIRST WE'LL SETUP THE DATA LOADER, SINCE IT MAY NEED TO READ
  // IN DATA TO SETUP OTHER STUFF
  BugginOutDataLoader* bugginOutDataLoader = new BugginOutDataLoader();

#ifndef CROSS
  bugginOutDataLoader->initWinHandle(hInstance, nCmdShow);
#endif

  bugginOutGame.setDataLoader(bugginOutDataLoader);
  bugginOutDataLoader->loadGame(&bugginOutGame, W_DATA_DIR + W_INIT_FILE);

  // WHAT WE SHOULD BE DOING HERE IS LOADING THE GAME DATA FROM FILES. THIS
  // MEANS THE GUIS THEMSELVES AS WELL AS THE LEVELS. THAT WILL BE LEFT
  // FOR BECHMARK HWS. FOR NOW WE WILL JUST HARD CODE THE LOADING OF THE GUI

  // LOAD THE GUI STUFF, AGAIN, NOTE THAT THIS SHOULD REALLY
  // BE DONE FROM A FILE, NOT HARD CODED
  bugginOutDataLoader->loadGUI(&bugginOutGame, W_DATA_DIR + W_GUI_INIT_FILE);

  // SPECIFY WHO WILL HANDLE BUTTON EVENTS
  BugginOutButtonEventHandler bugginOutButtonHandler;
  GameGUI& gui = bugginOutGame.getGUI();
  gui.registerButtonEventHandler(&bugginOutButtonHandler);

  // SPECIFY WHO WILL HANDLE KEY EVENTS
  BugginOutKeyEventHandler bugginOutKeyHandler;
  bugginOutGame.getInput()->registerKeyHandler(&bugginOutKeyHandler);

  // THIS WILL HANDLE PHYSICS COLLISION EVENTS
  BugginOutCollisionListener* bugginOutCollisionListener = new BugginOutCollisionListener();
  bugginOutGame.getGSM().getPhysics().setCollisionListener(bugginOutCollisionListener);

  // START THE GAME LOOP
  sf::Music blah;
  if (blah.openFromFile("data/audio/FastTalkin.ogg")) {
	  blah.setLoop(true);
	  blah.play();
  }
  bugginOutGame.runGameLoop();

  // GAME'S OVER SHUTDOWN ALL THE STUFF WE CONSTRUCTED HERE
  delete bugginOutGame.getOS();
  delete bugginOutGame.getInput();
  delete bugginOutGame.getTimer();
  delete bugginOutGame.getGraphics();
  delete bugginOutGame.getText().getTextGenerator();

  // AND RETURN
  return 0;
}
