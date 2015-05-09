#include "sssf_VS/stdafx.h"
#include "sssf/platforms/Cross/SFMLInput.h"

#include "sssf/gui/GameGUI.h"
#include "sssf/game/Game.h"
#include "sssf/gui/Cursor.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/platforms/Cross/SFMLOS.h"
#include "sssf/util/Util.h"

namespace cse380 {
  namespace sssf {
    namespace sfml {

      using gsm::state::GameStateManager;
      using gui::Cursor;
      using gui::GameGUI;
      using graphics::GameGraphics;

      SFMLInput::SFMLInput(Game* game) : _mouse(0, 0), _game(game) {}

      SFMLInput::~SFMLInput() {
        //dtor
      }

      void SFMLInput::updateInputState() {
        for (int i = 0; i < inputState.size(); i++) {
          if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(i))) {
            // FILL IN isPressed FOR THOSE PRESSED
            // then UPDATE wasHeldDown & isFirstPress
            inputState[i].isPressed = true;

            if (inputState[i].wasHeldDown) {
              inputState[i].isFirstPress = false;
            }
            else {
              inputState[i].wasHeldDown = true;
              inputState[i].isFirstPress = true;
            }
          }
          else {
            // RESET isPressed FOR ALL KEYS (if they're not pressed)
            inputState[i].isPressed = false;
            inputState[i].wasHeldDown = false;
            inputState[i].isFirstPress = false;
          }
        }
      }

      void SFMLInput::processInput(Game*) {
        //SFMLOS* os = static_cast<SFMLOS*>(this->_game->getOS());
        updateCursorPosition(_game, _game->getGUI().getCursor());
        this->updateInputState();
        this->respondToKeyboardInput(this->_game);
        this->respondToMouseInput(this->_game);
        // I dunno why these methods have to take in a Game* when I can just pass
        // it into the constructor
        // -JTG
      }

      void SFMLInput::respondToMouseInput(Game*) {
        GameGUI& gui = _game->getGUI();
        GameStateManager& gsm = _game->getGSM();
        gui.updateGUIState(_mouse.x, _mouse.y, gsm.getCurrentGameState());

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          gui.checkCurrentScreenForAction(_game);
        }
      }
      void SFMLInput::shutdown() {
        // IF WE WERE USING ANOTHER LIBRARY, WE MAY NEED TO SHUT THINGS DOWN
      }

      void SFMLInput::updateCursorPosition(Game* game, Cursor* cursor) {
        GameGraphics* graphics = game->getGraphics();

        SFMLOS* os = static_cast<SFMLOS*>(game->getOS());
        sf::RenderWindow& window = os->getWindow();
        this->_mouse = sf::Mouse::getPosition(window);
        
        _mouse.x = util::clamp(_mouse.x, 0, graphics->getScreenWidth() - 1);
        _mouse.y = util::clamp(_mouse.y, 0, graphics->getScreenHeight() - 1);

        //scursor->setPosition(_mouse.x, _mouse.y);
      }
    }
  }
}
