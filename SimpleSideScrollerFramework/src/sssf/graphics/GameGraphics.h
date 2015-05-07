/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department
    */

#pragma once

#include <cstdint>

#include "sssf/graphics/RenderList.h"

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }
    namespace os { class GameOS; }
    namespace text {
      class GameText;
      class TextToDraw;
    }

    namespace graphics {

      using game::Game;
      using os::GameOS;
      using text::TextToDraw;
      using text::GameText;

      class TextureManager;

      // Provides a framework for technology-specific classes that will manage
      // the rendering of all game textures and text. It provides methods for
      // the initialization of objects necessary for rendering images and text.
      //
      // Rendering should be done by generating a list of RenderItem objects
      // each frame and putting them into a list. These objects store information
      // about the textures that have to be drawn to the screen.
      class GameGraphics {
      public:
        // CONCRETE METHODS DEFINED IN GameGraphics.cpp
        GameGraphics();

        // Destructor, it cleans up the render lists and texture managers. This
        // will only be called when the application is closing.
        virtual ~GameGraphics();

        // Can't copy a GameGraphics
        GameGraphics& operator=(const GameGraphics&) = delete;
        GameGraphics(const GameGraphics&) = delete;

        // INLINED ACCESSOR METHODS
        RenderList& getGUIRenderList() { return guiRenderList; }
        const RenderList& getGUIRenderList() const { return guiRenderList; }

        TextureManager* getGUITextureManager() { return guiTextureManager; }
        const TextureManager* getGUITextureManager() const { return guiTextureManager; }

        RenderList& getWorldRenderList() { return worldRenderList; }
        const RenderList& getWorldRenderList() const { return worldRenderList; }

		RenderList& getLightRenderList() { return lights; }
		const RenderList& getLightRenderList() const { return lights; }

        TextureManager* getWorldTextureManager() { return worldTextureManager; }
        const TextureManager* getWorldTextureManager() const { return worldTextureManager; }

		RenderList& getTileRenderList() { return tiles; }
		const RenderList& getTileRenderList() const { return tiles; }

        // AND A METHOD FOR TURNING DEBUG TEXT ON AND OFF
        void toggleDebugTextShouldBeRendered() {
          debugTextShouldBeRendered = !debugTextShouldBeRendered;
        }

        uint16_t getScreenHeight() const { return screenWidth; }
        uint16_t getScreenWidth() const { return screenHeight; }
        // VIRTUAL METHODS TO BE OVERRIDDEN BY TECHNOLOGY
        // SPECIFIC CLASSES

        virtual void initGraphics(GameOS*, const bool) = 0;

        /// <summary>
        /// Initializes our font object, which is necessary for all text
        /// rendering. Only needs to be called start of the application unless
        /// we want to change the font we're using.
        /// </summary>
        virtual void initTextFont(const uint16_t) = 0;

        /// <summary>
        /// Reacquires the GPU and sprite handler, then reloads all the textures
        /// in the current texture managers. Call this after regaining the GPU.
        /// </summary>
        virtual void reloadGraphics() = 0;
        virtual void renderGame(Game* game) = 0;

        // Renders a single piece of text to the screen using our font.
        virtual void renderTextToDraw(const TextToDraw&) = 0;
        virtual void setColorKey(const uint8_t, const uint8_t, const uint8_t) = 0;
        virtual void setFontColor(const uint8_t, const uint8_t, const uint8_t) = 0;
        virtual void shutdown() = 0;

        // When the game leaves a level we have to clear out these data structures.
        // Calling clear on these will delete all the objects inside.
        void clearWorldTextures();

        // Fills the render lists with the things that must be drawn this frame.
        void fillRenderLists(Game* game);

        /// <summary>
        /// Constructs the data structures for managing textures and render lists.
        /// Calls createTextureManager, which is technology specific and thus
        /// implemented only by child classes.
        /// </summary>
        void init(const uint16_t screenWidth, const uint16_t screenHeight);

        /**
         * Goes through the GameText argument, pulls out each TextToDraw object,
         * and renders each piece of text.
         */
        void renderText(const GameText& text);
      protected:
        // Constructs a technology-specific TextureManager.
        virtual TextureManager* createTextureManager() = 0;

        // WE PERIODICALLY NEED ACCESS TO GAME STUFF
        Game* game;

        // WE'LL SORE THE SCREEN DIMENSIONS HERE
        uint16_t screenWidth;
        uint16_t screenHeight;
        uint16_t fontSize;

        // EACH FRAME, RENDER LISTS ARE CREATED, FILLED WITH
        // DATA, THEN USED FOR RENDERING THE FRAME, AND DESTROYED
        RenderList guiRenderList;
        RenderList worldRenderList;
		RenderList lights;
		RenderList tiles;

        // NOTE THAT IF WE REALLY WANTED TO BE SLICK, WE COULD PROVIDE
        // MULTIPLE WORLD RENDER LISTS, ONE FOR EACH Z-LEVEL

        // THE TextureManagers STORE ALL THE IMAGES FOR OUR GAME
        TextureManager* guiTextureManager;
        TextureManager* worldTextureManager;

        // TOGGLES WHETHER OR NOT TO RENDER THE TEXT, WHICH IS USEFUL
        // FOR DISPLAYING DEBUG INFORMATION
        bool debugTextShouldBeRendered;
      };
    }
  }
}
