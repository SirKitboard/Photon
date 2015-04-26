/*
    Author: Jesse Talavera-Greenberg
    Don't like DirectX?  Neither do I.
    */

#ifndef SFMLTEXTUREMANAGER_H
#define SFMLTEXTUREMANAGER_H

#include <cstdint>
#include <string>
#include <unordered_map>

#include "sssf/graphics/TextureManager.h"

namespace cse380 {
  namespace sssf {
    namespace graphics { class GameGraphics; }
    namespace sfml {

      using std::wstring;
      using sssf::graphics::TextureManager;
      using sssf::graphics::GameGraphics;


      class SFMLTextureManager : public TextureManager {
      public:
        SFMLTextureManager(GameGraphics* gfx) : TextureManager(gfx) {}
        virtual ~SFMLTextureManager() {}

        void clear() override;
        size_t loadTexture(const wstring& key) override;
        void reloadAllTextures() override;
        bool loadTileSetFromTexture(
          game::Game* game,
          const wstring& path,
          uint16_t tileWidth,
          uint16_t tileHeight
          ) override;

        sf::Texture& getTexture(const size_t id) {
          // RETURN THE TEXTURE MAPPED TO THE ID
          return _textures[id];
        }

        const sf::Texture& getTexture(const size_t id) const {
          return _textures.at(id);
        }

      private:
        std::unordered_map<size_t, sf::Texture> _textures;
      };
    }
  }
}

#endif // SFMLTEXTUREMANAGER_H
