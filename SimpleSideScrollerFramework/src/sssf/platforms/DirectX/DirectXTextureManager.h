/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    DirectXTextureManager.h

    This class is a TextureManager class that does all
    texture management using DirectX.
    */

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>

#include <windows.h>
#include <d3d9.h>

#include "sssf/game/Game.h"
#include "sssf/graphics/TextureManager.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace graphics { class GameGraphics; }
    namespace win32 {
      using std::wstring;
      using std::unordered_map;

      class DirectXTextureManager : public graphics::TextureManager {
      public:
        // INLINED ACCESSOR METHODS
        IDirect3DTexture9* getTexture(const wstring& fileName) {
          return textures.find(this->wstringTable.getIndexOfWString(fileName))->second;
        }

        // METHODS DEFINED IN DirectXTextureManager.cpp
        DirectXTextureManager(graphics::GameGraphics* gfx);
        virtual ~DirectXTextureManager();
        void clear() override;
        size_t loadTexture(const wstring& key) override;
        void reloadAllTextures() override;

        // PLATFORM-SPECIFIC
        /// <summary>
        /// Returns the texture with the provided id. This is done by looking up
        /// the file path/name in the string table that corresponds to the id index,
        /// then fetching the appropritate texture out of the map.
        /// </summary>
        IDirect3DTexture9*  getTexture(const size_t id);
        unsigned int fillTexture(const wstring& key, IDirect3DTexture9** textureToFill);
        bool loadTileSetFromTexture(
          game::Game* game,
          const wstring& path,
          uint16_t tileWidth,
          uint16_t tileHeight
          ) override;

      private:
        // HERE ARE OUR TEXTURES. NOTE THAT THE FILE NAME/PATH FOR ALL
        // TEXTURES ARE STORED IN THE INHERITED STRING TABLE AND THAT
        // THE INDICES OF THE FILE NAMES THERE MUST CORRESPOND TO THE
        // INDICES OF THESE TEXTURES
        unordered_map<size_t, IDirect3DTexture9*> textures;
      };
    }
  }
}