/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutKeyEventHandler.h

    This is a custom key event handler. Each game can choose
    to use different keyboard keys and respond differently to when
    they are pressed. This class will handle key presses for
    this particular game following the KeyEventHandler pattern.
    */

#pragma once
#include "sssf/input/KeyEventHandler.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
  }
  namespace bugginout {
    class BugginOutKeyEventHandler : public sssf::input::KeyEventHandler {
    public:
      BugginOutKeyEventHandler() {}
      ~BugginOutKeyEventHandler() {}
      void handleKeyEvents(sssf::game::Game* game) override;
    };
  }
}