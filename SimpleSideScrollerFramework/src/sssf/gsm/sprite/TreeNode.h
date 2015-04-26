#pragma once

#include "sssf_VS\stdafx.h"
#include "sssf/gsm/ai/Bot.h"

using std::vector;

class TreeNode {
private:
	std::vector < Bot* > botList;
	int width;
	int height;
	int startingX;
	int startingY;
	int centerX;
	int centerY;
	int endingX;
	int endingY;

public:
	TreeNode(int x, int y, int w, int h) {
		width = w;
		height = h;
		startingX = x;
		startingY = y;
		centerX = x + (w / 2);
		centerY = y + (h / 2);
		endingX = x + w;
		endingY = y + h;
	}

	int getWidth() {
		return width;
	}
	int getHeight() {
		return height;
	}
	int getStartingX() {
		return startingX;
	}
	int getStartingY() {
		return startingY;
	}
	int getCenterX() {
		return centerX;
	}
	int getCenterY() {
		return centerY;
	}
	int getEndingX() {
		return endingX;
	}
	int getEndingY() {
		return endingY;
	}
	std::vector<Bot*> getBotlist() {
		return botList;
	}

	void clearList() {
		botList.clear();
	}

	void addBot(Bot * bot) {
		botList.push_back(bot);
	}

	int getSize() {
		return botList.size();
	}

};