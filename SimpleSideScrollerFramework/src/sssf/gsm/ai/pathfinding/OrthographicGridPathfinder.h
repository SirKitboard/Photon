#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf\game\Game.h"
#include "sssf\gsm\ai\pathfinding\GridPathfinder.h"
#include "sssf\gsm\world\World.h"

const int DIAGONAL_COST = 14;
const int COLUMN_COST = 10;
const int ROW_COST = 10;
const float GRID_EPSILON = 30.0f;
const float MAX_WALK_SPEED = 60.0f;


namespace cse380 {
	namespace sssf {

		namespace gsm {

			namespace physics { class Physics; }

			namespace ai {
				namespace pathfinding {
					using std::list;
					using std::map;
					using sprite::AnimatedSprite;
					class OrthographicGridPathfinder : public GridPathfinder
					{
					public:
						OrthographicGridPathfinder(game::Game *game);
						~OrthographicGridPathfinder(){}
						void mapPath(AnimatedSprite *sprite, float worldX, float worldY);
						void updatePath(AnimatedSprite *sprite);
						float getColumnCenterX(int column);
						float getRowCenterY(int row);
						void initGrid();
						bool isWalkable(int col, int row);
						int getGridWidth();
						int getGridHeight();
					private:
						bool containsPathNode(list<PathNode> *path, PathNode testNode);
						int calculateG(PathNode node);
						int calculateH(PathNode start, PathNode *end);
						bool hasReachedNode(AnimatedSprite *sprite, PathNode destination);
						void buildPath(list<PathNode> *pathToFill,
							float startX, float startY,
							float endX, float endY);
						int getGridIndex(int col, int row);

						void moveSelectedPathfindingCell(int worldX, int worldY);
						int getAdjacencyIndex(int col, int row);
						void findNeighborsToCheck(bool *adjacencies,
							PathNode *nodeToConsider,
							map<int, PathNode> *closedNodes);
						PathNode* findDupNodeInList(PathNode targetNode, list<PathNode> *nodeList);
						void addNeighbors(bool *nodesToAdd, PathNode *centerNode, PathNode *destination, map<int, PathNode> *openNodes, map<int, PathNode> *closedNodes);
						PathNode* findCheapestNode(map<int, PathNode> *openNodes);
						void removeNodeFromList(PathNode *nodeToRemove, list<PathNode> *nodeList);

					};
				}
			}
		}
	}
}