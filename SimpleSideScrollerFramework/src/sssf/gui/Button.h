/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    Button.h
    */

#pragma once

#include <cstdint>
#include <string>

#include "sssf/input/ButtonEventHandler.h"

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }

    namespace gui {
      using std::wstring;

      /// <summary>
      /// <para>
      /// Represents a single button in a game GUI. In addition to storing
      /// information about it's presentation, like the x, y location, width and
      /// height, and image id from the gui texture manager, it also stores a
      /// pointer to the event handler that will be used when this button is
      /// clicked. This is the button's ButtonEventHandler.
      /// </para>
      /// <para>
      /// Buttons can have either 1 or 2 images. The two images being a normal
      /// image and a mouse over image. If only one image id is provided, it is
      /// used for both states.
      /// </para>
      /// </summary>
      class Button {
      public:
        Button(
          const size_t,
          const size_t,
          const int,
          const int,
          const int16_t,
          const uint8_t,
          const uint16_t,
          const uint16_t,
          const bool,
          const wstring&,
          input::ButtonEventHandler* handler = nullptr
          );

        // WE ARE NOT DELETING THE EVENT HANDLER BECAUSE IT
        // MAY BE SHARED BY MULTIPLE OBJECTS. WHEN THE APPLICATION
        // CLOSES IT SHOULD BE DELETED BY THE GAME
        ~Button() {}

        // INLINED ACCESSOR METHODS
        size_t getNormalTextureID() const { return normalTextureID; }
        size_t getMouseOverTextureID() const { return mouseOverTextureID; }
        size_t getCurrentTextureID() const {
          return mouseOver ? mouseOverTextureID : normalTextureID;
        }

        int getX() const { return x; }
        int getY() const { return y; }
        int16_t getZ() const { return z; }
        uint8_t getAlpha() const { return alpha; }
        uint16_t getHeight() const { return height; }
        uint16_t getWidth() const { return width; }
        bool isMouseOver() const { return mouseOver; }
        const wstring& getCommand() const { return command; }

        // INLINED MUTATOR METHODS
        void setNormalTextureID(const size_t id) { normalTextureID = id; }
        void setMouseOverTextureID(const size_t id) { mouseOverTextureID = id; }

        void setX(const int initX) { x = initX; }
        void setY(const int initY) { y = initY; }
        void setZ(const int16_t initZ) { z = initZ; }
        void setAlpha(const uint8_t initAlpha) { alpha = initAlpha; }
        void setWidth(uint16_t initWidth) { width = initWidth; }
        void setHeight(uint16_t initHeight) { height = initHeight; }
        void setCommand(const wstring& cmd) { command = cmd; }
        void setEventHandler(input::ButtonEventHandler* e) { eventHandler = e; }

        // METHODS DEFINED IN Button.cpp


        /// The Button has been clicked, tell the eventHandler to respond.
        void fireEvent(game::Game* game);

        /**
         * Checks to see if the provided mousePoint argument is over the button.
         * If it is, it sets the mouseOver instance variable to true and returns
         * true. Otherwise, it sets the mouseOver instance variable to false and
         * returns false.
         *
         * Note that this should only be called once per frame. If needed more
         * than once per frame for this button, just call it once, and then call
         * isMouseOver thereafter. isMouseOver is a simple accessor method that
         * returns the instance variable set by this method.
         */
        bool updateMouseOver(long mouseX, long mouseY);
      private:
        // INDEX IN THE GUI TEXTURE MANAGER OF THE NORMAL BUTTON IMAGE
        unsigned int normalTextureID;

        // INDEX IN THE GUI TEXTURE MANAGER OF THE MOUSE-OVER BUTTON IMAGE
        unsigned int mouseOverTextureID;

        // OTHER RENDERING VARIABLES
        int x;
        int y;
        int16_t z;
        uint16_t width;
        uint16_t height;
        uint8_t alpha;
        bool mouseOver;

        // EACH BUTTON HAS A command, AND WHEN WE DETECT A MOUSE CLICK ON THE
        // BUTTON, WE SEND THE command TO THE eventHandler, WHICH PROVIDES
        // A PROGRAMMED RESPONSE
        wstring command;
        input::ButtonEventHandler* eventHandler;
      };
    }
  }
}
