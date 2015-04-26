#ifndef SFMLINPUT_H
#define SFMLINPUT_H

#include "sssf/input/GameInput.h"

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }
    namespace gui { class Cursor; }

    namespace sfml {
      using namespace input;
      using game::Game;

      class SFMLInput : public GameInput {
      public:
        SFMLInput(Game*);
        virtual ~SFMLInput();
        void updateInputState() override;
        void processInput(Game*) override;
        void respondToMouseInput(Game*) override;
        void shutdown() override;
        const sf::Vector2i& getMousePoint() const {
          return this->_mouse;
        }
      private:
        void updateCursorPosition(Game* game, gui::Cursor* cursor);
        sf::Vector2i _mouse;
        Game* _game;
      };
    }
  }
}

#endif // SFMLINPUT_H
