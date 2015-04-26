/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    DirectXGraphics.h

    This class is a GameGraphics class that does all graphics
    management using DirectX.
    */

#pragma once

#include <cstdint>
#include <vector>

#include <d3d9.h>
#include <windows.h>

#include "sssf/graphics/GameGraphics.h"
#include "sssf/os/GameOS.h"
#include "sssf/text/GameText.h"

namespace cse380 {
  namespace sssf {
    namespace win32 {
      using std::vector;
      using namespace graphics;

      // THIS IS OUR DESIRED COLOR FORMAT. HOPEFULLY THE PLAYER'S
      // GRAPHICS CARD WILL HAVE IT
      const D3DFORMAT DEFAULT_COLOR_FORMAT = D3DFMT_X8R8G8B8;

      // THIS IS THE COLOR WE WILL USE TO CLEAR THE SCREEN WITH
      const D3DCOLOR  BACKGROUND_COLOR = D3DCOLOR_XRGB(96, 96, 96);

      class DirectXGraphics : public GameGraphics {
      public:
        // INLINED ACCESSOR METHODS
        D3DCOLOR getColorKey() const { return colorKey; }
        D3DCOLOR getFontColor() const { return fontColor; }
        IDirect3DDevice9* getGraphicsDevice() { return graphicsDevice; }
        const IDirect3DDevice9* getGraphicsDevice() const { return graphicsDevice; }
        const D3DPRESENT_PARAMETERS& getPresentParameters() const { return presentParameters; }
        ID3DXSprite* getSpriteHandler() { return spriteHandler; }
        const ID3DXSprite* getSpriteHandler() const { return spriteHandler; }

        // METHODS DEFINED IN DirectXGraphics.cpp
        DirectXGraphics(Game* initGame);
        virtual ~DirectXGraphics();

        // OVERRIDDEN GameGraphics METHODS DEFINED IN DirectXGraphics.cpp
        bool containsDisplayMode(
          const vector<D3DDISPLAYMODE>&,
          const D3DFORMAT,
          const uint16_t,
          const uint16_t
          ) const;
        
        void findAlternativeDisplayMode(const vector<D3DDISPLAYMODE>&, D3DFORMAT&);
        void initGraphics(GameOS*, const bool) override;
        void initTextFont(const uint16_t fontSize) override;
        void reloadGraphics() override;
        void renderGame(Game* game) override;
        void renderTextToDraw(const TextToDraw& textToDraw) override;
        void setColorKey(const uint8_t r, const uint8_t g, const uint8_t b) override;
        void setFontColor(const uint8_t r, const uint8_t g, const uint8_t b) override;
        void shutdown() override;
      protected:
        TextureManager* createTextureManager() override;
      private:
        // DIRECT3D STUFF FOR RENDERING
        IDirect3D9* d3d;
        IDirect3DDevice9* graphicsDevice;
        ID3DXFont* textFont;
        ID3DXSprite* spriteHandler;
        D3DPRESENT_PARAMETERS presentParameters;
        D3DCOLOR colorKey;
        D3DCOLOR fontColor;
        RECT textRect;

        // INTERNAL METHODS DEFINED IN DirectXGraphics.cpp
        HRESULT createDirectXDeviceAndSpriteHandler();
        void endDirectXFrameRendering();
        vector<D3DDISPLAYMODE> getDirectXDisplayModes();
        void renderGUIRenderList();
        void renderWorldRenderList();
        void startDirectXFrameRendering();
      };
    }
  }
}
