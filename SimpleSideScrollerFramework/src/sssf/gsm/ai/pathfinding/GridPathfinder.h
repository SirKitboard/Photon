#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\gsm\physics\AABB.h"
#include "sssf/gsm/sprite/AnimatedSprite.h"


namespace cse380 {
	namespace sssf {
		namespace gsm {
			namespace ai {
				namespace pathfinding {
					
					class GridPathfinder
					{

					protected:
						game::Game *game;
						std::vector<bool> pathGrid;
						int numColumns;
						int numRows;
						physics::AABB selectedPathfindingCell;
						physics::AABB destinationPathfindingCell;

					public:
						physics::AABB* getSelectedPathfindingCell() { return &selectedPathfindingCell; }
						int getNumColumns() { return numColumns; }
						int getNumRows() { return numRows; }
						virtual void	updatePath(sprite::AnimatedSprite *sprite) = 0;
						virtual float	getColumnCenterX(int column) = 0;
						virtual float	getRowCenterY(int row) = 0;
						virtual void	mapPath(sprite::AnimatedSprite *sprite, float worldX, float worldY) = 0;
						virtual void	initGrid() = 0;
						virtual bool	isWalkable(int col, int row) = 0;
						virtual int		getGridWidth() = 0;
						virtual	int		getGridHeight() = 0;
					};
				}
			}
		}
	}
}