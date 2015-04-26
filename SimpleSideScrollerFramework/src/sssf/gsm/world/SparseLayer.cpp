/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    SparseLayer.cpp

    See SparseLayer.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/world/SparseLayer.h"

#include "sssf/graphics/RenderList.h"
#include "sssf/gsm/physics/CollidableObject.h"
#include "sssf/gsm/physics/Physics.h"
#include "sssf/gui/Viewport.h"
#include "sssf/gui/OverlayImage.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace world {

        using graphics::RenderList;
        using gui::Viewport;
        using physics::CollidableObject;
        using physics::Physics;

        /*
            addRenderItemsToRenderList - This method is called once per
            frame for each TiledLayer in a game world while the game is
            being played. It determines which Tiles in the layer are in
            the viewport, and creates a RenderItem for each one. It then
            adds each one to the provided render list.
            */
        void SparseLayer::addRenderItemsToRenderList(RenderList& renderList,
          const Viewport& viewport) const {
          // WE'LL USE THIS IN OUR CALCULATIONS
          float viewportX = viewport.getViewportX();
          float viewportY = viewport.getViewportY();

          // TILES THAT WE PUT IN THE RENDER LIST ARE GOING TO BE
          // RENDERED, SO WE ONLY WANT TO PUT TILES IN THE LIST
          // THAT OVERLAP THE CURRENT VIEWPORT. TO ENSURE PARALLAX
          // SCROLLING, SUBTRACT THE CORRECTED parallaxViewportX
          // & parallaxViewportY FROM THE TILE X & Y VALUES TO
          // SCALE SCROLLING.
          int adjustedX, adjustedY;

          // GO THROUGH ALL THE TILES, STARTING WITH THE FIRST ONE
          for (const OverlayImage& tile : sparseTiles) {
            // GET THE CURRENT TILE
            adjustedX = tile.x - viewportX;
            adjustedY = tile.y - viewportY;

            // IS THIS TILE VISIBLE?
            if (viewport.areWorldCoordinatesInViewport(
              tile.x,
              tile.y,
              tile.width,
              tile.height)) {
              // IT'S VISIBLE, SO RENDER IT
              renderList.addRenderItem(tile.imageID,
                adjustedX,
                adjustedY,
                tile.z,
                tile.alpha,
                tile.width,
                tile.height,
				0);
            }
          }
        }

        void SparseLayer::findTileCollisionsOverlappingSweptShape(Physics& physics,
          CollidableObject* dynamicObject) {
          // CURRENTLY DOESN'T DO ANYTHING
        }
      }
    }
  }
}
