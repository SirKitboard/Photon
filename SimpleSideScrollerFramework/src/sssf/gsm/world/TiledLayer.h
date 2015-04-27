/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    TiledLayer.h
    */
#pragma once

#include <cassert>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>

#include "sssf/gsm/world/WorldLayer.h"
#include "sssf/gsm/world/Tile.h"
#include "sssf/util/Util.h"
#include "sssf/gsm/world/CollidableTile.h"

//#include "Box2D/" 

// THIS CLASS IS-A WorldLayer

namespace cse380 {
  namespace sssf {
    namespace graphics { class RenderList; }
    namespace gui { class Viewport; }
    namespace gsm {

      namespace physics {
        class AABB;
        class CollidableObject;
        class Physics;
      }

      namespace world {
        using std::round;
        using std::string;
        using std::stoi;
		using std::stof;
        using std::stol;
        using std::stoll;
        using std::stoul;
        using std::stoull;
        using std::stof;
        using std::stod;
        using std::to_string;
        using std::unordered_map;
        using std::vector;
        using std::wstring;

        using namespace physics;

        /// <summary>
        /// Manages a tiled background, which is basically a grid of images that
        /// together create a game background.
        /// </summary>
        class TiledLayer : public WorldLayer {
        public:
          /// <summary>
          /// Initializes the layer by calculating the dimensions of the layer
          /// based on the sizes of the tiles and the numbers of rows/columns.
          /// In addition, it calculates the parallax scroll factor. It does not,
          /// however, fill the data structure with Tiles; use the addTile method
          /// for that.
          /// </summary>
          TiledLayer(
            int initColumns,
            int initRows,
            int initTileWidth,
            int initTileHeight,
            int initZ,
            bool initCollidableTiles,
            int initWorldWidth,
            int initWorldHeight,
            size_t gid
            );

          // Cleans up the Tile grid.
          virtual ~TiledLayer() {}

          // INLINED ACCESSOR METHODS
          int getColumns() const { return columns; }
          int getRows() const { return rows; }
          int getTileHeight() const { return tileHeight; }
          int getTileWidth() const { return tileWidth; }
          int getLayerWidth() const { return layerWidth; }
          int getLayerHeight() const { return layerHeight; }
          int getZ() const { return z; }

          // INLINED MUTATOR METHOD
          void setZ(int initZ) { z = initZ; }


          // No copying
          TiledLayer(const TiledLayer&) = delete;
          TiledLayer& operator=(const TiledLayer&) = delete;

          /// <summary>
          /// If the width of a TiledLayer is different from the world width,
          /// horizontal parallax scrolling will be used. layerWidth is used
          /// to calculate parallaxFactorX; it is simply the width of tiles used
          /// in this layer times the number of rows.
          /// </summary>
          int calculateAndSetLayerWidth() {
            return (this->layerWidth = columns * tileWidth);
          }

          /// <summary>
          /// If the height of a TiledLayer is different from the world height,
          /// vertical parallax scrolling will be used. layerHeight is used to
          /// calculate parallaxFactorY; it is simply the height of tiles used
          /// in this layer times the number of columns.
          /// </summary>
          int calculateAndSetLayerHeight() {
            return (this->layerHeight = rows * tileHeight);
          }

          /// <summary>
          /// Determines what column the visible tiles in the left-most part
          /// of the viewport belong to, thus letting us know which columns of
          /// tiles need to be rendered and which can be ignored. There is no
          /// need to render tiles in columns that are not in the viewport.
          /// </summary>
          int calculateViewportStartColumn(const int vx, const int vw) const {
            // APPLY PARALLAX SCROLLING FACTOR TO VIEWPORT VALUE
            float parallaxX = vx *
              ((static_cast<float>(layerWidth - vw)) / (worldWidth - vw));
            return round(parallaxX / tileWidth);
          }

          /// <summary>
          /// Determines what column the visible tiles in the right-most part
          /// of the viewport belong to, thus letting us know which columns of
          /// tiles need to be rendered and which can be ignored. There is no
          /// need to render tiles in columns that are not in the viewport.
          /// </summary>
          int calculateViewportEndColumn(const int vx, const int vw) const {
            float parallaxX = vx *
              (static_cast<float>(layerWidth - vw) / (worldWidth - vw));
            float parallaxRight = parallaxX + vw;
            return round(parallaxRight / tileWidth);
          }

          /// <summary>
          /// Determines what column the visible tiles in the top-most part
          /// of the viewport belong to, thus letting us know which rows of
          /// tiles need to be rendered and which can be ignored. There is no
          /// need to render tiles in rows that are not in the viewport.
          /// </summary>
          int TiledLayer::calculateViewportStartRow(const int vy, const int vh) const {
            // APPLY PARALLAX SCROLLING FACTOR TO VIEWPORT VALUE
            float parallaxY = vy *
              (static_cast<float>(layerHeight - vh) / (worldHeight - vh));
            return round(parallaxY / tileHeight);
          }

          /// <summary>
          /// Determines what column the visible tiles in the bottom-most part
          /// of the viewport belong to, thus letting us know which rows of
          /// tiles need to be rendered and which can be ignored. There is no
          /// need to render tiles in rows that are not in the viewport.
          /// </summary>
          int calculateViewportEndRow(const int vy, const int vh) const {
            // APPLY PARALLAX SCROLLING FACTOR TO VIEWPORT VALUE
            float parallaxY = vy *
              (static_cast<float>(layerHeight - vh) / (worldHeight - vh));
            float parallaxBottom = vy + vh;
            return round(parallaxBottom / tileHeight);
          }

          void addTile(const size_t id, const Tile& tile) {
            this->tiles[tile.tileID] = tile;
          }

          /// <summary>
          /// Retrieves the Tile in the grid at (row, column). Since the Tiles
          /// are stored in a single dimensional vector, we must convert the
          /// coordinates from 2D to 1D. May return nullptr, which signifies a
          /// lack of a tile in that location (i.e. an empty space).
          /// </summary>
          Tile* getTile(const uint16_t row, const uint16_t column) {
            uint32_t cell = (row * columns) + column;
            size_t id = tileLayout[cell];
            return (cell < tileLayout.size()) ? &tiles[id - firstGID] : nullptr;
          }

          const Tile* getTile(const uint16_t row, const uint16_t column) const {
            uint32_t cell = (row * columns) + column;
            size_t id = tileLayout.at(cell);
            return (cell < tileLayout.size()) ? &(tiles.at(id - firstGID)) : nullptr;
          }

          const Tile& getTile(const size_t index) const {
            return this->tiles.at(index);
          }

          /// <summary>
          /// Reassigns a tile in the grid at location (row, column). If the
          /// given Tile is not part of the tile set this TiledLayer uses, the
          /// behavior is undefined.
          /// </summary>
          void setTile(const Tile& tile, const uint16_t row, const uint16_t column) {
            assert(this->tiles.count(tile.tileID) == 1);
            uint32_t index = (row * columns) + column;
            this->setTile(tile.tileID, index);
          }

          void setTile(const Tile& tile, const uint32_t index) {
            assert(this->tiles.count(tile.tileID) == 1);
            this->setTile(tile.tileID, index);
          }

          void setTile(const size_t tileID, const uint16_t row, const uint16_t column) {
            uint32_t index = (row * columns) + column;
            this->setTile(tileID, index);
          }

          void setTile(const size_t tileID, const uint32_t index) {
            this->tileLayout[index] = tileID - this->firstGID;
          }

          /// <summary>
          /// Called once per frame for each TiledLayer in a game world while
          /// the game is being played. It determines which Tiles in the layer
          /// are in the viewport, and creates a RenderItem for each one. It
          /// then adds each one to the provided RenderList.
          /// </summary>
          void addRenderItemsToRenderList(graphics::RenderList&, const gui::Viewport&) const override;
          void findTileCollisionsForSprite(Physics&, CollidableObject&) override;
          bool willSpriteCollideOnTile(Physics&, const CollidableObject&) const override;
		  void generateCollidableTiles(b2World* gameWorld);
        protected:
          // NOTE: tileLayout stores a grid of Tiles (rows X columns),
          // but does so using a one-dimensional vector. So, when
          // accessing and changing tiles in this data structure,
          // rows and columns must be converted into indices.
          vector<size_t> tileLayout;
          unordered_map<size_t, Tile> tiles;
          int columns;
          int rows;
          int tileWidth;
          int tileHeight;
          int layerWidth;
          int layerHeight;
          int z;
          size_t firstGID;
		  std::list<CollidableTile*> collidableTilesList;
          // NOTE: z IS HERE IN CASE WE WANT TO LAYER SPRITES
          // IN FRONT OF OR BEHIND LAYERS IN A GAME WORLD

          // HELPER METHODS
          void initOverlappingCellRange(const AABB& aabb, int& startCol, int& endCol,
            int& startRow, int& endRow) const;
		  bool overlapsCollidableTile(AABB aabb);
		  bool isInsideCollidableTile(int centerX, int centerY);

        };
      }
    }
  }
}
