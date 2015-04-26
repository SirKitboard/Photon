#include "sssf_VS/stdafx.h"
#include "sssf/platforms/Cross/SFMLOS.h"

#include "sssf/game/Game.h"

namespace cse380 {
  namespace sssf {
    namespace sfml {
      using std::wstring;

      SFMLOS::SFMLOS(
        const sf::VideoMode& mode,
        const wstring& title,
        const unsigned int style,
        game::Game* game,
        const sf::ContextSettings& settings
        ) : _window(mode, title, style, settings) {
        assert(game != nullptr);

        this->_name = title;
        this->_style = style;
        this->_game = game;
      }

      void SFMLOS::setApplicationName(const wstring& title) {
        this->_name = title;
        this->_window.setTitle(title);
      }

      void SFMLOS::processOSMessages() {
        for (sf::Event event; this->_window.pollEvent(event);) {
          switch (event.type) {
          case sf::Event::Closed:
            this->_game->shutdown();
            exit(0);
          case sf::Event::Resized:
          case sf::Event::MouseEntered:
            this->_window.setMouseCursorVisible(false);
          }
        }
      }

      void SFMLOS::shutdown() {
        this->_window.close();
      }
    }
  }
}
