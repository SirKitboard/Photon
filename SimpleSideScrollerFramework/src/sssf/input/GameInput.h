/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameInput.h
    */

#pragma once

#include <array>
#include <cstdint>

#include "sssf/input/KeyEventHandler.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace input {

      /// <summary>
      /// Manages all game input. This is currently done only via a keyboard and
      /// mouse.
      /// </summary>
      class GameInput {
      protected:
        /**
        KeyState structure is used to represent the current
        state of a keyboard key. It would be updated each
        frame and helps us know which keys are pressed, and
        whether this is the first frame it has been pressed
        or if it has been held down for consecutive frames,
        i.e., it has been held down.
        **/
        struct InputState {
          bool isPressed : 1;
          bool isFirstPress : 1;
          bool wasHeldDown : 1;
          // These are bit fields; these will take up one bit each (with 5 to spare)
          // bools normally take up a full byte, as they're just chars in disguise
        };

        std::array<InputState, 256> inputState;
        // stores the state for all keys

        // THE KEY HANDLER RESPONDS TO KEY PRESSES
        KeyEventHandler* keyHandler;

      public:
        // INLINED METHODS

        // CHECKS TO SEE IF A KEY IS CURRENTLY PRESSED
        bool isKeyDown(const int key) const {
          return inputState[key].isPressed;
        }

        // CHECKS TO SEE IF A KEY WAS RECENTLY PRESSED (FIRED ONCE PER PRESS)
        bool isKeyDownForFirstTime(const int key) const {
          return inputState[key].isFirstPress;
        }

        // REGISTERS A KeyHandler
        void registerKeyHandler(KeyEventHandler* initKeyHandler) {
          keyHandler = initKeyHandler;
        }

        // ASKS THE keyHandler TO RESPOND
        void respondToKeyboardInput(game::Game* game) {
          keyHandler->handleKeyEvents(game);
        }

        // METHODS DEFINED IN GameInput.cpp
        GameInput();
        virtual ~GameInput();
        void initInputState();
        virtual void updateInputState() = 0;
        virtual void processInput(game::Game* game) = 0;
        virtual void respondToMouseInput(game::Game* game) = 0;
        virtual void shutdown() = 0;
      };
    }
  }
}
