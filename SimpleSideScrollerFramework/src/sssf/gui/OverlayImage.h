/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Overlayimage.h
    */

#pragma once

#include <cstdint>

namespace cse380 {
  namespace sssf {
    namespace gui {

      /// <summary>
      /// A simple structure for storing information about some static image in
      /// a game. It might be a background image in a GUI or for static objects
      /// in a game world.
      /// </summary>
      struct OverlayImage {
        // INDEX OF IMAGE IN TEXTURE MANAGER
        size_t imageID;

        // X LOCATION
        int x;

        // Y LOCATION
        int y;

        // Z LAYER
        int16_t z;

        // TEXTURE WIDTH TO USE
        uint16_t width;

        // TEXTURE HEIGHT TO USE
        uint16_t height;

        // TRANSPARENCY
        uint8_t alpha;
      };
    }
  }
}
