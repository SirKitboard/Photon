/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    KeyEventHandler.h

   */

#pragma once

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace input {

      /// <summary>
      /// <para>
      /// Provides a framework for handling keyboard input. The assumption is that
      /// each game will have different key mappings and different responses to
      /// key presses; such responses must be implemented based on the needs of
      /// the game.
      /// </para>
      /// <para>
      /// Child classes would put their custom responses inside the handleKeyEvents
      /// method, which GameInput calls every frame. KeyEventHandlers can access
      /// any game state they need.
      /// </para>
      /// </summary>
      class KeyEventHandler {
      protected:
        // INLINED DESTRUCTOR
        virtual ~KeyEventHandler() {}

      public:
        // METHOD DEFINED IN GAME-SPECIFIC CHILD CLASSES
        virtual void handleKeyEvents(game::Game* game) = 0;

        void operator()(game::Game* game) {
          this->handleKeyEvents(game);
        }
      };
    }
  }
}
