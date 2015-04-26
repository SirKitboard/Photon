/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    WindowsInput.h

    This class manages all game input. This is currently done only
    via a keyboard and mouse. Windows method are used to get input
    information each frame.
    */

#pragma once

#include <windows.h>

#include "sssf/input/KeyEventHandler.h"
#include "sssf/input/GameInput.h"

namespace cse380 {
  namespace sssf {
    // FORWARD DECLARATIONS OF CLASSES USED BY WindowsInput.h
    namespace game { class Game; }
    namespace gui { class Cursor; }
    namespace win32 {
      using game::Game;

      class WindowsInput : public input::GameInput {
      private:
        // THIS IS THE LOCATION OF THE CURSOR
        POINT mousePoint;

      public:
        // INLINED METHODS

        // GETS THE MOUSE LOCATION
        const POINT&  getMousePoint() const { return mousePoint; }

        // METHODS DEFINED IN GameInput.cpp
        WindowsInput();
        virtual ~WindowsInput();
        void initInputState();
        void updateInputState();
        void updateCursorPosition(Game* game, const WINDOWINFO& wi, gui::Cursor* cursor);
        void processInput(Game* game);
        void respondToMouseInput(Game* game);
        void shutdown();
      };
    }
  }
}
