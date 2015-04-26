/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    TextureManager.h
    */

#pragma once

#include <cstdint>
#include <string>
#include "sssf/game/WStringTable.h"

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }

    namespace graphics {
      using std::wstring;
      using game::WStringTable;

      class GameGraphics;
      /// <summary>
      /// <para>
      /// Provides the foundation for technology specific classes that will manage
      /// game images. Different child classes may use different graphics libraries
      /// (i.e.DirectX, OpenGL) to load images into the GPU. This class provides
      /// a framework that all such classes should follow.
      /// </para>
      /// </summary>
      ///
      /// <remarks>
      /// <para>
      /// TextureManagers maintain a data structure of images mapped to file names.
      /// It also has a WStringTable, which maps file names to indices. The point
      /// of this is to allow mutliple game elements to share images where the
      /// images only need to be loaded into memory once, in the texture manager.
      /// It is up to the child classes to define the image-wchar_t mappings
      /// because the image types differ based on the technology used.
      /// </para>
      ///
      /// <para>
      /// In rendering things like game world backgrounds, we may wish to tile
      /// textures, repeating certain images over and over.This allows for us to
      /// efficiently render a large world without having to load giant images.
      /// </para>
      ///
      /// <para>
      /// Since <c>TextureManager</c>s map the textures to file names(and path), they
      /// should also be able to reload all the images they store if necessary.
      /// This may be necessary when the GPU device is lost and then regained.
      /// </para>
      /// </remarks>
      class TextureManager {
      public:
        TextureManager(GameGraphics* gfx) : graphics(gfx) {}
        virtual ~TextureManager() {}

        // No copying allowed!
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        // INLINED ACCESSOR METHOD
        const WStringTable& getWStringTable() const {
          return wstringTable;
        }

        // INLINED MUTATOR METHOD
        void setGraphics(GameGraphics* gfx) { graphics = gfx; }

        // VIRTUAL METHODS TO BE OVERRIDDEN BY TECHNOLOGY
        // SPECIFIC CLASSES

        // Deletes every texture loaded and clears the string table.
        virtual void clear() = 0;

        /// <summary>
        /// Loads a texture with the given path/file name key into the GPU's
        /// memory and stores a reference to it for later retrieval. The key is
        /// registered to the string table and the corresponding index is returned.
        /// If the texture with the given name already exists, returns the existing
        /// key without loading anything new.
        /// </summary>
        virtual size_t loadTexture(const wstring& key) = 0;

        /// <summary>
        /// Should our game lose access to the GPU, the textures we loaded in
        /// may be lost. This method reloads all textures originally used; call
        /// it when the GPU is regained.
        /// </summary>
        virtual void reloadAllTextures() = 0;
        virtual bool loadTileSetFromTexture(
          game::Game* game,
          const wstring& path,
          uint16_t tileWidth,
          uint16_t tileHeight
          ) = 0;
      protected:
        // WE MAY NEED THE GRAPHICS OBJECT TO HELP WITH LOADING IMAGES
        GameGraphics* graphics;

        // THE wstring TABLE SHOULD MAP FILE PATH/NAME TO INDEX
        WStringTable wstringTable;
      };
    }
  }
}
