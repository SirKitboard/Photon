#include "sssf_VS/stdafx.h"
#include "sssf/platforms/Cross/SFMLTextureManager.h"

#include "sssf/platforms/Cross/SFMLGraphics.h"
#include "sssf/util/Util.h"

namespace cse380 {
  namespace sssf {
    namespace sfml {
      using std::cout;
      using std::endl;
      using std::string;

      void SFMLTextureManager::clear() {
        _textures.clear();
        wstringTable.clear();
#ifdef DEBUG
        cout << "Cleared all loaded SFML textures in texture manager " << this << endl;
#endif
      }

      size_t SFMLTextureManager::loadTexture(const wstring& key) {
        size_t prior = wstringTable.getIndexOfWString(key);
        string skey = util::to_string(key);
        if (prior != WStringTable::NOT_FOUND) {
#ifdef DEBUG
          cout << "Sought texture \"" << skey << "\" (ID " << prior << " at manager "
            << this << "), and it was already loaded" << endl;
#endif
          return prior;
        }

        sf::Image image;
        if (!image.loadFromFile(skey)) {
          // If we couldn't load the texture...
#ifdef DEBUG
          cout << "Could not load texture " << skey << endl;
#endif
          return -1;
        }

        SFMLGraphics* gfx = static_cast<SFMLGraphics*>(this->graphics);

        image.createMaskFromColor(gfx->getColorKey());
        // Any engine worth its salt these days supports transparency in textures,
        // so color keys aren't technically necessary (though they're faster, I
        // think). But I wanted to maintain compatibility with McKenna's engine.
        // -JTG

        sf::Texture texture;

        if (!texture.loadFromImage(image)) {
#ifdef DEBUG
          cout << "Unknown problem converting sf::Image to sf::Texture (\""
            << skey << "\")" << endl;
#endif
          return -1;
        }

        // NOW PUT THE KEY IN THE wchar_t TABLE
        size_t index = wstringTable.putWStringInTable(key);
        texture.setSmooth(true);

        this->_textures[index] = std::move(texture);

#ifdef DEBUG
        cout << "Loaded texture \"" << skey << "\" (ID " << index << " at manager "
          << this << ')' << endl;
#endif
        // AND RETURN THE TEXTURE'S ID
        return index;
      }

      void SFMLTextureManager::reloadAllTextures() {
#ifdef DEBUG
        cout << "SFMLTextureManager::reloadAllTextures() called, but it manages"
          << "its own resources already." << endl;
#endif
      }

      bool SFMLTextureManager::loadTileSetFromTexture(
        game::Game* game,
        const wstring& sourcePath,
        uint16_t tileWidth,
        uint16_t tileHeight) {
        using std::cout;
        using std::endl;
        // Note: Ideally, when dealing with a tile set, you should store the whole
        // set as one texture, then just render slices of it; this is called texture
        // atlasing.  Switching textures is expensive; picking a slice of the
        // currently-bound texture to render is cheap.  You could add texture atlasing
        // fairly easily, but that would require adding extra information to AnimatedSprites
        // and Tiles (namely, the rectangle that describes the slice to render).
        //
        // If you want to do this, ask me about it and I'd be happy to help you!
        // -JTG
        std::string ssource = util::to_string(sourcePath);
        sf::Image image;

        if (!image.loadFromFile(ssource)) {
#ifdef DEBUG
          cout << "Failed to load tileset image \"" << ssource << '"' << endl;
#endif
          return false;
        }

        SFMLGraphics* gfx = static_cast<SFMLGraphics*>(this->graphics);
        image.createMaskFromColor(gfx->getColorKey());

        sf::Vector2u size = image.getSize();
        int columns = size.x / tileWidth;
        int rows = size.y / tileHeight;

        size_t startID = wstringTable.getNumWStringsInTable();
        // THE TILE SET IMAGE LOADED SUCCESSFULLY, SO LET'S CUT IT UP
        for (int row = 0; row < rows; ++row) {
          for (int column = 0; column < columns; ++column) {
            sf::IntRect sourceRect;
            sourceRect.left = column * tileWidth;
            sourceRect.width = tileWidth;
            sourceRect.top = row * tileHeight;
            sourceRect.height = tileHeight;

            // BUILD A UNIQUE FILE NAME FOR THIS TEXTURE
            size_t id = wstringTable.getNumWStringsInTable();
            wstring textureFilename = sourcePath + std::to_wstring(id) + L".png";
            // McKenna's DirectX implementation saves these textures back to files
            // and reloads them.  I have no idea why.  Maybe I'll cut that at some
            // point, but I can't be bothered to right now.
            // -JTG

            // ADD IT TO THE STRING TABLE
            size_t newID = wstringTable.putWStringInTable(textureFilename);

            // AND ADD IT TO THE TEXTURES
            sf::Texture& tex = (_textures[newID] = sf::Texture());
            tex.loadFromImage(image, sourceRect);
          }
        }

#ifdef DEBUG
        size_t endID = wstringTable.getNumWStringsInTable() - 1;
        cout << "Loaded tileset image \"" << ssource << "\" (IDs " << startID
          << '-' << endID << " at manager " << this << ')' << endl;
#endif
        return true;
      }
    }
  }
}
