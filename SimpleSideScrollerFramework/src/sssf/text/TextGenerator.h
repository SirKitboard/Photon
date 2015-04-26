/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    TextGenerator.h
    */

#pragma once

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }
    namespace text {

      /// <summary>
      /// Provides a framework for generating text each frame. Each game may
      /// generate different text, so such a would define initText and updateText
      /// based on the game's needs.
      /// </summary>
      class TextGenerator {
      public:
        TextGenerator() {}
        virtual ~TextGenerator() {}

        // No copying allowed
        TextGenerator(const TextGenerator&) = delete;
        TextGenerator& operator=(const TextGenerator&) = delete;

        virtual void initText(game::Game* game) = 0;
        virtual void updateText(game::Game* game) = 0;
      };
    }
  }
}
