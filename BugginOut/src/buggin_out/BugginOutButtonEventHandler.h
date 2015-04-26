/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutButtonEventHandler.h

    This is a custom button event handler. Each game can choose
    to use different buttons and respond differently to when
    buttons are pressed. This class will handle button presses for
    the BugginOutGame following the ButtonEventHandler pattern.
    */

#pragma once

#include <string>

#include "sssf/input/ButtonEventHandler.h"

namespace cse380 {
  namespace sssf {
    namespace game {
      class Game;
    }
  }
  namespace bugginout {
    using std::wstring;

    class BugginOutButtonEventHandler : public sssf::input::ButtonEventHandler {
    public:
      // AS-IS, THIS CLASS HAS NO DATA, SO THERE IS NOTHING
      // TO INITIALIZE OR DESTROY
      BugginOutButtonEventHandler() {}
      ~BugginOutButtonEventHandler() {}

      // DEFINED INSIDE BugginOutButtonEventHandler.cpp
      void handleButtonEvents(sssf::game::Game* game, const wstring& command) override;
    };
  }
}
