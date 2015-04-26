#ifndef SFMLGRAPHICS_H
#define SFMLGRAPHICS_H

#include "sssf/graphics/GameGraphics.h"

#include <SFML/Graphics.hpp>

const int DEFAULT_BPP = 32;

namespace cse380 {
  namespace sssf {

    namespace game { class Game; }
    namespace sfml {

      using namespace graphics;

      class SFMLGraphics : public GameGraphics {
      public:
        SFMLGraphics(Game*);
        virtual ~SFMLGraphics() {}
        void initGraphics(GameOS* os, const bool) override;
        void initTextFont(const uint16_t) override;
        void reloadGraphics() override;
        void renderGame(Game* game) override;
        void renderTextToDraw(const TextToDraw&) override;
        void setColorKey(const uint8_t, const uint8_t, const uint8_t) override;
        void setFontColor(const uint8_t, const uint8_t, const uint8_t) override;
        void shutdown() override;
		void renderLight();
        const sf::Color& getColorKey() const { return this->_colorKey; }
        const sf::Color& getFontColor() const { return this->_fontColor; }
      protected:
        TextureManager* createTextureManager() override;
      private:
        void renderGUIRenderList();
        void renderWorldRenderList();

        sf::Color _colorKey;
        sf::Color _fontColor;
        sf::Font _font;
        sf::Text _text;
        int _fontSize;
        bool _fontLoaded;
      };
    }
  }
}
#endif // SFMLGRAPHICS_H
