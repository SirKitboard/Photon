/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    ScreenGUI.cpp

    See ScreenGUI.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gui/ScreenGUI.h"
#include "sssf/game/Game.h"
#include "sssf/graphics/RenderList.h"
#include "sssf/gui/Button.h"
#include "sssf/gui/OverlayImage.h"

namespace cse380 {
  namespace sssf {
    namespace gui {
      using input::ButtonEventHandler;

      ScreenGUI::ScreenGUI(const wstring& name) : screenName(name) {}

      ScreenGUI::~ScreenGUI() {
        for (Button* button : this->buttons) {
          delete button;
        }
      }

      void ScreenGUI::addButton(Button* buttonToAdd) {
        buttons.push_back(buttonToAdd);
      }

      void ScreenGUI::addOverlayImage(const OverlayImage& imageToAdd) {
        overlayImages.push_back(imageToAdd);
      }

      void ScreenGUI::addRenderItemsToRenderList(graphics::RenderList& renderList) const {
        // FIRST ADD THE OVERLAY IMAGES TO THE RENDER LIST
        for (const OverlayImage& image : overlayImages) {
          // ADD IT TO THE LIST
          renderList.addRenderItem(image.imageID,
            image.x,
            image.y,
            image.z,
            image.alpha,
            image.width,
            image.height,
			0);

        }

        for (const Button* buttonToRender : buttons) {
          // DETERMINE WHICH Button IMAGE TO USE

          // ADD IT TO THE LIST
          renderList.addRenderItem(
            buttonToRender->getCurrentTextureID(),
            buttonToRender->getX(),
            buttonToRender->getY(),
            buttonToRender->getZ(),
            buttonToRender->getAlpha(),
            buttonToRender->getWidth(),
            buttonToRender->getHeight(),
			0);

        }
      }

      bool ScreenGUI::fireButtonCommand(game::Game* game) {
        // GO THROUGH ALL THE BUTTONS IF NECESSARY
        for (Button* buttonToTest : buttons) {

          // IF THE CURSOR IS OVER THE BUTTON, WE'VE FOUND THE BUTTON THAT WAS CLICKED
          if (buttonToTest->isMouseOver()) {
            // WE FOUND THE CURSOR WAS OVER A BUTTON, FIRE THE EVENT
            buttonToTest->fireEvent(game);
            // THIS WILL END THE LOOP LOOKING FOR THE BUTTON
            return true;
          }
        }

        // NO EVENTS FIRED, THE CURSOR WAS NOT OVER A BUTTON WHEN IT WAS PRESSED
        return false;
      }

      void ScreenGUI::registerButtonEventHandler(ButtonEventHandler* eventHandler) {
        for (Button* buttonToSet : buttons) {
          // LET THE BUTTON HAVE THE EVENT HANDLER
          buttonToSet->setEventHandler(eventHandler);
        }
      }

      void ScreenGUI::updateAllButtons(long mouseX, long mouseY) {
        for (Button* buttonToUpdate : buttons) {
          // UPDATE ALL BUTTONS IN THIS SCREEN SO THEY KNOW IF THE CURSOR IS
          // CURRENTLY OVER THEM OR NOT
          buttonToUpdate->updateMouseOver(mouseX, mouseY);
        }
      }
    }
  }
}
