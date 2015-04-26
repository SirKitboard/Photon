/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameGUI.h
    */
#pragma once

#include <cstdint>
#include <unordered_map>

#include "sssf/gsm/state/GameState.h"
#include "sssf/gui/Viewport.h"

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }
    namespace input { class ButtonEventHandler; }

    namespace gui {
      using input::ButtonEventHandler;
      using gsm::state::GameState;
      class Cursor;
      class ScreenGUI;

      /// <summary>
      /// Manages all data for the game gui, the cursor and the ScreenGUIs,
      /// which store the buttons and overlaid images. For example, the splash
      /// screen might be one screen, the main menu might be another, and the
      /// game as it's running might be another.
      /// </summary>
      class GameGUI {
      public:
        /**
         * Default Constructor, it sets up the data structure for storing all
         * ScreenGUIs. These should subsequently be added using addScreenGUI
         */
        GameGUI();

        /**
          Destructor, this would be called when the application is closing, it
          will clean up all memory for the GUI, including the cursor and all
          ScreenGUIs.
          */
        ~GameGUI();

        // No copying allowed!
        GameGUI(const GameGUI&) = delete;
        GameGUI& operator=(const GameGUI&) = delete;

        // INLINED ACCESSOR METHODS
        Cursor* getCursor() { return customCursor; }
        const Cursor* getCursor() const { return customCursor; }

        ScreenGUI* getScreen(const GameState gameState) {
          return screens.at(static_cast<uint8_t>(gameState));
          // I don't feel like implementing a hash function (however trivial) for GameState
        }

        const ScreenGUI* getScreen(const GameState state) const {
          return screens.at(static_cast<uint8_t>(state));
        }

        Viewport& getViewport() { return viewport; }
        const Viewport& getViewport() const { return viewport; }

        // INLINED MUTATOR METHOD
        void setCursor(Cursor* cursor) { customCursor = cursor; }

        // METHODS DEFINED IN GameGUI.cpp

        /**
         * Cascades this call to the current ScreenGUI. That object will add the
         * appropriate items to the render list. This method should be called
         * once per frame to make sure the current GUI is always rendered.
         */
        void addRenderItemsToRenderList(game::Game*) const;

        /**
         * Adds a constructed ScreenGUI to the game's gui.
         */
        void addScreenGUI(const GameState gameState, ScreenGUI* screenToAdd) {
          screens[static_cast<uint8_t>(gameState)] = screenToAdd;
        }

        /**
         * This method should be called once per frame. It checks with the
         * current screen to see if a button event needs to be fired, and fires
         * it if necessary.
         */
        bool checkCurrentScreenForAction(game::Game* game);


        /**
         * Tells all of the screens in this gui the class that will handle
         * button click events.
         */
        void registerButtonEventHandler(ButtonEventHandler* eventHandler);

        /**
         * Sends mousePoint, the current cursor location, to the current GUI
         * screen such that all of its button states can be updated. This method
         * should be called once per frame after mouse position has been updated
         * but before we check for fired events.
         */
        void updateGUIState(long mouseX, long mouseY, GameState gameState);
      private:
        // THIS MAP STORES ALL OF OUR SCREENS FOR VARIOUS GAME STATES
        std::unordered_map<uint8_t, ScreenGUI*> screens;

        // THIS IS OUR CURSOR, WITH WHICH WE CAN LOAD
        // WHATEVER IMAGES WE WANT
        Cursor* customCursor;

        // THE VIEWPORT IS THE WINDOW ONTO THE GAME THAT WE LOOK THROUGH
        Viewport viewport;
      };
    }
  }
}
