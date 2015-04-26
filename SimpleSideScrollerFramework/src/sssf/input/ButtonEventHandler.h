/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    ButtonEventHandler.h
    */
#pragma once

#include <string>

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }

    namespace input {
      using std::wstring;

      /// <summary>
      /// <para>
      /// Provides a framework for button event handlers. Each Button in a
      /// GameGUI has a ButtonEventHandler, but what that handler does is up to
      /// the individual game application.
      /// </para>
      /// <para>
      /// Button responses should be defined inside an implementation's
      /// handleButtonEvents method. Since that method is passed the Game object,
      /// it has access to everything and can quite literally do anything it wants
      /// to the game.
      /// </para>
      /// <para>
      /// NOTE: Instead of making one big-ass ButtonEventHandler for your own
      /// game, you should probably make one for each unique type of Button (e.g.
      /// a different handler for a Quit button, a different handler for in-game
      /// GUI like dialogue, etc.).
      /// </para>
      /// </summary>
      class ButtonEventHandler {
      protected:
        // INLINED CONSTRUCTOR
        ButtonEventHandler() {}

        // INLINED DESTRUCTOR
        virtual ~ButtonEventHandler() {}

      public:
        // METHOD DEFINED IN GAME-SPECIFIC CHILD CLASSES
        virtual void handleButtonEvents(game::Game* game, const wstring& command) = 0;

        void operator()(game::Game* game, const wstring& command) {
          this->handleButtonEvents(game, command);
        }
      };
    }
  }
}
