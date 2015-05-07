/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    TiledLayer.cpp

    See TiledLayer.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/world/TiledLayer.h"

#include "sssf/gsm/physics/AABB.h"
#include "sssf/gsm/physics/CollidableObject.h"
#include "sssf/gsm/physics/Physics.h"
#include "sssf/game/Game.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/graphics/RenderList.h"
#include "sssf/gsm/world/Tile.h"
#include "sssf/gui/Viewport.h"
#include "sssf/util/Util.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace world {
        using std::max;
        using std::min;

        TiledLayer::TiledLayer(int initColumns,
          int initRows,
          int initTileWidth,
          int initTileHeight,
          int initZ,
          bool initCollidableTiles,
          int initWorldWidth,
          int initWorldHeight,
          size_t gid) :
          rows(initRows),
          columns(initColumns),
          tileWidth(initTileWidth),
          tileHeight(initTileHeight),
          z(initZ),
          firstGID(firstGID),
          tileLayout(initColumns * initRows, 0) {
          worldWidth = initWorldWidth;
          worldHeight = initWorldHeight;
          collidableTiles = initCollidableTiles;
          calculateAndSetLayerWidth();
          calculateAndSetLayerHeight();
        }

        void TiledLayer::addRenderItemsToRenderList(graphics::RenderList& renderList,
          const gui::Viewport& viewport) const {
          // WE'LL USE THIS IN OUR CALCULATIONS
          int vx = viewport.getViewportX();
          int vy = viewport.getViewportY();

          // THE GUI MIGHT TAKE UP PART OF THE SCREEN, SO FACTOR
          // THAT IN TO OUR RENDERING OF THE GAME WORLD
          int vw = viewport.getViewportWidth();
          int vh = viewport.getViewportHeight();

          // TILES THAT WE PUT IN THE RENDER LIST ARE GOING TO BE
          // RENDERED OF COURSE, SO WE ONLY WANT TO PUT TILES IN THE
          // LIST THAT ARE ACTUALLY IN THE CURRENT VIEWPORT. HERE
          // WE ARE CALCULATING WHICH Tiles IN OUR GRID ARE IN THE
          // VIEWPORT. FOR THIS, WE NEED THE START AND END rows AND
          // columns OF THE GRID.

          // 0 IS THE LOWEST COLUMN, SO CLAMP AT 0
          int startCol = max(0, calculateViewportStartColumn(vx, vw));

          // columns - 1 IS THE LARGEST COLUMN, SO CLAMP THERE
          int endCol = min(columns - 1, calculateViewportEndColumn(vx, vw));

          // 0 IS THE LOWEST ROW, SO CLAMP AT 0
          int startRow = max(0, calculateViewportStartRow(vy, vh));

          // rows - 1 IS THE LARGEST ROW, SO CLAMP THERE
          int endRow = min(rows - 1, calculateViewportEndRow(vy, vh));

          float dlw = layerWidth - vw;
          float dww = worldWidth - vw;
          float dlh = layerHeight - vh;
          float dwh = worldHeight - vh;

          float dwidth = dlw / dww;
          float dheight = dlh / dwh;

          // NOW GO THROUGH THE PART OF THE GRID THAT WE'VE DETERMINED
          // IS IN THE VIEWPORT AND ADD THOSE CELLS TO THE RENDER LIST
          // AS RenderItemS. TO ENSURE PARALLAX SCROLLING, SUBTRACT
          // THE CORRECTED parallaxViewportX & parallaxViewportY FROM
          // THE TILE X & Y VALUES TO SCALE SCROLLING.
		  int temp;
          for (int i = startRow; i <= endRow; i++) {
            for (int j = startCol; j <= endCol; j++) {
              const Tile* tileToAdd = getTile(i, j);

              if (tileToAdd) {
	              if (tileToAdd->collidable==false) {
		              int parallaxTileX = (j * static_cast<float>(tileWidth)) - (vx * dwidth);
		              int parallaxTileY = (i * static_cast<float>(tileHeight)) - (vy * dheight);

		              //int parallaxTileX = (int)((j * tileWidth) - (vx * (((float)(layerWidth - vw)) / ((float)(worldWidth - vw)))));
		              //int parallaxTileY = (int)((i * tileHeight) - (vy * (((float)(layerHeight - vh)) / ((float)(worldHeight - vh)))));

		              // APPLY PARALLAX SCROLLING TO THE COORDINATES
		              renderList.addRenderItem(
			              tileToAdd->textureID,
			              parallaxTileX - 32,
			              parallaxTileY - 32,
			              z,
			              255,
			              tileWidth,
			              tileHeight,
						  0
		              );
					  tileToAdd->textureID;
	              }
					  temp = tileToAdd->textureID;
              }
            }
          }
		  std::list <CollidableTile*>::const_iterator co_iterator = collidableTilesList.begin();
		  int counter = 0;
		  while (co_iterator != collidableTilesList.end()) {
			  CollidableTile* cotile = (*co_iterator);
			  b2Body* bodytoadd = cotile->tileBody;
			  if (bodytoadd->GetPosition().x > vx-63 && bodytoadd->GetPosition().x < vx + vw+63){
				  if (bodytoadd->GetPosition().y > vy-63 && bodytoadd->GetPosition().y < vy + vh+63) {
					  renderList.addRenderItem(
						  cotile->tile->textureID-1,
						  bodytoadd->GetPosition().x - vx - 32,
						  bodytoadd->GetPosition().y - vy - 32,
						  z,
						  255,
						  tileWidth,
						  tileHeight,
						  0
					  );
				  }
			  }
			 
			  ++co_iterator;
		  }
		  //LMAOWTF = (LMAOWTF - 4) % 10 + 5;
        }


		//TODO: Create Replacement
        // This method will be largely irrelevant once you use Box2D
        void TiledLayer::findTileCollisionsForSprite(Physics& physics,
          CollidableObject& dynamicObject) {
          int startColumn, endColumn, startRow, endRow;
          const AABB& aabb = dynamicObject.getSweptShape();
          initOverlappingCellRange(aabb, startColumn, endColumn, startRow, endRow);
          for (int i = startColumn; i <= endColumn; i++) {
            for (int j = startRow; j <= endRow; j++) {
              Tile* testTile = getTile(j, i);
              if (testTile && testTile->collidable) {
                float tileX = i * tileWidth;
                float tileY = j * tileHeight;
                physics.addTileCollision(&dynamicObject, testTile, tileX, tileY,
                  tileWidth, tileHeight);
              }
            }
          }
        }

		void TiledLayer::generateCollidableTiles(b2World* gameWorld) {
			//LMAOWTF = 5;
			for (int i = 0; i < rows; i++) {
				for (int j = 0; j < columns; j++) {
					const Tile* tileToAdd = getTile(i,j);

					if (tileToAdd) {
						if (tileToAdd->collidable) {
							int parallaxTileX = (j * static_cast<float>(tileWidth));
							int parallaxTileY = (i * static_cast<float>(tileHeight));

							//int parallaxTileX = (int)((j * tileWidth) - (vx * (((float)(layerWidth - vw)) / ((float)(worldWidth - vw)))));
							//int parallaxTileY = (int)((i * tileHeight) - (vy * (((float)(layerHeight - vh)) / ((float)(worldHeight - vh)))));
							// APPLY PARALLAX SCROLLING TO THE COORDINATES
							b2BodyDef tileBodyDef;
							tileBodyDef.position.Set(j * tileWidth, i * tileHeight);
							b2Body* tileBody = gameWorld->CreateBody(&tileBodyDef);
							b2PolygonShape tileBox;
							tileBox.SetAsBox(32.0f, 32.0f);
							tileBody->CreateFixture(&tileBox, 0.0f);
							tileBody->SetActive(true);
							CollidableTile* cotile = new CollidableTile();
							cotile->tile = tileToAdd;
							cotile->tileBody = tileBody;
							collidableTilesList.push_back(cotile);
						}

					}
				}
			}
		}


		//TODO: Create Replacement
        // This method will be largely irrelevant once you use Box2D
        bool TiledLayer::willSpriteCollideOnTile(Physics& physics,
          const CollidableObject& dynamicObject) const {
          int startCol, endCol, startRow, endRow;
          const AABB& ss = dynamicObject.getSweptShape();
          initOverlappingCellRange(ss, startCol, endCol, startRow, endRow);
          AABB tileAABB;
          for (int i = startCol; i <= endCol; i++) {
            int j = endRow;
            const Tile* testTile = getTile(j, i);
            if (testTile && testTile->collidable) {
              float tileX = i * tileWidth;
              float tileY = j * tileHeight;
              tileAABB.setCenterX(tileX + (tileWidth / 2));
              tileAABB.setCenterY(tileY + (tileHeight / 2));
              tileAABB.setWidth(tileWidth);
              tileAABB.setHeight(tileHeight);
              bool willCollide = physics.willSpriteCollideOnTile(dynamicObject, tileAABB);
              if (willCollide)
                return true;
            }
          }
          return false;
        }

		void TiledLayer::initOverlappingCellRange(const AABB& aabb,
			int& startCol, int& endCol, int& startRow, int& endRow) const {
			// GET ALL TILES THAT OVERLAP THE left,right,top,bottom AREA
			// AND TAKE A CollidableObject FROM the recycledList FOR EACH
			// ONE AND PUT IT INTO BOTH AXIS SWEEP VECTORS
			float left = aabb.getLeft();
			float right = aabb.getRight();
			float top = aabb.getTop();
			float bottom = aabb.getBottom();
			startCol = round(left / tileWidth);
			endCol = round(right / tileWidth);
			startRow = round(top / tileHeight);
			endRow = round(bottom / tileHeight);

			// DON'T ALLOW ILLEGAL VALUES
			startCol = util::clamp(startCol, 0, columns - 1);
			endCol = util::clamp(endCol, 0, columns - 1);
			startRow = util::clamp(startRow, 0, rows - 1);
			endRow = util::clamp(endRow, 0, rows - 1);
		}

		bool TiledLayer::isInsideCollidableTile(int centerX, int centerY)
		{
			int col = centerX / tileWidth;
			int row = centerY / tileHeight;
			return getTile(row, col)->collidable;
		}

		bool TiledLayer::overlapsCollidableTile(AABB aabb)
		{
			int startCol, endCol, startRow, endRow;
			this->initOverlappingCellRange(aabb, startCol, endCol, startRow, endRow);
			for (int i = startCol; i <= endCol; i++)
			{
				for (int j = startRow; j <= endRow; j++)
				{
					Tile *testTile = this->getTile(j, i);
					if (testTile->collidable)
						return true;
				}
			}
			return false;
		}
      }
    }
  }
}
