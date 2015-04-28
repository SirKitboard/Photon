/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Viewport.h
    */
#pragma once

namespace cse380 {
  namespace sssf {
    namespace gui {

      /**
      Stores information about the viewport to help with rendering. The viewport
      may move around the game world by calling moveViewport, which changes
      viewportX and viewportY. It also has information about the size of the
      viewport. Note that the viewport width and height refer to the full
      rendering area of the game, including any GUI regions. To compute the size
      of the game rendering area, which might not include GUI toolbars, for
      example, one may subtract the viewport offsets from the width and height
      values.
      */
      class Viewport {
      public:
        // INLINED ACCESSOR METHODS
        int getScrollSpeedX() const { return scrollSpeedX; }
        int getScrollSpeedY() const { return scrollSpeedY; }
        int getViewportX() const { return viewportX; }
        int getViewportY() const { return viewportY; }
        int getViewportWidth() const { return viewportWidth; }
        int getViewportHeight() const { return viewportHeight; }
        int getViewportOffsetX() const { return viewportOffsetX; }
        int getViewportOffsetY() const { return viewportOffsetY; }

        // INLINED MUTATOR METHODS
        void setScrollSpeedX(const int x) { scrollSpeedX = x; }
        void setScrollSpeedY(const int y) { scrollSpeedY = y; }

        void setScrollSpeed(const int vx, const int vy) {
          scrollSpeedX = vx;
          scrollSpeedY = vy;
        }

        void setViewportX(const int x) { viewportX = x; }
        void setViewportY(const int y) { viewportY = y; }

        void setViewportPosition(const int x, const int y) {
          viewportX = x;
          viewportY = y;
        }

        void setViewportWidth(const int w) { viewportWidth = w; }
        void setViewportHeight(const int h) { viewportHeight = h; }

        void setViewportSize(const int w, const int h) {
          viewportWidth = w;
          viewportHeight = h;
        }

        void setViewportOffsetX(const int x) { viewportOffsetX = x; }

        void setViewportOffsetY(const int y) {
          viewportOffsetY = y;
          normalOffsetY = viewportOffsetY;
        }

        void setViewportOffset(const int x, const int y) {
          viewportOffsetX = x;
          viewportOffsetY = y;
          normalOffsetY = y;
        }

		void reset() {
			viewportX = 0;
			viewportY = 0;
        }

        void setToggleOffsetY(const int y) { toggleOffsetY = y; }

        /**
         * This default constructor simply sets all integer values to 0, which
         * means they have not yet been assigned valid values.
         */
        Viewport();

        /**
         * This class has no pointer instance variables so this destructor has
         * nothing to clean up.
         */
        ~Viewport() {}

        // METHODS DEFINED IN Viewport.cpp

        /// <summary>
        /// Returns true if a  rectangular region (like a sprite) overlaps the
        /// viewport. Use this to test to see if a game object is on-screen, and
        /// thereby elegible for the render list.
        /// </summary>
        bool areWorldCoordinatesInViewport(
          float x,
          float y,
          int width,
          int height
          ) const;

        bool areViewportCoordinatesInViewport(
          int x,
          int y,
          int width,
          int height
          ) const;

        bool areScreenCoordinatesInViewport(int x, int y) const;

        /**
         * Moves the viewport, which is the region of the world seen by the
         * player. This would typically be tied to player movement or some
         * scripted sequence. This method prevents moving the viewport off the
         * edge of the world by clamping at the world edges.
         */
        void moveViewport(int incX, int incY, int worldWidth, int worldHeight);

        void toggleDebugView();
      private:
        int scrollSpeedX;
        int scrollSpeedY;
        int viewportX;
        int viewportY;
        int viewportWidth;
        int viewportHeight;
        int viewportOffsetX;
        int viewportOffsetY;
        bool viewportToggled;
        int toggleOffsetY;
        int normalOffsetY;
      };
    }
  }
}
