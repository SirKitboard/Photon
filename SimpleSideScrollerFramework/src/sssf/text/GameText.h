/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameText.h

    This class allows for easy rendering of text
    to the game screen.
    */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "sssf/text/TextFileWriter.h"

namespace cse380 {
  namespace sssf {
    namespace text {
      using std::wstring;
      using std::vector;
      class TextGenerator;

      const int MAX_TEXT_OBJECTS = 10;

      class TextToDraw {
      public:
        int maxChars;
        int numChars;
        int x;
        int y;
        int width;
        int height;

        void setText(wstring* textToSet) { text = textToSet; }
        const wstring* getText() const { return text; }

      private:
        wstring* text;
      };

      class GameText {
      public:
        // METHODS DEFINED IN GameText.cpp
        GameText() {}
        ~GameText() {}

        // INLINED ACCESSOR METHODS
        size_t getNumTextObjectsToDraw() const { return textToDraw.size(); }
        TextGenerator* getTextGenerator() { return textGenerator; }
        const TextGenerator* getTextGenerator() const { return textGenerator; }
        TextToDraw& getTextToDrawAtIndex(int index) { return textToDraw.at(index); }
        const TextToDraw& getTextToDrawAtIndex(int index) const { return textToDraw.at(index); }

        // INLINED MUTATOR METHOD
        void setTextGenerator(TextGenerator* initTextGenerator) {
          textGenerator = initTextGenerator;
        }

        // Adds text for rendering. Text only needs to be added once.
        void addText(wstring*, int, int, int, int);
        void changeTextOnly(wstring* textToAdd, int index);
        void initDebugFile(const std::string& debugFileName);
        void moveText(int index, int xMove, int yMove);
        void writeDebugOutput(const std::string& output);
      private:
        int textCounter;
        vector<TextToDraw> textToDraw;
        TextGenerator* textGenerator;

        // USED TO PRINT DEBUGGING OUTPUT
        TextFileWriter writer;
      };
    }
  }
}
