/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Button.cpp

    See Button.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gui/Button.h"

#include "sssf/game/Game.h"
#include "sssf/input/ButtonEventHandler.h"

namespace cse380 {
  namespace sssf {
    namespace gui {
      Button::Button(
        const size_t initNormalTextureID,
        const size_t initMouseOverTextureID,
        const int initX,
        const int initY,
        const int16_t initZ,
        const uint8_t initAlpha,
        const uint16_t initWidth,
        const uint16_t initHeight,
        const bool initMouseOver,
        const wstring& initCommand,
        input::ButtonEventHandler* handler) :
        normalTextureID(initNormalTextureID),
        mouseOverTextureID(initMouseOverTextureID),
        x(initX),
        y(initY),
        z(initZ),
        width(initWidth),
        height(initHeight),
        alpha(initAlpha),
        mouseOver(initMouseOver),
        command(initCommand),
        eventHandler(handler) {}

      void Button::fireEvent(game::Game* game) {
        if (eventHandler)
          eventHandler->handleButtonEvents(game, command);
      }

      bool Button::updateMouseOver(long mouseX, long mouseY) {
        // IS THE CURSOR OVER THIS BUTTON?
        if ((mouseX >= x) &&
          (mouseX <= (x + width)) &&
          (mouseY >= y) &&
          (mouseY <= (y + height))) {
          // SETTING THIS WILL ENSURE WE USE THE PROPER IMAGE FOR
          // RENDERING, IT WILL ALSO ENSURE AN EVENT IS FIRED IF
          // THE MOUSE BUTTON IS CLICKED
          mouseOver = true;
        }
        else {
          // THIS WILL ENSURE THE NORMAL IMAGE IS RENDERED
          mouseOver = false;
        }

        return mouseOver;
      }
    }
  }
}
