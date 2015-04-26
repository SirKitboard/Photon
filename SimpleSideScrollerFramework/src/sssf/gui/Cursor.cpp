/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Cursor.cpp

    See Cursor.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gui/Cursor.h"

#include "sssf/graphics/RenderList.h"

namespace cse380 {
  namespace sssf {
    namespace gui {

      Cursor::Cursor(
        const vector<size_t>& initImageIDs,
        const size_t initActiveCursorID,
        const int initX,
        const int initY,
        const int16_t initZ,
        const uint8_t initAlpha,
        const uint16_t initWidth,
        const uint16_t initHeight
        ) :
        imageIDs(initImageIDs),
        activeCursorID(initActiveCursorID),
        x(initX),
        y(initY),
        z(initZ),
        alpha(initAlpha),
        width(initWidth),
        height(initHeight) {}

      void Cursor::addRenderItemToRenderList(RenderList& renderList) const {
        renderList.addRenderItem(activeCursorID,
          x,
          y,
          z,
          alpha,
          width,
          height,
		  0);
      }
    }
  }
}
