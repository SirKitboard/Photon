/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    ScreenGUI.h
    */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "sssf/gui/OverlayImage.h"
#include "sssf/gui/Button.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace graphics { class RenderList; }
    namespace input { class ButtonEventHandler; }

    namespace gui {
      using std::wstring;
      using std::vector;

      /**
      Represents a single screen GUI, like the main menu. In a game application,
      there is always a GUI of some sort, whether it is application controls or
      game controls. So even while the game is being played, we can interact with
      a different GUI.

      This class manages a single GUI, and a game application may have many. Each
      GUI can have many Buttons, many OverlayImages, and a screen name.
      */
      class ScreenGUI {
      public:
        /**
         Initializes the data structures for storing the GUI Buttons and
         Overlay Images.
         */
        ScreenGUI(const wstring& name);
        ScreenGUI() : ScreenGUI(L"") {}

        // No copying allowed
        ScreenGUI(const ScreenGUI&) = delete;
        ScreenGUI& operator=(const ScreenGUI&) = delete;

        /**
          This destructor cleans up all the Buttons, OverlayImages, and their
          related memory.
          */
        ~ScreenGUI();

        // INLINED ACCESSOR METHODS

        size_t getNumButtons() const { return buttons.size(); }
        size_t getNumOverlayImages() const { return overlayImages.size(); }
        const wstring& getScreenName() const { return screenName; }

        // INLINED MUTATOR METHODS
        void setScreenName(const wstring& name) { screenName = name; }

        // METHODS DEFINED IN ScreenGUI.cpp

        /**
         Adds a contructed Button to this GUI screen.
         */
        void addButton(Button* buttonToAdd);

        /**
         * Adds a constructured OverlayImage to this GUI screen.
         */
        void addOverlayImage(const OverlayImage& imageToAdd);

        /**
          This method should be called once per frame on this screen if it's
          currently active. It goes through all the GUI elements, the buttons
          and the overlay images, and for each it generates a RenderItem and
          adds it to the list.
          */
        void addRenderItemsToRenderList(graphics::RenderList& renderList) const;

        /**
         * This method should be called when a mouse button is pressed. It will
         * test to see if any button events should be fired. This is done by
         * going through the buttons and seeing if the cursor is currently over
         * any of them. If the cursor is over a button, the registered button
         * event handler is fired, and the button itself will send it the
         * appropriate action command.
         */
        bool fireButtonCommand(game::Game* game);

        /**
         * Lets all the buttons in this ScreenGUI know the object that will
         * handle their events, should they be clicked. This method only needs
         * to be callled once for each screen gui, when it is being loaded.
         */
        void registerButtonEventHandler(input::ButtonEventHandler* eventHandler);

        /**
         * This method is given mousePoint, which is the current x,y location of
         * the cursor, and it goes through all of the buttons in this screen,
         * determining for each if the cursor is currently over it. A button
         * will record whether the cursor is over it or not, such that it will
         * know which image to render (mouse over or normal). In addition, if
         * the mouse button is clicked, all buttons will know if the mouse is
         * currently over them or not.
         *
         * This method should be called once per frame for the currently visible
         * ScreenGUI such that the buttons are rendered properly and so that
         * button clicks fire events.
         */
        void updateAllButtons(long mouseX, long mouseY);
      private:
        // THE BUTTONS FOR THIS SCREEN
        vector<Button*> buttons;

        // ADDITIONAL IMAGES FOR THIS SCREEN, LIKE BORDERS
        vector<OverlayImage> overlayImages;

        // SCREEN NAME, LIKE "MAIN MENU"
        wstring screenName;
      };
    }
  }
}
