#pragma once

#include <string>

#ifdef CROSS
#include <SFML/Window.hpp>
#endif

namespace cse380 {
  namespace bugginout {
    using std::string;
    using std::wstring;

    /*
        BugginOutGame.h

        This file contains setup data for making the BugginOut game. Note that nearly
        all of this data would better serve us if loaded from files.
        */


    // HERE IS THE BugginOut GAME SETUP/INIT FILE. EVERYTHING WOULD START FROM HERE
    static  wstring    W_DATA_DIR = L"data/";
    static  wstring    W_LEVEL_1_DIR = L"data/levels/SideScrollerDesert/";
    static  wstring    W_LEVEL_1_NAME = L"SideScrollerDesertLevel.tmx";
	static  wstring    W_LEVEL_2_NAME = L"SideScrollerDesertLevel2.tmx";
	static  wstring    W_LEVEL_3_NAME = L"SideScrollerDesertLevel3.tmx";
    static  wstring    W_LEVEL_1_PATH = W_LEVEL_1_DIR + W_LEVEL_1_NAME;
	static  wstring    W_LEVEL_2_PATH = W_LEVEL_1_DIR + W_LEVEL_2_NAME;
	static  wstring    W_LEVEL_3_PATH = W_LEVEL_1_DIR + W_LEVEL_3_NAME;
    static  wstring    W_INIT_FILE = L"BugginOutInit.txt";
    static  wstring    W_GUI_INIT_FILE = L"BugginOutGUI.txt";
    static  string     W_DEBUG_FILE = "DebugOutput.txt";

    // HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
    static  wstring    W_TITLE = L"TITLE";
    static  wstring    W_SCREEN_WIDTH = L"SCREEN_WIDTH";
    static  wstring    W_SCREEN_HEIGHT = L"SCREEN_HEIGHT";
    static  wstring    W_USE_FULLSCREEN_MODE = L"USE_FULLSCREEN_MODE";
    static  wstring    W_TEXT_FONT_SIZE = L"TEXT_FONT_SIZE";
    static  wstring    W_VIEWPORT_OFFSET_X = L"VIEWPORT_OFFSET_X";
    static  wstring    W_VIEWPORT_OFFSET_Y = L"VIEWPORT_OFFSET_Y";
    static  wstring    W_TOGGLE_OFFSET_Y = L"TOGGLE_OFFSET_Y";
    static  wstring    W_FONT_COLOR_RED = L"FONT_COLOR_RED";
    static  wstring    W_FONT_COLOR_GREEN = L"FONT_COLOR_GREEN";
    static  wstring    W_FONT_COLOR_BLUE = L"FONT_COLOR_BLUE";
    static  wstring    W_COLOR_KEY_RED = L"COLOR_KEY_RED";
    static  wstring    W_COLOR_KEY_GREEN = L"COLOR_KEY_GREEN";
    static  wstring    W_COLOR_KEY_BLUE = L"COLOR_KEY_BLUE";

    // CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
    static  wstring    W_GO_TO_MM_COMMAND = L"Go To Main Menu";
    static  wstring    W_START_COMMAND = L"Start";
    static  wstring    W_EXIT_COMMAND = L"Exit";
    static  wstring    W_QUIT_COMMAND = L"Quit";
	static  wstring    W_RESUME_COMMAND = L"Resume";
	static  wstring    W_NEXT_LEVEL_COMMAND = L"Next";
	static  wstring    W_RELOAD_LEVEL_COMMAND = L"Reload";
    static  wstring    W_GREEN_CURSOR_PATH = L"GREEN_CURSOR_PATH";
    static  wstring    W_RED_CURSOR_PATH = L"RED_CURSOR_PATH";
	static  wstring	W_SPLASH_SCREEN_PATH = L"SPLASH_SCREEN_PATH";
	static  wstring	W_MAIN_MENU_PATH = L"MAIN_MENU_PATH";
	static  wstring	W_RDM_PATH = L"RDM_PATH";
	static  wstring	W_START_IMAGE_PATH = L"START_IMAGE_PATH";
	static  wstring	W_START_IMAGE_MO_PATH = L"START_IMAGE_MO_PATH";
	static  wstring	W_QUIT_IMAGE_PATH = L"QUIT_IMAGE_PATH";
	static  wstring	W_QUIT_IMAGE_MO_PATH = L"QUIT_IMAGE_MO_PATH";
	static  wstring	W_EXIT_IMAGE_PATH = L"EXIT_IMAGE_PATH";
	static  wstring	W_EXIT_IMAGE_MO_PATH = L"EXIT_IMAGE_MO_PATH";
	static  wstring W_RESUME_IMAGE_PATH = L"RESUME_IMAGE_PATH";
	static  wstring W_RESUME_IMAGE_MO_PATH = L"RESUME_IMAGE_MO_PATH";
	static  wstring W_NEXT_LEVEL_PATH = L"NEXT_LEVEL_PATH";
	static  wstring W_NEXT_LEVEL_MO_PATH = L"NEXT_LEVEL_MO_PATH";
	static  wstring W_RETRY_LEVEL_PATH = L"RETRY_LEVEL_PATH";
	static  wstring W_RETRY_LEVEL_MO_PATH = L"RETRY_LEVEL_MO_PATH";
	static  wstring	W_ABOUT_IMAGE_PATH = L"ABOUT_IMAGE_PATH";
	static  wstring	W_ABOUT_IMAGE_H_PATH = L"ABOUT_IMAGE_H_PATH";
	static  wstring	W_HELP_IMAGE_PATH = L"HELP_IMAGE_PATH";
	static  wstring	W_HELP_IMAGE_H_PATH = L"HELP_IMAGE_H_PATH";
	static  wstring W_WIN_IMAGE_PATH = L"WIN_IMAGE_PATH";
	static  wstring W_LOSE_IMAGE_PATH = L"LOSE_IMAGE_PATH";
	static  wstring W_THANKS_IMAGE_PATH = L"THANKS_IMAGE_PATH";

    // FOR OUR SPRITE
    static  wstring    SPRITE_TYPES_DIR = L"SPRITE_TYPES_DIR";
	static  wstring    SPRITE_TYPES_LIST = L"SPRITE_TYPES_LIST";
    static  wstring    JUMPING = L"JUMPING";

    // FOR THE GAME WORLD
    static  float      MAX_VIEWPORT_AXIS_VELOCITY = 20.0f;
    static  float      W_GRAVITY = 0.8f;
    static  float      JUMPING_BOT_MIN_CYCLES = 30;
    static  float      JUMPING_BOT_MAX_CYCLES = 120;
    static  float      JUMPING_BOT_MAX_VELOCITY = 40;
    static  float      PLAYER_INIT_X = 80.0f;
    static  float      PLAYER_INIT_Y = 100.0f;
    static  float      JUMP_SPEED = -24.0f;
    static  float      PLAYER_SPEED = 50000.0f;
    static  int        MIN_FPS = 5;
    static  int        MAX_FPS = 100;
    static  int        FPS_INC = 1;
    static  wstring    ATTACKING_RIGHT = L"ATTACKING_RIGHT";
    static  wstring    ATTACKING_LEFT = L"ATTACKING_LEFT";
    static  wstring    IDLE = L"IDLE";
	static	int			CURRENT_LEVEL = 0;

    // USED FOR MOVEMENT
#ifdef CROSS
    const unsigned int W_KEY = sf::Keyboard::W;
    const unsigned int A_KEY = sf::Keyboard::A;
    const unsigned int S_KEY = sf::Keyboard::S;
    const unsigned int D_KEY = sf::Keyboard::D;
    const unsigned int G_KEY = sf::Keyboard::G;
    const unsigned int P_KEY = sf::Keyboard::P;
    const unsigned int T_KEY = sf::Keyboard::T;
	const unsigned int L_KEY = sf::Keyboard::L;
    const unsigned int UP_KEY = sf::Keyboard::Up;
	const unsigned int LCTRL_KEY = sf::Keyboard::LControl;
	const unsigned int RCTRL_KEY = sf::Keyboard::RControl;
	const unsigned int ESC_KEY = sf::Keyboard::Escape;
    const unsigned int DOWN_KEY = sf::Keyboard::Down;
    const unsigned int LEFT_KEY = sf::Keyboard::Left;
    const unsigned int RIGHT_KEY = sf::Keyboard::Right;
    const unsigned int SPACE_KEY = sf::Keyboard::Space;
    const unsigned int SHIFT_KEY = sf::Keyboard::RShift;
    const unsigned int HOME_KEY = sf::Keyboard::Home;
    const unsigned int END_KEY = sf::Keyboard::End;
	const unsigned int ONE_KEY = sf::Keyboard::Num1;
	const unsigned int TWO_KEY = sf::Keyboard::Num2;
	const unsigned int THREE_KEY = sf::Keyboard::Num3;


    // THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
    const unsigned int C_KEY = sf::Keyboard::C;
#else
    const unsigned int W_KEY = (unsigned int)'W';
    const unsigned int A_KEY = (unsigned int)'A';
    const unsigned int S_KEY = (unsigned int)'S';
    const unsigned int D_KEY = (unsigned int)'D';
    const unsigned int G_KEY = (unsigned int)'G';
    const unsigned int P_KEY = (unsigned int)'P';
    const unsigned int T_KEY = (unsigned int)'T';
	const unsigned int P_KET = (unsigned int)'P';
    const unsigned int UP_KEY = VK_UP;
    const unsigned int DOWN_KEY = VK_DOWN;
    const unsigned int LEFT_KEY = VK_LEFT;
    const unsigned int RIGHT_KEY = VK_RIGHT;
    const unsigned int SPACE_KEY = VK_SPACE;
    const unsigned int SHIFT_KEY = VK_SHIFT;
    const unsigned int HOME_KEY = VK_HOME;
    const unsigned int END_KEY = VK_END;
    const unsigned int ESC_KEY = VK_ESCAPE;
	const unsigned int LCTRL_KEY = VK_LCONTROL;
	const unsigned int RCTRL_KEY = VK_RCONTROL;
	const unsigned int ONE_KEY = (unsigned int)'1';
	const unsigned int TWO_KEY = (unsigned int)'2';
	const unsigned int THREE_KEY = (unsigned int)'3';

    // THIS IS JUST FOR SHOWING HOW THE CURSOR CAN BE CHANGED
    const unsigned int C_KEY = (unsigned int)'C';
#endif // CROSS
  }
}