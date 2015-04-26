/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutTextGenerator.h

    This class is a custom text generator for the EmptyGame
    application. Basically, by defining the initText and updateText
    methods, we can have it display whatever text we like.
    */

#pragma once

#include <array>
#include <string>

#include "sssf/text/TextGenerator.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
  }
  namespace bugginout {
    using std::array;
    using std::wstring;
    using sssf::game::Game;

    class BugginOutTextGenerator : public sssf::text::TextGenerator {
    public:
      // NOTHING TO INITIALIZE OR DESTROY
      BugginOutTextGenerator() {}
      ~BugginOutTextGenerator() {}

      // DEFINED IN BugginOutTextGenerator.cpp
      void initText(Game* game) override;
      void updateText(Game* game) override;
    private:
      // THIS IS THE TEXT THIS OBJECT WILL UPDATE EACH FRAME
      array<wstring, 3> textToGenerate;
      void appendMouseCoords(Game* game, int index);
      void appendTimeInfo(Game* game, int index);
    };
  }
}