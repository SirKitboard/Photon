/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BugginOutTextGenerator.cpp

    See BugginOutTextGenerator.h for a class description.
    */

#include "buggin_out_VS/stdafx.h"
#include "buggin_out/BugginOutTextGenerator.h"

#include "buggin_out/BugginOut.h"
#include "sssf/game/Game.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/text/GameText.h"
#include "sssf/timer/GameTimer.h"

#ifdef CROSS
#include "sssf/platforms/Cross/SFMLInput.h"
#include "sssf/platforms/Cross/StandardTimer.h"
#else
#include "sssf/platforms/Windows/WindowsInput.h"
#include "sssf/platforms/Windows/WindowsTimer.h"
#endif // CROSS

namespace cse380 {
  namespace bugginout {
    using std::wstringstream;

#ifdef CROSS
    using namespace sssf::sfml;
    using namespace sssf::cpp;
#else
    using namespace sssf::win32;
#endif
    using namespace sssf::text;


    static const wstring    W_MOUSE_COORDS_TEXT = L"Mouse: (";
    static const int        W_TEXT_X = 200;
    static const int        W_TEXT_Y = 10;
    static const int TEXT_X_DIFF = 200;

    /*
        initText - Provides an example of how to render text to our screen every frame,
        allowing for the text to continually change. This method adds a text object
        to the text manager (GameText), which allows it to be updated each frame. This
        method should be called only once for each object.
        */
    void BugginOutTextGenerator::initText(Game* game) {
      // FIRST UPDATE THE TEXT THIS GENERATOR IS USING
      appendMouseCoords(game, 0);
      appendTimeInfo(game, 1);

      int width = game->getGraphics()->getScreenWidth();
      int height = game->getGraphics()->getScreenHeight();

      // AND THEN HAND IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
      GameText& text = game->getText();

      for (int i = 0; i < this->textToGenerate.size(); ++i) {
        text.addText(&textToGenerate[i], W_TEXT_X + TEXT_X_DIFF * i, W_TEXT_Y, width, height);
      }
    }

    /*
        updateText - updates the text. Note that the text must already have been added
        to the text manager via the initText method. Also not that there should be a
        separate TextGenerator for each line of text we wish to render.
        */
    void BugginOutTextGenerator::updateText(Game* game) {
      // THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
      // SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
      // WHAT WE WANT.
      for (wstring& text : textToGenerate) {
        text.clear();
      }
      this->appendMouseCoords(game, 0);
      this->appendTimeInfo(game, 1);
    }

    void BugginOutTextGenerator::appendTimeInfo(Game* game, int index) {
      wstringstream wss;

      sssf::time::GameTimer* timer = game->getTimer();

      wss << "MS/F: " << timer->getLastFrameTime() << '\n';
  
      wss << "Avg. FPS: " << timer->calculateAverageActualFramerate() << '\n';
      wss << "Avg. Sleepless FPS: " << timer->calculateAverageSleeplessFramerate() << '\n';
      wss << "Avg. Sleep: " << timer->calculateAverageSleepTime() << "ms\n";

      textToGenerate[index].append(wss.str());
    }

    void BugginOutTextGenerator::appendMouseCoords(Game* game, int index) {
      wstringstream wss;
#ifdef CROSS
      SFMLInput* input = static_cast<SFMLInput*>(game->getInput());
      const sf::Vector2i& mouse = input->getMousePoint();
#else
      WindowsInput* input = static_cast<WindowsInput*>(game->getInput());
      const POINT& mouse = input->getMousePoint();
#endif

      wss << W_MOUSE_COORDS_TEXT << mouse.x << ", " << mouse.y << ")\n";
      textToGenerate[index].append(wss.str());
    }
  }
}