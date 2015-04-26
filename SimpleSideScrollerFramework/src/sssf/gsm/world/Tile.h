#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <Box2D/Dynamics/b2Body.h>

//#include "../physics/Box2D/Dynamics/b2Body.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace world {
        using std::string;
        using std::unordered_map;

        /// <summary>
        /// Represents a single image in a tiled background. Tiles can be
        /// collidable or not collidable. Tiles do not know where they are
        /// placed. They do know the index of the texture that should be used
        /// for rendering themselves as stored in the GameGraphics's world
        /// string table and its partnered texture manager.
        /// </summary>
        struct Tile {
          size_t textureID;
          size_t tileID;
          bool collidable;
		  b2Body *physicalTile;
          unordered_map<string, string> properties;
        };
      }
    }
  }
}
