/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    SparseLayer.h

    A SparseLayer manages a sparse background, which is
    number of images that are placed in a world with
    space in between them. They are not packed into a grid
    and can be in any random arrangement.
    */
#pragma once

#include <cstdint>
#include <vector>

#include "sssf/gsm/world/WorldLayer.h"
#include "sssf/gui/OverlayImage.h"

// THIS CLASS IS-A WorldLayer

namespace cse380 {
  namespace sssf {

    namespace graphics { class RenderList; }
    namespace gui {
      struct OverlayImage;
      class Viewport;
    }

    namespace gsm {

      namespace physics {
        class CollidableObject;
        class Physics;
      }

      namespace world {
        using std::vector;
        using gui::OverlayImage;
        using namespace sssf::gsm::physics;

        class SparseLayer : public WorldLayer {
        public:
          SparseLayer() {}
          ~SparseLayer() {}

          // INLINED ACCESSOR METHOD
          OverlayImage& getTile(const size_t index) {
            return sparseTiles.at(index);
          }

          const OverlayImage& getTile(const size_t index) const {
            return sparseTiles.at(index);
          }

          // INLINED MUTATOR METHOD
          void addTile(const OverlayImage& initTile) {
            sparseTiles.push_back(initTile);
          }
          void setTile(const OverlayImage& initTile, const size_t index) {
            sparseTiles.at(index) = initTile;
          }


		  int getColumns() const override { return 0; }
		  int getRows() const override { return 0; }
          // METHODS DEFINED IN SparseLayer.cpp
          void addRenderItemsToRenderList(graphics::RenderList&, graphics::RenderList&,const gui::Viewport&) const override;
          void findTileCollisionsOverlappingSweptShape(Physics&, CollidableObject*);
		  bool overlapsCollidableTile(AABB aabb);
		  bool isInsideCollidableTile(int centerX, int centerY);
		  bool willSpriteCollideOnTile(Physics *physics,
			  CollidableObject *dynamicObject);
        private:
          // SPARSE TILES NEED NOT BE IN ANY ORDER OR PATTERNED ARRANGEMENT
          vector<OverlayImage> sparseTiles;
        };
      }
    }
  }
}
