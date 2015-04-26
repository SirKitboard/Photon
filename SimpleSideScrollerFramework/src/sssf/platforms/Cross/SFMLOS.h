#ifndef SFMLOS_H
#define SFMLOS_H

#include <cstdint>
#include <string>

#include "sssf/os/GameOS.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
    namespace sfml {
      using std::wstring;

      /// <summary>
      /// <para>
      /// Calling this class "SFMLOS" is a bit of a misnomer.  I'd personally call it
      /// SFMLSystem (and likewise for WindowsOS).  Such is life.
      /// </para>
      /// <para>
      /// Most of the window control stuff you need can be done through the
      /// sf::RenderWindow instance.  The accessor returns a reference, so you can't
      /// store it locally as a sf::RenderWindow, it would have to be a sf::RenderWindow&amp;.
      /// </para>
      /// <author>Jesse Talavera-Greenberg</author>
      /// </summary>
      class SFMLOS : public os::GameOS {
      public:
        SFMLOS(
          const sf::VideoMode&,
          const wstring&,
          const unsigned int,
          game::Game*,
          const sf::ContextSettings& = sf::ContextSettings()
          );
        virtual ~SFMLOS() {}

        void processOSMessages() override;
        void shutdown() override;

        void setApplicationName(const wstring& initApplicationName) override;

        bool isFullscreen() const override { return this->_style & sf::Style::Fullscreen; }
        sf::RenderWindow& getWindow() { return this->_window; }
        const wstring& getApplicationName() const { return this->_name; }
        unsigned int getWindowStyle() const { return this->_style; }
      private:
        wstring _name;
        unsigned int _style;
        sf::RenderWindow _window;
        game::Game* _game;
        // You should only use raw pointers to refer to existing objects whose
        // lifetime is not the pointer-user's responsibility
      };
    }
  }
}

#endif // SFMLOS_H
