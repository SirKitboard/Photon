#pragma once

#include <Box2D/Box2D.h>
#include "sssf/game/Game.h"
#include <sssf/platforms/Cross/SFMLOS.h>

class PhotonDebugDraw  : public b2Draw
{
public:
	PhotonDebugDraw(cse380::sssf::game::Game* game)
	{	
		this->game = game;
		os = static_cast<cse380::sssf::sfml::SFMLOS*>(game->getOS());
		window = &(os->getWindow());
	}

	void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;
	void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override;
	void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override;
	void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;
	void DrawTransform(const b2Transform& xf) override;
	void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color);
	void DrawAABB(b2AABB* aabb, const b2Color& c);

	void setGame(cse380::sssf::game::Game* game)
	{
		this->game = game;
	}
private:
	cse380::sssf::sfml::SFMLOS* os;
	sf::RenderWindow* window;
	cse380::sssf::game::Game* game;
};
