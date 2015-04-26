#include "sssf_VS/stdafx.h"
#include "sssf/gsm/sprite/QuadTree.h"

QuadTree::QuadTree() {
	
	initTree(0, 3200, 3200,0,0);
	cout << "Blahblahblah";
}

void QuadTree::initTree(int index, int width, int height, int x, int y) {
	if (index < 21) {
		botHeap[index] = new TreeNode(x, y, width, height);
		int newWidth = width / 2;
		int newHeight = height / 2;
		initTree(4 * index + 1, newWidth, newWidth, x, y);
		initTree(4 * index + 2, newWidth, newWidth, x+newWidth, y);
		initTree(4 * index + 3, newWidth, newWidth, x+newWidth, y+newHeight);
		initTree(4 * index + 4, newWidth, newWidth, x, y+newHeight);
	}
	else;
}

void QuadTree::clearTree() {
	for (int i = 0; i < 21; i++) {
		botHeap[i]->clearList();
	}
}

void QuadTree::generateTree(std::list<Bot*> botList) {
	clearTree();
	for (Bot* bot : botList) {
		addToTree(0, bot);
	}
}


void QuadTree::addToTree(int index, Bot* bot) {
	if (index>=5 && index<21) {
		botHeap[index]->addBot(bot);
	}
	else if(index<21){
		float left = bot->getBoundingVolume()->getLeft();
		float right = bot->getBoundingVolume()->getRight();
		float top = bot->getBoundingVolume()->getTop();
		float bottom = bot->getBoundingVolume()->getBottom();

		if (right < botHeap[index]->getCenterX() && bottom < botHeap[index]->getCenterY()) {
			addToTree(4 * index + 1, bot);
		}
		else if (left > botHeap[index]->getCenterX() && bottom < botHeap[index]->getCenterY()) {
			addToTree(4 * index + 2, bot);
		}
		else if (left > botHeap[index]->getCenterX() && top > botHeap[index]->getCenterY()) {
			addToTree(4 * index + 3, bot);
		}
		else if (right < botHeap[index]->getCenterX() && top > botHeap[index]->getCenterY()) {
			addToTree(4 * index + 4, bot);
		}
		else {
			botHeap[index]->addBot(bot);
		}
	}
}

std::list<Bot*> QuadTree::getBotsInViewport(Viewport* viewport) {
	std::list<Bot*> foundBots;
	int x = viewport->getViewportX();
	int y = viewport->getViewportY();
	int w = viewport->getViewportWidth();
	int h = viewport->getViewportHeight();
	for (int i = 0; i < 21; i++) {
		if (x>botHeap[i]->getEndingX() || (x + w) < botHeap[i]->getStartingX());
		else if (y > botHeap[i]->getEndingY() || (y + h) < botHeap[i]->getStartingY());
		else {
			for (Bot * bot : botHeap[i]->getBotlist()) {
				foundBots.push_back(bot);
			}
		}
	}
	return foundBots;
}

int QuadTree::indexOf(Player* player) {
	return indexOfBot(player, 0);
}

int QuadTree::indexOfBot(AnimatedSprite* animated, int index) {
	if (index >= 5 && index<21) {
		return index;
	}
	else {
			float left = animated->getBoundingVolume()->getLeft();
			float right = animated->getBoundingVolume()->getRight();
			float top = animated->getBoundingVolume()->getTop();
			float bottom = animated->getBoundingVolume()->getBottom();

			if (right < botHeap[index]->getCenterX() && bottom < botHeap[index]->getCenterY()) {
				indexOfBot(animated,4 * index + 1);
			}
			else if (left > botHeap[index]->getCenterX() && bottom < botHeap[index]->getCenterY()) {
				indexOfBot(animated, 4 * index + 2);
			}
			else if (left > botHeap[index]->getCenterX() && top > botHeap[index]->getCenterY()) {
				indexOfBot(animated,4 * index + 3);
			}
			else if (right < botHeap[index]->getCenterX() && top > botHeap[index]->getCenterY()) {
				indexOfBot(animated,4 * index + 4);
			}
			else {
				return index;
			}
		
	}
}

std::list<Bot*> QuadTree::getBotsInNode(int index) {
	std::list<Bot*> bots;
	if (index == 0) {
		for (int i = 0; i < 21; i++) {
			for (Bot* bot : botHeap[i]->getBotlist()) {
				bots.push_back(bot);
			}
		}
	}
	else if (index > 0 && index < 5) {
		for (Bot* bot : botHeap[0]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[index]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[4*index+1]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[4 * index + 1]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[4 * index + 2]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[4 * index + 3]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[4 * index + 4]->getBotlist()) {
			bots.push_back(bot);
		}	
	}
	else {
		for(Bot* bot : botHeap[0]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[index]->getBotlist()) {
			bots.push_back(bot);
		}
		for (Bot* bot : botHeap[(index-1)/4]->getBotlist()) {
			bots.push_back(bot);
		}
	}
	return bots;
}
