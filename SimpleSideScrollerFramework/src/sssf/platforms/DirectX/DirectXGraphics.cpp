/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    DirectXGraphics.cpp

    See DirectXGraphics.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/platforms/DirectX/DirectXGraphics.h"

#include "sssf/game/Game.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/graphics/TextureManager.h"
#include "sssf/gui/GameGUI.h"
#include "sssf/gui/Viewport.h"
#include "sssf/os/GameOS.h"
#include "sssf/platforms/DirectX/DirectXTextureManager.h"
#include "sssf/platforms/Windows/WindowsOS.h"
#include "sssf/text/GameText.h"

namespace cse380 {
  namespace sssf {
    namespace win32 {
      using std::cout;
      using std::endl;
      using std::round;

      /*
          DirectXGraphics - Default constructor, it doesn't initialize anything.
          To setup all the DirectX objects call initGraphics after construction.
          */
      DirectXGraphics::DirectXGraphics(Game* initGame) {
        this->game = initGame;
      }

      /*
          ~DirectXGraphics - Destructor, it destroys all of the DirectX pointers.
          This would be called only when the application closes, unless someone
          decides to use different rendering technologies during the game.
          */
      DirectXGraphics::~DirectXGraphics() {
        this->shutdown();
      }

      /*
          containsDisplayMode - This method looks at the vector of display modes that
          were presumably retrieved from the GPU, and tests to see if our desired
          color format and screen resolution are inside. If found, true is returned,
          otherwise false.
          */
      bool DirectXGraphics::containsDisplayMode(
        const vector<D3DDISPLAYMODE>& modes,
        const D3DFORMAT format,
        const uint16_t w,
        const uint16_t h) const {

        for (const D3DDISPLAYMODE& mode : modes) {
          // LOOK THROUGH THE WHOLE VECTOR

          // IS IT THE ONE WE'RE LOOKING FOR?
          if ((mode.Format == format) && (mode.Width == w) && (mode.Height == h))
            return true;

        }
        return false;
      }

      /*
          createDirectXDeviceAndSpriteHandler - THIS METHOD CREATES OUR GPU AND
          SPRITE HANDLER (used for batch rendering textures) USING THE COLOR
          FORMAT AND SCREEN RESOLUTION OF OUR CHOICE.
          */
      HRESULT DirectXGraphics::createDirectXDeviceAndSpriteHandler() {
        HRESULT result;
        GameText& text = game->getText();

        // CREATE OUR GPU
        result = d3d->CreateDevice(
          D3DADAPTER_DEFAULT,
          D3DDEVTYPE_HAL,
          presentParameters.hDeviceWindow,
          D3DCREATE_HARDWARE_VERTEXPROCESSING,
          &presentParameters,
          &graphicsDevice
          );

        // IF GPU CREATION WAS SUCCESSFUL
        if (SUCCEEDED(result)) {
#ifdef DEBUG
          cout << "Acquired GPU" << endl;
#endif

          // CREATE OUR SPRITE HANDLER
          result = D3DXCreateSprite(graphicsDevice, &spriteHandler);
          if (SUCCEEDED(result)) {
#ifdef DEBUG
            cout << "D3DXCreateSprite(): SUCCEEDED" << endl;
#endif
          }
          else {
#ifdef DEBUG
            cout << "D3DXCreateSprite(): FAILED" << endl;
#endif
          }

          this->initTextFont(this->fontSize);
        }
        else {
#ifdef DEBUG
          cout << "Failed to acquire GPU" << endl;
#endif
        }

        return result;
      }

      /*
          createTextureManager - This method constructs a technology-specific
          TextureManager. Since this DirectXGraphics class uses the DirectX
          library, this method creates a DirectXTextureManager.
          */
      TextureManager* DirectXGraphics::createTextureManager() {
        return new DirectXTextureManager(this);
      }

      /*
          endDirectXFrameRendering - This method should be called after rendering a frame
          so that we can display what we've drawn on the GPU onto the monitor. It also
          releases a lock on the GPU so other threads may use it.
          */
      void DirectXGraphics::endDirectXFrameRendering() {
        // ALL DONE DRAWING ONTO THE GPU FOR THIS FRAME
        if (FAILED(graphicsDevice->EndScene())) {
#ifdef DEBUG
          cout << "graphicsDevice->EndScene(): FAILED" << endl;
#endif
        }

        // PUT WHAT WE JUST RENDERED ONTO THE SCREEN
        if (FAILED(graphicsDevice->Present(nullptr, nullptr, nullptr, nullptr))) {
#ifdef DEBUG
          cout << "graphicsDevice->Present(): FAILED" << endl;
#endif
        }
      }

      /*
          findAlternativeDisplayMode - If the player's GPU doesn't have the
          display mode we want this method can pick a new one. It does so
          by first seeing if there is another display mode with the resolution
          we want but a different color model. If found, we'll use it. If not it
          simply picks the largest one it can find. This method uses call-by-reference
          to set the formatToSet, screenWidthToSet, & screenHeightToSet parameters
          using the chosen display mode parameters.
          */
      void DirectXGraphics::findAlternativeDisplayMode(
        const vector<D3DDISPLAYMODE>& displayModes,
        D3DFORMAT& formatToSet) {
        // FIRST FIND ONE WITH THE PREFERRED SCREEN
        // DIMENSIONS, DEFAULT_SCREEN_HEIGHT &
        // DEFAULT_SCREEN_WIDTH, SINCE CHANGING A GUI TO
        // FIT DIFFERENT RESOLUTIONS IS PAINFUL
        bool defaultScreenSizeFound = false;
        for (const D3DDISPLAYMODE& testMode : displayModes) {
          if ((testMode.Width == screenWidth)
            && (testMode.Height == screenHeight)) {
            if (testMode.Format == DEFAULT_COLOR_FORMAT) {
              formatToSet = DEFAULT_COLOR_FORMAT;
              return;
            }
            else {
              defaultScreenSizeFound = true;
              formatToSet = testMode.Format;
            }
          }
        }

        if (defaultScreenSizeFound)
          return;

        // NONE WERE FOUND IN THE SCREEN SIZE WE WANT, SO
        // NOW JUST FIND THE LARGEST RESOLUTION AVAILABLE
        int totalPixels = 0;
        int maxPixels = 0;
        for (const D3DDISPLAYMODE& testMode : displayModes) {
          totalPixels = testMode.Width * testMode.Height;
          if (totalPixels > maxPixels) {
            formatToSet = testMode.Format;
            this->screenWidth = testMode.Width;
            this->screenHeight = testMode.Height;
            maxPixels = totalPixels;
          }
        }
      }

      /*
          getDirectXDisplayModes - This method queries the GPU and gets a vector
          of all the display modes available, returning this data structure.
          */
      vector<D3DDISPLAYMODE> DirectXGraphics::getDirectXDisplayModes() {
        // WE'LL FILL THIS WITH THE DISPLAY MODES WE FIND
        vector<D3DDISPLAYMODE> displayOptions;

        // WE'LL LOOK AT EACH COLOR MODEL, STARTING WITH ONE AT INDEX 1, IF THERE IS ONE
        for (int adapter = 1; adapter < 1000; ++adapter) {
          D3DFORMAT format = D3DFORMAT(adapter);

          // HOW MANY MODES HAVE THIS COLOR MODEL?
          int adapters = d3d->GetAdapterModeCount(D3DADAPTER_DEFAULT, format);

          // GET ALL FOR THIS COLOR MODEL
          for (int i = 0; i < adapters; i++) {
            D3DDISPLAYMODE mode;
            d3d->EnumAdapterModes(D3DADAPTER_DEFAULT, format, i, &mode);

            // PUT THEM INTO OUR VECTOR
            displayOptions.push_back(mode);
          }
        }
        return displayOptions;
      }

      /*
          init - This is the entry point for the application setting up the
          DirectX objects. It will get all available display modes and pick one,
          then use it to make a GPU device. Once this method is called, rendering
          can begin. It only needs to be called once at the start of the application.
          Even if we lose the graphics card (ALT-TAB), we don't have to re-init. We
          would have to re-init if we wished to change from fullscreen mode to
          windowed mode, or if we want to change the screen resolution or color model.
          */
      void DirectXGraphics::initGraphics(GameOS* os, const bool isFullscreen) {
        d3d = Direct3DCreate9(D3D_SDK_VERSION);

        // WHAT ARE THE CAPABILITIES OF THE PLAYER'S GPU?
        vector<D3DDISPLAYMODE> displayOptions = getDirectXDisplayModes();

        // THESE WILL MAKE UP OUR DESIRED DISPLAY MODE
        D3DFORMAT formatToUse;

        // DOES THE PLAYER HAVE OUR DESIRED FORMAT?
        if (containsDisplayMode(displayOptions,
          DEFAULT_COLOR_FORMAT,
          screenWidth,
          screenHeight)) {
          // THE GPU HAS OUR DESIRED FORMAT
          formatToUse = DEFAULT_COLOR_FORMAT;
        }
        else {
          // THE GPU DOESN'T HAVE OUR DESIRED FORMAT, WE NEED TO PICK ANOTHER ONE
          findAlternativeDisplayMode(displayOptions,
            formatToUse);
        }

        // WE NEED TO FILL OUT A D3DPRESENT_PARAMETERS STRUCTURE WITH OUR
        // PREFERENCES FOR CREATING OUR GPU DEVICE

        // FIRST MAKE SURE OUR STRUCTURE IS EMPTY
        ZeroMemory(&presentParameters, sizeof(presentParameters));

        // WINDOWED MODE OR FULLSCREEN?
        presentParameters.Windowed = !isFullscreen;

        // DISCARD OLD FRAMES
        presentParameters.SwapEffect = D3DSWAPEFFECT_FLIP;// D3DSWAPEFFECT_DISCARD;

        // THE WINDOW HANDLE
        HWND hWnd;

        // MAKE SURE WE'RE USING WINDOWS
        hWnd = static_cast<WindowsOS*>(os)->getWindowHandle();
        presentParameters.hDeviceWindow = hWnd;

        // THE DISPLAY MODE WE WILL BE USING
        presentParameters.BackBufferFormat = formatToUse;
        presentParameters.BackBufferWidth = screenWidth;
        presentParameters.BackBufferHeight = screenHeight;

        // OK, NOW WE CAN MAKE OUR GPU & SPRITE HANDLER.
        createDirectXDeviceAndSpriteHandler();
      }

      void DirectXGraphics::initTextFont(const uint16_t size) {
        this->fontSize = size;
        HRESULT result = D3DXCreateFont(
          graphicsDevice,             // OUR GPU
          fontSize,                   // EG FONT SIZE FOR HEIGHT
          0,                          // 0 FOR FONT WIDTH, USE DEFAULT ADJUST BASED ON HEIGHT
          FW_BOLD,                    // FONT WEIGHT
          0,                          // MIP LEVELS
          FALSE,                      // ITALICIZED?
          DEFAULT_CHARSET,            // CHARACTER SET
          OUT_DEFAULT_PRECIS,         // RENDERING PRECISION
          DEFAULT_QUALITY,            // RENDERING QUALITY
          DEFAULT_PITCH | FF_DONTCARE,  // FONT FAMILY NAME
          L"Arial",                   // FONT FACE NAME
          &textFont);                 // THE FONT WE ARE CREATING
      }

      void DirectXGraphics::reloadGraphics() {
#ifdef DEBUG
        cout << "Reloading DirectX graphics resources" << endl;
#endif
        createDirectXDeviceAndSpriteHandler();
        initTextFont(this->fontSize);
        guiTextureManager->reloadAllTextures();
        worldTextureManager->reloadAllTextures();
      }

      /*
          renderDirectXRenderList - This method renders a render list of game
          elements to the screen. It can process render lists for the game
          world or the gui. Note that GUI render lists use screen coordinates
          and so don't have to be offset, but game world lists use world
          coordinates, and so they will need to be offset.
          */
      void DirectXGraphics::renderGUIRenderList() {
        DirectXTextureManager* manager =
          static_cast<DirectXTextureManager*>(guiTextureManager);

        // GO THROUGH EACH ITEM IN THE LIST
        for (const RenderItem& item : guiRenderList) {
          // LET'S GET THE TEXTURE WE WANT TO RENDER
          IDirect3DTexture9* texture = manager->getTexture(item.id);
          D3DXVECTOR3 position(item.x, item.y, 0);

          if (FAILED(spriteHandler->Draw(
            texture,
            nullptr,
            nullptr,
            &position,
            D3DCOLOR_ARGB(item.a, 255, 255, 255)))) {
#ifdef DEBUG
            cout << "spriteHandler->Draw(): FAILED (texture ID " <<
              item.id << ')' << endl;
#endif
          }
        }

        // NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
        guiRenderList.clear();
      }

      void DirectXGraphics::renderWorldRenderList() {
        gui::GameGUI& gui = game->getGUI();
        gui::Viewport& viewport = gui.getViewport();
        //RECT rect{ 0, 0, 0, 0 };
        DirectXTextureManager* manager =
          static_cast<DirectXTextureManager*>(worldTextureManager);

        // GO THROUGH EACH ITEM IN THE LIST
        for (const RenderItem& item : worldRenderList) {
          // LET'S GET THE TEXTURE WE WANT TO RENDER
          IDirect3DTexture9* texture = manager->getTexture(item.id);
          D3DXVECTOR3 position(item.x, item.y, 0);
          position.x += viewport.getViewportOffsetX();
          position.y += viewport.getViewportOffsetY();

          // ADJUST FOR THE GUI OFFSET
          if ((position.x < viewport.getViewportOffsetX())
            || (position.y < viewport.getViewportOffsetY())) {
            /*
            IDirect3DSurface9* surface = nullptr;
            HRESULT result = texture->GetSurfaceLevel(0, &surface);
            D3DSURFACE_DESC surfaceDescription;
            surface->GetDesc(&surfaceDescription);
            rect.left = 0;
            rect.top = 0;
            rect.right = surfaceDescription.Width;
            rect.bottom = surfaceDescription.Height;
            */
            if (position.x < viewport.getViewportOffsetX()) {
              int dx = viewport.getViewportOffsetX() - item.x;
              //rect.left = dx;
              position.x += dx;
            }
            if (position.y < viewport.getViewportOffsetY()) {
              int dy = viewport.getViewportOffsetY() - item.y;
              //rect.top = dy;
              position.y += dy;
            }
          }

          if (FAILED(spriteHandler->Draw(
            texture,
            nullptr, // &rect
            nullptr,
            &position,
            D3DCOLOR_ARGB(item.a, 255, 255, 255)))) {
#ifdef DEBUG
            cout << "spriteHandler->Draw(): FAILED (texture ID " <<
              item.id << ')' << endl;
#endif
          }
        }

        // NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
        worldRenderList.clear();
      }

      void DirectXGraphics::renderGame(Game* game) {
        gsm::state::GameStateManager& gsm = game->getGSM();
        gsm::world::World& world = gsm.getWorld();
        GameText& text = game->getText();

        // CHECK TO SEE IF WE STILL HAVE THE GPU
        HRESULT result = graphicsDevice->TestCooperativeLevel();

        // IF WE HAVE THE GPU, RENDER THE GAME
        if (SUCCEEDED(result)) {
          // NOW PREPARE TO RENDER THE LISTS USING
          // BATCH TEXTURE RENDERING
          startDirectXFrameRendering();
          spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);
          // RENDER THE WORLD RENDER LIST
          renderWorldRenderList();

          // RENDER THE GUI RENDER LIST
          renderGUIRenderList();

          // RENDER THE TEXT
          renderText(text);

          // WRAP UP RENDERING RESOURCES
          if (FAILED(spriteHandler->End())) {
#ifdef DEBUG
            cout << "spriteHandler->End(): FAILED" << endl;
#endif
          }

          endDirectXFrameRendering();
        }
        else if (result == D3DERR_DEVICELOST) {
          // WE'VE LOST THE GPU, SLEEP UNTIL WE GET IT BACK
          spriteHandler->OnLostDevice();
          textFont->OnLostDevice();
          Sleep(100);
        }
        else if (result == D3DERR_DEVICENOTRESET) {
          // WE'VE GOT IT BACK, RELOAD EVERYTHING. NOTE THAT
          // WE'LL ONLY GET THIS MESSAGE ONCE.
          if (FAILED(graphicsDevice->Reset(&presentParameters))) {
#ifdef DEBUG
            cout << "graphicsDevice->Reset(): FAILED - Reloading GPU images" << endl;
#endif
            game->reloadAllDevices();
          }
          else {
            spriteHandler->OnResetDevice();
            textFont->OnResetDevice();
          }
        }
      }

      void DirectXGraphics::renderTextToDraw(const TextToDraw& textToDraw) {
        textRect.left = textToDraw.x;
        textRect.right = textToDraw.x + textToDraw.width;
        textRect.top = textToDraw.y;
        textRect.bottom = textToDraw.y + textToDraw.height;
        const wchar_t* text = textToDraw.getText()->c_str();
        if (FAILED(textFont->DrawText(
          spriteHandler,
          text,
          -1,
          &textRect,
          DT_LEFT,
          fontColor))) {
#ifdef DEBUG
          cout << "textFont->DrawText(): FAILED" << endl;
#endif
        }
      }

      /*
          setColorKey - This sets the color key to be used for loading images.
          */
      void DirectXGraphics::setColorKey(const uint8_t r, const uint8_t g, const uint8_t b) {
        colorKey = D3DCOLOR_XRGB(r, g, b);
      }

      /*
          setFontColor - This sets the color to be used for rendering text.
          */
      void DirectXGraphics::setFontColor(const uint8_t r, const uint8_t g, const uint8_t b) {
        fontColor = D3DCOLOR_XRGB(r, g, b);
      }
      // penis penis I wonder how long until somebody sees this penis penis -- JTG

      /*
          shutdownGraphics - This method releases the DirectX objects we've created
          so that other applications can use the GPU. This should only be called
          when the application is closing.
          */
      void DirectXGraphics::shutdown() {
        if (textFont) {
          textFont->Release();
          textFont = nullptr;
        }

        if (spriteHandler) {
          spriteHandler->Release();
          spriteHandler = nullptr;
        }

        if (graphicsDevice) {
          graphicsDevice->Release();
          graphicsDevice = nullptr;
        }

        if (d3d) {
          d3d->Release();
          d3d = nullptr;
        }
#ifdef DEBUG
        cout << "Released DirectX graphics resources" << endl;
#endif
      }

      /*
          startDirectXFrameRendering - This does some setup for rendering, like locking
          the GPU. Only one thread at a time can have a lock on the GPU.
          */
      void DirectXGraphics::startDirectXFrameRendering() {
        // CLEAR OUT ALL THE OLD RENDERING
        if (FAILED(graphicsDevice->Clear(0, nullptr, D3DCLEAR_TARGET, BACKGROUND_COLOR,
          1.0f, 0))) {
#ifdef DEBUG
          cout << "graphicsDevice()->Clear(): FAILED" << endl;
#endif
        }

        // ALLOWS DRAWING TO START, LOCKING THE GPU
        if (FAILED(graphicsDevice->BeginScene())) {
#ifdef DEBUG
          cout << "graphicsDevice()->BeginScene(): FAILED" << endl;
#endif
        }
      }
    }
  }
}
