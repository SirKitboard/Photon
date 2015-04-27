#include "buggin_out_VS/stdafx.h"
#include "buggin_out/BugginOutDataLoader.h"

// BugginOut GAME INCLUDES
#include "buggin_out/BugginOutButtonEventHandler.h"
#include "buggin_out/BugginOut.h"
#include "buggin_out/BugginOutKeyEventHandler.h"
#include "buggin_out/BugginOutTextGenerator.h"

// GAME OBJECT INCLUDES
#include "sssf/game/Game.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/gsm/ai/bots/RandomJumpingBot.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/gsm/physics/Physics.h"
#include "sssf/gsm/sprite/SpriteManager.h"
#include "sssf/gsm/state/GameState.h"
#include "sssf/gsm/world/TiledLayer.h"
#include "sssf/gui/Button.h"
#include "sssf/gui/OverlayImage.h"
#include "sssf/gui/Cursor.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/gui/ScreenGUI.h"
#include "sssf/input/GameInput.h"
#include "sssf/os/GameOS.h"
#include "sssf/text/GameText.h"

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

// TMX MAP LOADING
#include "tmxmi/TMXMapImporter.h"

// ANIMATED SPRITE TYPE LOADING
#include "psti/PoseurSpriteTypesImporter.h"
#include <sssf/gsm/ai/pathfinding/OrthographicGridPathfinder.h>
#include <sssf/gsm/ai/bots/RoamingLightBot.h>

/*
    loadGame - This method loads the setup game data into the game and
    constructs all the needed objects for the game to work.
    */
namespace cse380 {
  namespace bugginout {
#ifdef CROSS
    using namespace sssf::sfml;
    using namespace sssf::cpp;
#else
    using namespace sssf::win32;
#endif
    using namespace sssf::gsm::ai;
    using namespace sssf::gsm::ai::bots;
    using namespace sssf::gsm::physics;
    using namespace sssf::gsm::sprite;
    using namespace sssf::gsm::state;
    using namespace sssf::gui;
    using sssf::game::Game;
	using std::stof;

    void BugginOutDataLoader::loadGame(Game* game, const wstring& path) {

      // AND LET'S READ IN THE GAME SETUP INFO
      // FIRST LOAD ALL THE PROPERTIES
      unordered_map<wstring, wstring> properties;
      loadGameProperties(game, properties, path);

      // WE NEED THE TITLE AND USE_FULLSCREEN_MODE TO INITIALIZE OUR WINDOW
      wstring& titleProp = properties[W_TITLE];
      wstring& useFullscreenProp = properties[W_USE_FULLSCREEN_MODE];
      bool useFullscreen = (useFullscreenProp == L"true");

      // GET THE SCREEN WIDTH AND HEIGHT
      wstring& screenWidthProp = properties[W_SCREEN_WIDTH];
      wstring& screenHeightProp = properties[W_SCREEN_HEIGHT];
      int screenWidth = stoi(screenWidthProp), screenHeight = stoi(screenHeightProp);
	  W_GREEN_CURSOR_PATH = properties[W_GREEN_CURSOR_PATH];
	  W_RED_CURSOR_PATH = properties[W_RED_CURSOR_PATH];
	  W_SPLASH_SCREEN_PATH = properties[W_SPLASH_SCREEN_PATH];
	  W_START_IMAGE_PATH = properties[W_START_IMAGE_PATH];
	  W_START_IMAGE_MO_PATH = properties[W_START_IMAGE_MO_PATH];
	  W_MAIN_MENU_PATH = properties[W_MAIN_MENU_PATH];
	  W_QUIT_IMAGE_PATH = properties[W_QUIT_IMAGE_PATH];
	  W_QUIT_IMAGE_MO_PATH = properties[W_QUIT_IMAGE_MO_PATH];
	  W_EXIT_IMAGE_PATH = properties[W_EXIT_IMAGE_PATH];
	  W_EXIT_IMAGE_MO_PATH = properties[W_EXIT_IMAGE_MO_PATH];
	  W_ABOUT_IMAGE_PATH = properties[W_ABOUT_IMAGE_PATH];
	  W_ABOUT_IMAGE_H_PATH = properties[W_ABOUT_IMAGE_H_PATH];
	  W_HELP_IMAGE_PATH = properties[W_HELP_IMAGE_PATH];
	  W_HELP_IMAGE_H_PATH = properties[W_HELP_IMAGE_H_PATH];
	  SPRITE_TYPES_DIR = properties[SPRITE_TYPES_DIR];
	  SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + properties[SPRITE_TYPES_LIST];
	  wstring& viewPortAxisProp = properties[L"MAX_VIEWPORT_AXIS_VELOCITY"];
	  MAX_VIEWPORT_AXIS_VELOCITY = stof(viewPortAxisProp);
	  W_GRAVITY = stof(properties[L"W_GRAVITY"]);
	  JUMPING_BOT_MIN_CYCLES = stoi(properties[L"JUMPING_BOT_MIN_CYCLES"]);
	  JUMPING_BOT_MAX_CYCLES = stoi(properties[L"JUMPING_BOT_MAX_CYCLES"]);
	  JUMPING_BOT_MAX_VELOCITY = stoi(properties[L"JUMPING_BOT_MAX_VELOCITY"]);
	  PLAYER_INIT_X = stof(properties[L"PLAYER_INIT_X"]);
	  PLAYER_INIT_Y = stof(properties[L"PLAYER_INIT_Y"]);
	  JUMP_SPEED = stof(properties[L"JUMP_SPEED"]);
	  PLAYER_SPEED = stof(properties[L"PLAYER_SPEED"]);
	  MIN_FPS = stoi(properties[L"MIN_FPS"]);
	  MAX_FPS = stoi(properties[L"MAX_FPS"]);
	  FPS_INC = stoi(properties[L"FPS_INC"]);

#ifdef CROSS
      sf::VideoMode mode(screenWidth, screenHeight);
      unsigned int style = useFullscreen ? sf::Style::Fullscreen : sf::Style::Default;
      SFMLOS* bugginOutOS = new SFMLOS(mode, titleProp, style, game);

      // RENDERING WILL BE DONE USING SFML
      SFMLGraphics* bugginOutGraphics = new SFMLGraphics(game);

      SFMLInput* bugginOutInput = new SFMLInput(game);
      StandardTimer* bugginOutTimer = new StandardTimer();
      // C++11 has built-in timing functionality
#else
      // MAKE A CUSTOM GameOS OBJECT (WindowsOS) FOR SOME WINDOWS
      // PLATFORM STUFF, INCLUDING A Window OF COURSE
      WindowsOS* bugginOutOS = new WindowsOS(
        hInstance,
        nCmdShow,
        useFullscreen,
        titleProp,
        screenWidth, screenHeight,
        game
        );

      // RENDERING WILL BE DONE USING DirectX
      DirectXGraphics* bugginOutGraphics = new DirectXGraphics(game);

      WindowsInput* bugginOutInput = new WindowsInput();
      // WE'LL USE WINDOWS PLATFORM METHODS FOR GETTING INPUT

      // AND WINDOWS FOR THE TIMER AS WELL
      WindowsTimer* bugginOutTimer = new WindowsTimer();
#endif // CROSS

      wstring& textFontSizeProp = properties[W_TEXT_FONT_SIZE];
      int textFontSize = stoi(textFontSizeProp);

      bugginOutGraphics->init(screenWidth, screenHeight);
      bugginOutGraphics->initGraphics(bugginOutOS, useFullscreen);
      bugginOutGraphics->initTextFont(textFontSize);

      // AND NOW LOAD THE COLORS THE GRAPHICS WILL USE
      // AS A COLOR KEY AND FOR RENDERING TEXT
      initColors(bugginOutGraphics, properties);

      // NOW INITIALIZE THE Game WITH ALL THE
      // PLATFORM AND GAME SPECIFIC DATA WE JUST CREATED
      game->initPlatformPlugins(
        bugginOutGraphics,
        bugginOutInput,
        bugginOutOS,
        bugginOutTimer
        );

      // LOAD OUR CUSTOM TEXT GENERATOR, WHICH DRAWS
      // TEXT ON THE SCREEN EACH FRAME
      GameText& text = game->getText();
      BugginOutTextGenerator* bugginOutTextGenerator = new BugginOutTextGenerator();
      text.setTextGenerator(bugginOutTextGenerator);
      text.initDebugFile(W_DEBUG_FILE);
      bugginOutTextGenerator->initText(game);

      // INIT THE VIEWPORT TOO
      initViewport(game->getGUI(), properties);

    }

    /*
        initColors - this helper method loads the color key, used for loading
        images, and the font color, used for rendering text.
        */
    void BugginOutDataLoader::initColors(
      GameGraphics* graphics,
      unordered_map<wstring, wstring>& properties
      ) {
      wstring& fontRedProp = properties[W_FONT_COLOR_RED];
      wstring& fontGreenProp = properties[W_FONT_COLOR_GREEN];
      wstring& fontBlueProp = properties[W_FONT_COLOR_BLUE];
      int fontRed = stoi(fontRedProp), fontGreen = stoi(fontGreenProp),
        fontBlue = stoi(fontBlueProp);

      // COLOR USED FOR RENDERING TEXT
      graphics->setFontColor(fontRed, fontGreen, fontBlue);

      wstring& keyRedProp = properties[W_COLOR_KEY_RED];
      wstring& keyGreenProp = properties[W_COLOR_KEY_GREEN];
      wstring& keyBlueProp = properties[W_COLOR_KEY_BLUE];
      int keyRed = stoi(keyRedProp), keyGreen = stoi(keyGreenProp),
        keyBlue = stoi(keyBlueProp);

      // COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
      // NOTE, IF YOU WISH TO USE PNG IMAGES THAT CONTAIN ALPHA
      // CHANNEL DATA, YOU DON'T NEED A COLOR KEY
      graphics->setColorKey(keyRed, keyGreen, keyBlue);
    }

    /*
        loadGUI - This method loads all the GUI assets described in the guiInitFile
        argument. Note that we are loading all GUI art for all GUIs when the application
        first starts. We'll learn a better technique later in the semester.
        */
    void BugginOutDataLoader::loadGUI(Game* game, const wstring& path) {
      // WE'RE JUST GOING TO IGNORE THE GUI FILE FOR NOW.
      // FOR THE MOMENT WE ARE CALLING THIS HARD-CODED GUI LOADER
      hardCodedLoadGUIExample(game);
    }

    /*
        loadLevel - This method should load the data the level described by the
        levelInitFile argument in to the Game's game state manager.
        */
    void BugginOutDataLoader::loadWorld(sssf::game::Game* game, const wstring& path) {
      // LOAD THE LEVEL'S SPRITE IMAGES
      psti::PoseurSpriteTypesImporter psti(game);
      psti.loadSpriteTypes(SPRITE_TYPES_LIST);

      // LOAD THE LEVEL'S BACKGROUND TILES
      tmxmi::TMXMapImporter tmxMapImporter;
      tmxMapImporter.loadWorld(game, path);

      // LET'S MAKE A PLAYER SPRITE
      // @TODO - IT WOULD BE BETTER TO LOAD THIS STUFF FROM A FILE
      GameStateManager& gsm = game->getGSM();
      Physics& physics = gsm.getPhysics();
      physics.setGravity(W_GRAVITY);
	  pathfinding::OrthographicGridPathfinder *pathfinder = new pathfinding::OrthographicGridPathfinder(game);
      SpriteManager& spriteManager = gsm.getSpriteManager();
	  spriteManager.setPathfinder(pathfinder);
      AnimatedSprite& player = spriteManager.getPlayer();
      physics.addCollidableObject(&player);

      AnimatedSpriteType* playerSpriteType = spriteManager.getSpriteType(L"red_box_man");
      player.setSpriteType(playerSpriteType);
      player.setAlpha(255);
      player.setCurrentState(IDLE);
      PhysicalProperties& playerProps = player.getPhysicalProperties();
	  playerProps.initBody(game->getGameWorld());
      playerProps.setPosition(PLAYER_INIT_X, PLAYER_INIT_Y);
      playerProps.setVelocity(0.0f, 0.0f);
      playerProps.setAcceleration(0, 0);
      player.setOnTileThisFrame(false);
      player.setOnTileLastFrame(false);
      player.affixTightAABBBoundingVolume();

      AnimatedSpriteType* botSpriteType = spriteManager.getSpriteType(L"green_round_man");
      // AND LET'S ADD A BUNCH OF RANDOM JUMPING BOTS, FIRST ALONG
      // A LINE NEAR THE TOP

      // Ideally, you should load enemy positions, types, etc. from a file.
      // Learn how to use Tiled's custom data; it *will* be useful.
      // - Jesse

     /* for (int i = 2; i <= 26; i++) {
        float botX = 400.0f + (i * 100.0f);
        float botY = 100.0f;
        makeRandomJumpingBot(game, botSpriteType, botX, botY);
      }*/

      // AND THEN STRATEGICALLY PLACED AROUND THE LEVEL
	  makeLightBot(game, playerSpriteType, 200, 800);
	  //makeLightBot(game, playerSpriteType, 1000, 350);
	  //makeLightBot(game, playerSpriteType, 1200, 1350);
	  //makeLightBot(game, playerSpriteType, 120, 1250);
	  //makeLightBot(game, playerSpriteType, 220, 1250);
    /*  makeRandomJumpingBot(game, botSpriteType, 400, 100);
      makeRandomJumpingBot(game, botSpriteType, 200, 400);
      makeRandomJumpingBot(game, botSpriteType, 400, 400);
      makeRandomJumpingBot(game, botSpriteType, 800, 700);
      makeRandomJumpingBot(game, botSpriteType, 900, 700);
      makeRandomJumpingBot(game, botSpriteType, 1000, 700);
      makeRandomJumpingBot(game, botSpriteType, 100, 1000);
      makeRandomJumpingBot(game, botSpriteType, 300, 1000);
      makeRandomJumpingBot(game, botSpriteType, 500, 1000);
      makeRandomJumpingBot(game, botSpriteType, 100, 1400);
      makeRandomJumpingBot(game, botSpriteType, 400, 1400);
      makeRandomJumpingBot(game, botSpriteType, 700, 1400);*/

      // AND THEN A BUNCH LINED UP NEAR THE LEVEL EXIT
      /*for (int i = 0; i < 14; i++)
        makeRandomJumpingBot(game, botSpriteType, 1700.0f + (i*100.0f), 1300.0f);*/

    }

    void BugginOutDataLoader::makeRandomJumpingBot(Game* game,
      AnimatedSpriteType* randomJumpingBotType, float initX, float initY) {
      SpriteManager& spriteManager = game->getGSM().getSpriteManager();
      Physics& physics = game->getGSM().getPhysics();
      RandomJumpingBot* bot = new RandomJumpingBot(game->getGameWorld(),physics, 30, 120, 40);
      physics.addCollidableObject(bot);
	  bot->setSpriteType(randomJumpingBotType);
	  bot->setCurrentState(JUMPING);
	  bot->setAlpha(255);
	  spriteManager.addBot(bot);
	  bot->affixTightAABBBoundingVolume();

    }

	void BugginOutDataLoader::makeLightBot(Game* game, AnimatedSpriteType* sprite_type, float initX, float initY) {
		SpriteManager& spriteManager = game->getGSM().getSpriteManager();
		Physics& physics = game->getGSM().getPhysics();
		RoamingLightBot* bot = new RoamingLightBot(game,  30, 120, 40, 0);
		physics.addCollidableObject(bot);
		PhysicalProperties& pp = bot->getPhysicalProperties();
		pp.initBody(game->getGameWorld());
		pp.setPosition(initX, initY);
		//AnimatedSpriteType* playerSpriteType = spriteManager.getSpriteType(L"red_box_man");
		bot->setSpriteType(sprite_type);
		bot->setAlpha(255);
		bot->setCurrentState(IDLE);
		PhysicalProperties& playerProps = bot->getPhysicalProperties();
		playerProps.initBody(game->getGameWorld());
		playerProps.setPosition(initX, initY);
		playerProps.setVelocity(0.0f, 0.0f);
		playerProps.setAcceleration(0, 0);
		bot->setOnTileThisFrame(false);
		bot->setOnTileLastFrame(false);
		bot->affixTightAABBBoundingVolume();
		spriteManager.addBot(bot);
    }

    /*
        initBugginOutGUI - This method builds a GUI for the BugginOut Game application.
        Note that we load all the GUI components from this method, including
        the ScreenGUI with Buttons and Overlays and the Cursor.
        */
    void BugginOutDataLoader::hardCodedLoadGUIExample(Game* game) {
      GameGUI& gui = game->getGUI();
      GameGraphics* graphics = game->getGraphics();

      TextureManager* guiTextureManager = graphics->getGUITextureManager();

      // SETUP THE CURSOR VIA OUR HELPER METHOD
      initCursor(gui, guiTextureManager);
      initSplashScreen(game, gui, guiTextureManager);
      initMainMenu(gui, guiTextureManager);
      initInGameGUI(gui, guiTextureManager);
    }

    /*
        initCursor - initializes a simple little cursor for the gui.
        */


    void BugginOutDataLoader::initCursor(GameGUI& gui,
      TextureManager* guiTextureManager) {
      // SETUP THE CURSOR
      vector<size_t> imageIDs;
      size_t imageID;

      // - FIRST LOAD THE GREEN CURSOR IMAGE
      imageID = guiTextureManager->loadTexture(W_GREEN_CURSOR_PATH);
      imageIDs.push_back(imageID);

      // - AND NOW THE RED ONE
      imageID = guiTextureManager->loadTexture(W_RED_CURSOR_PATH);
      imageIDs.push_back(imageID);

      // - NOW BUILD AND LOAD THE CURSOR
      Cursor* cursor = new Cursor(
        imageIDs,
        imageIDs[0],
        0,
        0,
        0,
        255,
        32,
        32
        );
      gui.setCursor(cursor);
    }

    /*
        initSplashScreen - initializes the game's splash screen gui.
        */
    void BugginOutDataLoader::initSplashScreen(Game* game, GameGUI& gui,
      TextureManager* guiTextureManager) {
      // NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
      ScreenGUI* splashScreenGUI = new ScreenGUI();

      // WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
      size_t normalTextureID = guiTextureManager->loadTexture(W_SPLASH_SCREEN_PATH);
      size_t mouseOverTextureID = normalTextureID;

      // INIT THE QUIT BUTTON
      Button* buttonToAdd = new Button(
        normalTextureID,
        mouseOverTextureID,
        0,
        0,
        0,
        255,
        game->getGraphics()->getScreenWidth(),
        game->getGraphics()->getScreenHeight(),
        false,
        W_GO_TO_MM_COMMAND);
      splashScreenGUI->addButton(buttonToAdd);

      // AND REGISTER IT WITH THE GUI
      gui.addScreenGUI(GameState::GS_SPLASH_SCREEN, splashScreenGUI);
    }

    /*
        initMainMenu - initializes the game's main menu gui.
        */
    void BugginOutDataLoader::initMainMenu(GameGUI& gui,
      TextureManager* guiTextureManager) {
      // NOW LET'S LOAD A MAIN MENU GUI SCREEN
      ScreenGUI* mainMenuGUI = new ScreenGUI();
      size_t imageID = guiTextureManager->loadTexture(W_MAIN_MENU_PATH);
      OverlayImage imageToAdd;
      imageToAdd.x = 0;
      imageToAdd.y = 0;
      imageToAdd.z = 0;
      imageToAdd.alpha = 255;
      imageToAdd.width = 512;
      imageToAdd.height = 512;
      imageToAdd.imageID = imageID;
      mainMenuGUI->addOverlayImage(imageToAdd);

      // - GET THE BUTTON COMMAND AND IMAGE IDs
      size_t normalTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_PATH);
      size_t mouseOverTextureID = guiTextureManager->loadTexture(W_QUIT_IMAGE_MO_PATH);

      // - INIT THE EXIT BUTTON
      Button* buttonToAdd = new Button(
        normalTextureID,
        mouseOverTextureID,
		104,
		457,
        0,
        255,
        200,
        100,
        false,
        W_EXIT_COMMAND
        );

      // AND NOW LOAD IT INTO A ScreenGUI
      mainMenuGUI->addButton(buttonToAdd);

      // - GET THE BUTTON COMMAND AND IMAGE IDs

      normalTextureID = guiTextureManager->loadTexture(W_START_IMAGE_PATH);
      mouseOverTextureID = guiTextureManager->loadTexture(W_START_IMAGE_MO_PATH);

      // - INIT THE START BUTTON
      buttonToAdd = new Button(
        normalTextureID,
        mouseOverTextureID,
		105,
		209,
        0,
        255,
        200,
        100,
        false,
        W_START_COMMAND
        );

      // AND NOW LOAD IT INTO A ScreenGUI
      mainMenuGUI->addButton(buttonToAdd);

	  //ABOUT BUTTON
	  normalTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_PATH);
	  mouseOverTextureID = guiTextureManager->loadTexture(W_ABOUT_IMAGE_H_PATH);


	  buttonToAdd = new Button(
		  normalTextureID,
		  mouseOverTextureID,
		  630,
		  209,
		  0,
		  255,
		  200,
		  100,
		  false,
		  L""
		  );

	  mainMenuGUI->addButton(buttonToAdd);

	  //HELP BUTTON
	  normalTextureID = guiTextureManager->loadTexture(W_HELP_IMAGE_PATH);
	  mouseOverTextureID = guiTextureManager->loadTexture(W_HELP_IMAGE_H_PATH);


	  buttonToAdd = new Button(
		  normalTextureID,
		  mouseOverTextureID,
		  630,
		  457,
		  0,
		  255,
		  200,
		  100,
		  false,
		  L""
		  );

	  mainMenuGUI->addButton(buttonToAdd);

      // AND LET'S ADD OUR SCREENS
      gui.addScreenGUI(GameState::GS_MAIN_MENU, mainMenuGUI);
    }

    /*
        initInGameGUI - initializes the game's in-game gui.
        */
    void BugginOutDataLoader::initInGameGUI(GameGUI& gui,
      TextureManager* guiTextureManager) {
      // NOW ADD THE IN-GAME GUI
      ScreenGUI* inGameGUI = new ScreenGUI();

      size_t normalTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);
      size_t mouseOverTextureID = guiTextureManager->loadTexture(W_EXIT_IMAGE_PATH);

      // INIT THE QUIT BUTTON
      Button* buttonToAdd = new Button(
        normalTextureID,
        mouseOverTextureID,
        0,
        0,
        0,
        255,
        200,
        100,
        false,
        W_QUIT_COMMAND
        );
      inGameGUI->addButton(buttonToAdd);

      // AND LET'S ADD OUR SCREENS
      gui.addScreenGUI(GameState::GS_GAME_IN_PROGRESS, inGameGUI);
    }

    /*
        initViewport - initializes the game's viewport.
        */
    void BugginOutDataLoader::initViewport(
      GameGUI& gui,
      unordered_map<wstring, wstring>& properties
      ) {
      // AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
      // WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
      // THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
      // THE VIEWPORT IN THE Y AXIS
      Viewport& viewport = gui.getViewport();

      wstring& viewportOffsetXProp = properties[W_VIEWPORT_OFFSET_X];
      wstring& viewportOffsetYProp = properties[W_VIEWPORT_OFFSET_Y];
      wstring& toggleOffsetYProp = properties[W_TOGGLE_OFFSET_Y];
      wstring& screenWidthProp = properties[W_SCREEN_WIDTH];
      wstring& screenHeightProp = properties[W_SCREEN_HEIGHT];

      int viewportOffsetX = stoi(viewportOffsetXProp);
      int viewportOffsetY = stoi(viewportOffsetYProp);
      int toggleOffsetY = stoi(toggleOffsetYProp);
      int screenWidth = stoi(screenWidthProp);
      int screenHeight = stoi(screenHeightProp);
      int viewportWidth = screenWidth - viewportOffsetX;
      int viewportHeight = screenHeight - viewportOffsetY;

      viewport.setViewportWidth(viewportWidth);
      viewport.setViewportHeight(viewportHeight);
      viewport.setViewportOffsetX(viewportOffsetX);
      viewport.setViewportOffsetY(viewportOffsetY);
      viewport.setToggleOffsetY(toggleOffsetY);
    }
  }
}