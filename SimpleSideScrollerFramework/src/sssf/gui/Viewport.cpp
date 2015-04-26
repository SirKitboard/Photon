/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Viewport.cpp

    See Viewport.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gui/Viewport.h"

#include "sssf/util/Util.h"

namespace cse380 {
  namespace sssf {
    namespace gui {
      Viewport::Viewport() {
        // NORMAL VIEW TO START
        viewportToggled = false;

        // DEFAULT SCROLL SPEED
        scrollSpeedX = 10;
        scrollSpeedY = 10;

        // NO DEFAULTS FOR THESE
        viewportX = 0;
        viewportY = 0;
        viewportWidth = 0;
        viewportHeight = 0;
        viewportOffsetX = 0;
        viewportOffsetY = 0;
      }

      bool Viewport::areViewportCoordinatesInViewport(int x,
        int y,
        int width,
        int height)  const {
        // IS IT OFF-SCREEN TO THE LEFT OF THE VIEWPORT?
        if ((x + width) < 0)
          return false;

        // IS IT OFF-SCREEN ABOVE THE VIEWPORT?
        else if ((y + height) < 0)
          return false;

        // IS IT OFF-SCREEN TO THE RIGHT OF THE VIEWPORT?
        else if (x >= viewportWidth)
          return false;

        // IS IT OFF-SCREEN BELOW THE VIEWPORT?
        else if (y >= viewportHeight)
          return false;

        // IT MUST BE AT LEAST PARTIALLY IN THE VIEWPORT
        else
          return true;
      }

      bool Viewport::areWorldCoordinatesInViewport(
        float x,
        float y,
        int width,
        int height)  const {
        // IS IT OFF-SCREEN TO THE LEFT OF THE VIEWPORT?
        if ((x + width) < viewportX)
          return false;

        // IS IT OFF-SCREEN ABOVE THE VIEWPORT?
        else if ((y + height) < viewportY)
          return false;

        // IS IT OFF-SCREEN TO THE RIGHT OF THE VIEWPORT?
        else if (x >= (viewportX + viewportWidth))
          return false;

        // IS IT OFF-SCREEN BELOW THE VIEWPORT?
        else if (y >= (viewportY + viewportHeight))
          return false;

        // IT MUST BE AT LEAST PARTIALLY IN THE VIEWPORT
        else
          return true;
      }

      void Viewport::moveViewport(int incX,
        int incY,
        int worldWidth,
        int worldHeight) {
        using util::clamp;

        // MOVE THE VIEWPORT IN X AXIS ACCORDING TO PROVIDED INCREMENT.
        viewportX = clamp(viewportX + incX, 0, worldWidth - viewportWidth - 1);

        // MOVE THE VIEWPORT IN Y AXIS ACCORDING TO PROVIDED INCREMENT.
        viewportY = clamp(viewportY + incY, 0, worldHeight - viewportHeight - 1);
      }


      bool Viewport::areScreenCoordinatesInViewport(int x, int y) const {
        if (x < viewportOffsetX) return false;
        if (y < viewportOffsetY) return false;
        if (x >(viewportOffsetX + viewportWidth)) return false;
        if (y >(viewportOffsetY + viewportHeight)) return false;
        return true;
      }

      void Viewport::toggleDebugView() {
        if (viewportToggled) {
          viewportOffsetY = normalOffsetY;
        }
        else {
          viewportOffsetY = toggleOffsetY;
        }
        viewportToggled = !viewportToggled;
      }
    }
  }
}
