/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    RenderList.cpp

    See RenderList.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/graphics/RenderList.h"

namespace cse380 {
  namespace sssf {
    namespace graphics {
      RenderList::RenderList(const size_t initMaxItems) :
        renderList(initMaxItems),
        size(0) {}

      void RenderList::addRenderItem(const size_t id,
        const int x,
        const int y,
        const int16_t z,
        const uint8_t a,
        const uint16_t width,
        const uint16_t height,
		float roations) {
        if (size < renderList.capacity()) {
          renderList[size].id = id;
          renderList[size].x = x;
          renderList[size].y = y;
          renderList[size].z = z;
          renderList[size].a = a;
          renderList[size].width = width;
          renderList[size].height = height;
		  renderList[size].roation = roations;
          size++;
        }
      }

      void RenderList::clear() {
        size = 0;
      }
    }
  }
}
