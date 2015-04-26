/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    RenderList.h
    */

#pragma once

#include <cstdint>
#include <vector>

namespace cse380 {
  namespace sssf {
    namespace graphics {
      /// <summary>
      /// Represents a texture to be rendered each frame. Each frame we should
      /// build two lists of these. One for the game world (if we're in-game),
      /// and one for the GUI. Note that this struct is sortable by Z value.
      /// </summary>
      struct RenderItem {
        size_t id;
        int x;
        int y;
        int16_t z;
        uint16_t width;
        uint16_t height;
        uint8_t a;
		float roation;

        // WE HAVE DEFINED THIS COMPARISON SUCH THAT OUR LIST CAN BE
        // SORTED USING THE sort METHOD FROM THE STL'S list CLASS.
        // THE () OPERATOR WILL ALLOW US TO SORT RenderItems IN THE
        // LIST BY THEIR Z VALUES
        bool operator()(const RenderItem& a, const RenderItem& b) const {
          return (a.z < b.z);
        }

        bool operator<(const RenderItem& other) const {
          return this->z < other.z;
        }
      };

      using std::vector;

      /// <summary>
      /// <para>
      /// Stores the data for rendering a list of textures each frame. While
      /// the game is running, each frame lists of RenderItems may be built for
      /// all visible textures. That list is then traversed, used for rendering,
      /// then cleared.
      /// </para>
      /// <para>
      /// RenderItems should be placed into the list in the order with which they
      /// should be rendered, background items at the start of the list and
      /// foreground items at the end of the list. Rendering should be done in
      /// the order of game world and then game gui.
      /// </para>
      /// <para>
      /// RenderItems have z values, so optionally we could sort the items in
      /// the list before rendering in order to ensure correct back to front
      /// rendering.
      /// </para>
      /// <para>
      /// Each RenderItem also has an alpha value such that each item can have
      /// its own custom transparency.
      /// </para>
      /// <para>
      /// The game world's background should be rendered first, then objects and
      /// characters. After that, the GUI should be rendered in order of the
      /// overlay images, the buttons, then the cursor.
      /// </para>
      /// </summary>
      class RenderList {
      private:
        // We'll allocate space in this vector once, then never add to it
        // Remember that std::arrays must have their size defined at compile time,
        // so we can't use those
        vector<RenderItem> renderList;
        size_t size;

      public:
        // METHODS DEFINED IN RenderList.cpp

        // Constructs the list but does not fill it. Filling the list should be
        // done via addRenderItem.
        RenderList(const size_t initMaxItems);
        ~RenderList() {}

        /**
         * Adds a constructed RenderItem to the render list. The result should
         * be that that item would be rendered in the next rendering. Note that
         * once rendered, the list should be cleared because next frame we'll
         * have different stuff to render.
         */
        void addRenderItem(const size_t id,
          const int x,
          const int y,
          const int16_t z,
          const uint8_t a,
          const uint16_t width,
          const uint16_t height,
		  float rotationInRadians);

        /**
         * Clears out the render list, deleting all the RenderItems inside. This
         * should be called after each time a RenderList is used for rendering.
         * If it is not called, the render list will get huge, and old stuff
         * that has since been updated or moved, or are out of view will still
         * be rendered.
         */
        void clear();

        vector<RenderItem>::iterator begin() {
          return renderList.begin();
        }

        vector<RenderItem>::iterator end() {
          return renderList.begin() + size;
        }

        vector<RenderItem>::const_iterator cbegin() const {
          return renderList.cbegin();
        }

        vector<RenderItem>::const_iterator cend() const {
          return renderList.cbegin() + size;
        }

        size_t getMaxItems() const {
          return this->renderList.capacity();
        }
      };
    }
  }
}
