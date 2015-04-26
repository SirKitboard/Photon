/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameText.cpp

    See GameText.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/text/GameText.h"

#include "sssf/game/Game.h"
#include "sssf/graphics/GameGraphics.h"

namespace cse380 {
  namespace sssf {
    namespace text {
      using std::string;

      void GameText::addText(wstring* textToAdd,
        int initX,
        int initY,
        int initWidth,
        int initHeight) {
        TextToDraw text;
        text.setText(textToAdd);
        text.x = initX;
        text.y = initY;
        text.width = initWidth;
        text.height = initHeight;
        textToDraw.push_back(text);
      }

      /*
          changeTextOnly - Once text is added, if we want to subsequently
          change it, we may use this method.
          */
      void GameText::changeTextOnly(wstring* textToSet, int index) {
        TextToDraw& textToChange = textToDraw.at(index);
        textToChange.setText(textToSet);
      }

      /*
          initDebugFile - This method sets up the writer to allow us to write
          to the debug output text file.
          */
      void GameText::initDebugFile(const string& debugFileName) {
        writer.initFile(debugFileName);
      }

      /*
          moveText - This method allows us to move the location
          of where we draw our text.
          */
      void GameText::moveText(int index, int xMove, int yMove) {
        textToDraw.at(index).x += xMove;
        textToDraw.at(index).y += yMove;
      }

      /*
          writeOutput - This method writes text to the debugging log file.
          */
      void GameText::writeDebugOutput(const string& output) {
        writer.writeText(output);
      }
    }
  }
}
