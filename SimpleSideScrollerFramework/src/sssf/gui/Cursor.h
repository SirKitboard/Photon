/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Cursor.h
    */

#pragma once

#include <cstdint>
#include <vector>

// Forward declarations of classes used by Cursor

namespace cse380 {
  namespace sssf {
    namespace graphics { class RenderList; }

    namespace gui {
      using std::vector;
      using graphics::RenderList;

      /// <summary>
      /// Stores information for a custom game cursor. It can store the image
      /// IDs of multiple images, as loaded into a texture manager, and so by
      /// changing the current cursor ID, it can change the cursor to be rendered.
      /// </summary>
      class Cursor {
      public:
        Cursor(const vector<size_t>& initImageIDs,
          const size_t initActiveCursorID,
          const int initX,
          const int initY,
          const int16_t initZ,
          const uint8_t initAlpha,
          const uint16_t initWidth,
          const uint16_t initHeight
          );
        ~Cursor() {}

        // INLINED ACCESSOR METHODS
        size_t getImageID(const int index) const { return imageIDs.at(index); }
        size_t getActiveCursorID() const { return activeCursorID; }
        int getX() const { return x; }
        int getY() const { return y; }
        int16_t getZ() const { return z; }
        uint8_t getAlpha() const { return alpha; }
        int getWidth() const { return width; }
        int getHeight() const { return height; }
        size_t getNumCursorIDs() const { return imageIDs.size(); }

        // INLINED MUTATOR METHODS
        void setActiveCursorID(const size_t id) { activeCursorID = id; }
        void setX(const int initX) { x = initX; }
        void setY(const int initY) { y = initY; }

        void setPosition(const int x, const int y) {
          this->x = x;
          this->y = y;
        }

        void setZ(const int16_t initZ) { z = initZ; }
        void setAlpha(const uint8_t initAlpha) { alpha = initAlpha; }

        // METHODS DEFINED IN Cursor.cpp
        /**
         * Called once per frame, this method makes a RenderItem representing
         * the current cursor and adds it to the render list. The result being
         * that the cursor will be rendered according to its current state.
         * The cursor should be rendered last, after the game world and the rest
         * of the GUI.
         */
        void addRenderItemToRenderList(RenderList& renderList) const;
      private:
        // THESE ARE THE INDICES IN THE GUI TEXTURE MANAGER OF
        // ALL OF THE IMAGES WE CAN USE FOR THIS CURSOR
        vector<size_t> imageIDs;

        // THIS IS THE INDEX OF THE IMAGE WE ARE CURRENTLY USING
        size_t activeCursorID;

        // OTHER VARIBLES FOR RENDERING
        int x;
        int y;
        int16_t z;
        uint16_t width;
        uint16_t height;
        uint8_t alpha;
      };
    }
  }
}
