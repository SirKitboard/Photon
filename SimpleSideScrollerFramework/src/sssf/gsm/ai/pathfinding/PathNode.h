#pragma once


struct PathNode
{
	int column;
	int row;
	int G;
	int H;
	PathNode *parentNode;
};