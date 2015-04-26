/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    DirectXTextureManager.cpp

    See DirectXTextureManager for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/platforms/DirectX/DirectXTextureManager.h"

#include "sssf/graphics/GameGraphics.h"
#include "sssf/platforms/DirectX/DirectXGraphics.h"
#include "sssf/util/Util.h"

namespace cse380 {
  namespace sssf {
    namespace win32 {
      using std::cout;
      using std::endl;
      using std::string;
      using graphics::GameGraphics;

      /*
          DirectXTextureManager - Default constructor, it constructs the map that
          will store all of the textures and file paths/names.
          */
      DirectXTextureManager::DirectXTextureManager(GameGraphics* gfx)
        : TextureManager(gfx) {}

      /*
          ~DirectXTextureManager - Destructor, it cleans up all the textures.
          */
      DirectXTextureManager::~DirectXTextureManager() {
        this->clear();
      }

      IDirect3DTexture9* DirectXTextureManager::getTexture(const size_t id) {
        // RETURN THE TEXTURE MAPPED TO THAT FILE NAME
        return textures[id];
      }

      size_t DirectXTextureManager::loadTexture(const wstring& key) {
        IDirect3DTexture9* textureToLoad;
        size_t prior = wstringTable.getIndexOfWString(key);
        std::string skey = util::to_string(key);
        if (prior != WStringTable::NOT_FOUND) {
#ifdef DEBUG
          cout << "Sought texture \"" << skey << "\" (ID " << prior << " at table "
            << &wstringTable << "), and it was already loaded" << endl;
#endif
          return prior;
        }


        if (fillTexture(key, &textureToLoad) == S_OK) {
          // PUT THE KEY IN THE STRING TABLE
          size_t index = wstringTable.putWStringInTable(key);

          // NOW PUT OUR LOADED TEXTURE INTO OUR MAP
          textures[index] = textureToLoad;

#ifdef DEBUG
          std::cout << "Loaded texture \"" << skey << "\" (ID "
            << index << " at table " << &wstringTable << ')' << std::endl;
#endif

          // AND RETURN THE TEXTURE'S ID
          return index;
        }

#ifdef DEBUG
        std::cout << "Could not load texture " << skey << std::endl;
#endif
        return -1;
      }

      unsigned int DirectXTextureManager::fillTexture(const wstring& key,
        IDirect3DTexture9** textureToFill) {
        // USED FOR READING BITMAP FILE INFO
        D3DXIMAGE_INFO info;

        // CONVERT THE FILE NAME INTO A WINDOW LONG CHAR wchar_t (LPCWSTR)
        const wchar_t* fileName = key.c_str();

        // GET THE IMAGE SIZE FROM THE IMAGE FILE
        HRESULT result = D3DXGetImageInfoFromFile(fileName, &info);

        DirectXGraphics* gfx = static_cast<DirectXGraphics*>(graphics);
        // IF THE IMAGE FILE WAS FOUND
        if (result == S_OK) {
          // GET THE COLOR KEY, WE'LL NEED THIS FOR LOADING OUR IMAGE
          D3DCOLOR colorKey = gfx->getColorKey();

          IDirect3DDevice9* graphicsDevice = gfx->getGraphicsDevice();

          // CREATE A NEW TEXTURE
          result = D3DXCreateTextureFromFileEx(
            graphicsDevice,     // GPU
            fileName,           // BITMAP FILE PATH/NAME
            info.Width,         // BITMAP IMAGE WIDTH
            info.Height,        // BITMAP IMAGE HEIGHT
            1,                  // MIP-MAP LEVELS (1 FOR NO CHAIN)
            D3DPOOL_DEFAULT,    // THE TYPE OF SURFACE (STANDARD)
            D3DFMT_UNKNOWN,     // SURFACE FORMAT (DEFAULT)
            D3DPOOL_DEFAULT,    // MEMORY CLASS FOR THE TEXTURE
            D3DX_DEFAULT,       // IMAGE FILTER
            D3DX_DEFAULT,       // MIP FILTER
            colorKey,           // COLOR KEY
            &info,              // BITMAP FILE INFO
            nullptr,            // COLOR PALETTE
            textureToFill);     // THE TEXTURE WE ARE CREATING AND LOADING

          return S_OK;
        }
        else
          return -1;
      }


      void DirectXTextureManager::reloadAllTextures() {
        for (const auto& texture : textures) {
          loadTexture(this->wstringTable.getWStringFromTable(texture.first));
        }
      }

      void DirectXTextureManager::clear() {
        for (auto& texture : textures) {
          texture.second->Release();
        }
        textures.clear();
        wstringTable.clear();
      }


      bool DirectXTextureManager::loadTileSetFromTexture(Game* game,
        const wstring& sourcePath,
        uint16_t tileWidth,
        uint16_t tileHeight) {
        // CONVERT THE FILE NAME INTO A WINDOW LONG CHAR wchar_t (LPCWSTR)

        IDirect3DTexture9* textureToDivide;
        IDirect3DSurface9* surfaceToDivide;

        string ssource = util::to_string(sourcePath);
        unsigned int fillResult = fillTexture(sourcePath, &textureToDivide);
        textureToDivide->GetSurfaceLevel(0, &surfaceToDivide);
        if (fillResult != S_OK) {
#ifdef DEBUG
          cout << "Failed to load tileset image \"" << ssource << '"' << endl;
#endif
          return false;
        }
        // DETERMINE THE NUMBER OF TILE ROWS AND COLUMNS
        D3DSURFACE_DESC surfaceDesc;
        surfaceToDivide->GetDesc(&surfaceDesc);
        int textureWidth = surfaceDesc.Width;
        int textureHeight = surfaceDesc.Height;
        int columns = textureWidth / tileWidth;
        int rows = textureHeight / tileHeight;
        DirectXGraphics* dxg = static_cast<DirectXGraphics*>(graphics);

        IDirect3DDevice9* graphicsDevice = dxg->getGraphicsDevice();
        size_t startID = wstringTable.getNumWStringsInTable();

        // THE TILE SET IMAGE LOADED SUCCESSFULLY, SO LET'S CUT IT UP
        for (int row = 0; row < rows; row++) {
          for (int column = 0; column < columns; column++) {
            IDirect3DTexture9* extractedTexture;
            IDirect3DSurface9* extractedSurface;
            fillResult = graphicsDevice->CreateRenderTarget(
              tileWidth,
              tileHeight,
              D3DFMT_A8R8G8B8,
              D3DMULTISAMPLE_NONE,
              0,
              false,
              &extractedSurface,
              nullptr
              );
            if (fillResult != S_OK) return false;

            RECT sourceRect;
            sourceRect.left = column * tileWidth;
            sourceRect.right = tileWidth + (column * tileWidth) - 1;
            sourceRect.top = row * tileHeight;
            sourceRect.bottom = tileHeight + (row * tileHeight) - 1;

            graphicsDevice->StretchRect(
              surfaceToDivide,
              &sourceRect,
              extractedSurface,
              nullptr,
              D3DTEXF_NONE
              );

            // BUILD A UNIQUE FILE NAME FOR THIS TEXTURE
            size_t id = wstringTable.getNumWStringsInTable();
            wstring textureFilename = sourcePath + std::to_wstring(id) + L".png";

            // LET'S PUT THE SURFACE IN AN IMAGE FILE
            D3DXSaveSurfaceToFileW(
              textureFilename.c_str(),
              D3DXIFF_PNG,
              extractedSurface,
              nullptr,
              nullptr
              );
            D3DXIMAGE_INFO info;
            HRESULT result = D3DXGetImageInfoFromFile(textureFilename.c_str(), &info);
            if (result != S_OK) return false;

            // AND THEN LOAD IT BACK IN AS A TEXTURE
            result = D3DXCreateTextureFromFileEx(
              graphicsDevice,         // GPU
              textureFilename.c_str(),            // BITMAP FILE PATH/NAME
              tileWidth,          // BITMAP IMAGE WIDTH
              tileHeight,         // BITMAP IMAGE HEIGHT
              1,                  // MIP-MAP LEVELS (1 FOR NO CHAIN)
              D3DPOOL_DEFAULT,    // THE TYPE OF SURFACE (STANDARD)
              D3DFMT_UNKNOWN,     // SURFACE FORMAT (DEFAULT)
              D3DPOOL_DEFAULT,    // MEMORY CLASS FOR THE TEXTURE
              D3DX_DEFAULT,       // IMAGE FILTER
              D3DX_DEFAULT,       // MIP FILTER
              0x000000,           // COLOR KEY
              &info,              // BITMAP FILE INFO
              nullptr,            // COLOR PALETTE
              &extractedTexture); // THE TEXTURE WE ARE CREATING AND LOADING
            if (result != S_OK) return false;

            // ADD IT TO THE STRING TABLE
            size_t index = wstringTable.putWStringInTable(textureFilename);

            // AND ADD IT TO THE TEXTURES
            textures[index] = extractedTexture;
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