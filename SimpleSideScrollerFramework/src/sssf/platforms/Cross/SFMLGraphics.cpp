#include "sssf_VS/stdafx.h"
#include "sssf/platforms/Cross/SFMLGraphics.h"

#include "sssf/graphics/TextureManager.h"
#include "sssf/platforms/Cross/SFMLTextureManager.h"
#include "sssf/platforms/Cross/SFMLOS.h"
#include "sssf/game/Game.h"
#include "sssf/gsm/world/World.h"
#include "sssf/gsm/state/GameStateManager.h"

namespace cse380 {
  namespace sssf {
    namespace sfml {
      // THIS IS THE COLOR WE WILL USE TO CLEAR THE SCREEN WITH
      const sf::Color BACKGROUND_COLOR(100,100,100);

      // BY DEFAULT, WE WILL USE THIS FOR TEXTURE DRAWING, IT USES NO TRANSPARENCY
      const sf::Color DEFAULT_ALPHA_COLOR(255, 255, 255, 255);

      using std::vector;
      using sf::VideoMode;
      using gsm::state::GameStateManager;
      using gsm::world::World;
      using gui::GameGUI;
      using gui::Viewport;


      SFMLGraphics::SFMLGraphics(Game* initGame) {
        this->game = initGame;
        this->_fontLoaded = false;
      }

      TextureManager* SFMLGraphics::createTextureManager() {
        return new SFMLTextureManager(this);
      }

      void SFMLGraphics::initGraphics(GameOS* os, const bool isFullscreen) {
        // WHAT ARE THE CAPABILITIES OF THE PLAYER'S GPU?
        SFMLOS* sfmlos = static_cast<SFMLOS*>(os);
        sf::RenderWindow& window = sfmlos->getWindow();

        // THESE WILL MAKE UP OUR DESIRED DISPLAY MODE
        VideoMode formatToUse(this->screenWidth, this->screenHeight, DEFAULT_BPP);

        const vector<VideoMode>& modes = VideoMode::getFullscreenModes();

        if (isFullscreen && !formatToUse.isValid()) {
          // DOES THE PLAYER HAVE OUR DESIRED FORMAT?
          for (const sf::VideoMode& mode : modes) {
            // THE GPU DOESN'T HAVE OUR DESIRED FORMAT, WE NEED TO PICK ANOTHER ONE
            formatToUse = mode;

            if (formatToUse.isValid()) break;
          }
        }

        sf::Vector2u size = window.getSize();
        this->screenWidth = size.x;
        this->screenHeight = size.y;
      }

      void SFMLGraphics::initTextFont(const uint16_t size) {
        if (!this->_fontLoaded) {
          this->_fontLoaded = this->_font.loadFromFile("data/font.ttf");
          this->_text.setFont(this->_font);
          this->_fontSize = size;
          this->_text.setCharacterSize(size);
        }
      }

      void SFMLGraphics::reloadGraphics() {
        initTextFont(this->_fontSize);
        guiTextureManager->reloadAllTextures();
        worldTextureManager->reloadAllTextures();
      }

      void SFMLGraphics::renderGame(Game* game) {
        GameStateManager& gsm = game->getGSM();
        World& world = gsm.getWorld();
        GameText& text = game->getText();

        SFMLOS* os = static_cast<SFMLOS*>(game->getOS());
        sf::RenderWindow& window = os->getWindow();

        window.clear(BACKGROUND_COLOR);
		renderLight();
        // RENDER THE WORLD RENDER LIST
        renderWorldRenderList();
		

        // RENDER THE GUI RENDER LIST
        renderGUIRenderList();

        // RENDER THE TEXT
        renderText(text);

        window.display();
      }

      void SFMLGraphics::renderTextToDraw(const TextToDraw& textToDraw) {
        _text.setString(*(textToDraw.getText()));
        _text.setPosition(textToDraw.x, textToDraw.y);
        SFMLOS* os = static_cast<SFMLOS*>(this->game->getOS());
        sf::RenderWindow& window = os->getWindow();

        window.draw(_text);
      }

	  void SFMLGraphics::renderLight() {
		  GameGUI& gui = game->getGUI();
		  const Viewport& viewport = gui.getViewport();
		  SFMLTextureManager* manager = static_cast<SFMLTextureManager*>(this->guiTextureManager);
		  SFMLOS* os = static_cast<SFMLOS*>(this->game->getOS());
		  sf::RenderWindow& window = os->getWindow();

		  // GO THROUGH EACH ITEM IN THE LIST
		  for (const RenderItem& itemToRender : lights) {
			  // LET'S GET THE TEXTURE WE WANT TO RENDER
			  sf::IntRect rect;
			  sf::Sprite sprite;
			  sf::Texture& texture = manager->getTexture(itemToRender.id);
			  sf::Vector2i position(itemToRender.x, itemToRender.y);
			  sf::BlendMode blend = sf::BlendMultiply;
			  blend.alphaDstFactor = sf::BlendMode::SrcAlpha;
			  blend.colorDstFactor = sf::BlendMode::SrcColor;
			  // sf::Transformable transformable(itemToRender.roation);

			  position.x += viewport.getViewportOffsetX();
			  position.y += viewport.getViewportOffsetY();

			  sf::Vector2u size = texture.getSize();
			  rect.left = 0;
			  rect.top = 0;
			  rect.width = size.x;
			  rect.height = size.y;

			  // ADJUST FOR THE GUI OFFSET
			  if (position.x < viewport.getViewportOffsetX()) {
				  int dx = viewport.getViewportOffsetX() - position.x;
				  rect.left = dx;
				  rect.width -= dx;
				  position.x += dx;
			  }

			  if (position.y < viewport.getViewportOffsetY()) {
				  int dy = viewport.getViewportOffsetY() - position.y;
				  rect.top = dy;
				  rect.height -= dy;
				  position.y += dy;
			  }
			  // This is a hackish form of clipping; when you start rotating and
			  // scaling objects, this *will* stop being useful.

			  sprite.setTexture(texture, true);
			  sprite.setTextureRect(rect);
			  sprite.setOrigin(0, 0);
			  sprite.setColor(sf::Color(255, 255, 255, itemToRender.a));
			  sprite.setPosition(position.x, position.y);
			  sprite.setRotation(itemToRender.roation);
			  // Not the most efficient way to render, but it'll do.
			  // Maybe you'll get to make this faster!

			  window.draw(sprite);
		  }
		  lights.clear();
      }

      void SFMLGraphics::setColorKey(const uint8_t r, const uint8_t g,
        const uint8_t b) {
        this->_colorKey = sf::Color(r, g, b);
      }

      void SFMLGraphics::setFontColor(const uint8_t r, const uint8_t g,
        const uint8_t b) {
        this->_fontColor = sf::Color(r, g, b);
        this->_text.setColor(this->_fontColor);
      }

      void SFMLGraphics::shutdown() {}

      void SFMLGraphics::renderGUIRenderList() {
        // GO THROUGH EACH ITEM IN THE LIST

        SFMLTextureManager* manager = static_cast<SFMLTextureManager*>
          (guiTextureManager);

        SFMLOS* os = static_cast<SFMLOS*>(this->game->getOS());
        sf::RenderWindow& window = os->getWindow();

        for (const RenderItem& item : guiRenderList) {
          sf::Sprite sprite;

          // LET'S GET THE TEXTURE WE WANT TO RENDER
          size_t id = item.id;
          sf::Texture& texture = manager->getTexture(id);

          sprite.setTexture(texture);
          sprite.setPosition(item.x, item.y);
          sprite.setColor(sf::Color(255, 255, 255, item.a));

          window.draw(sprite);
        }

        // NOW EMPTY THE LIST, WE'RE ALL DONE WITH IT
        guiRenderList.clear();
      }

      void SFMLGraphics::renderWorldRenderList() {
        GameGUI& gui = game->getGUI();
        const Viewport& viewport = gui.getViewport();
        SFMLTextureManager* manager = static_cast<SFMLTextureManager*>(this->worldTextureManager);
        SFMLOS* os = static_cast<SFMLOS*>(this->game->getOS());
        sf::RenderWindow& window = os->getWindow();

        // GO THROUGH EACH ITEM IN THE LIST
        for (const RenderItem& itemToRender : worldRenderList) {
          // LET'S GET THE TEXTURE WE WANT TO RENDER
			sf::ConvexShape bounding_box_shape;
          sf::IntRect rect;
          sf::Sprite sprite;
          sf::Texture& texture = manager->getTexture(itemToRender.id);
          sf::Vector2i position(itemToRender.x, itemToRender.y);
		  sf::BlendMode blend = sf::BlendMultiply;
		 // sf::Transformable transformable(itemToRender.roation);

		  //establish the bounding box for rendering
		  bounding_box_shape.setPointCount(4);
		  sf::Vector2f upperLeft(itemToRender.x,itemToRender.y);
		  bounding_box_shape.setPoint(0, upperLeft);
		  sf::Vector2f upperRight(itemToRender.x + itemToRender.width, itemToRender.y);
		  bounding_box_shape.setPoint(1, upperRight);
		  sf::Vector2f lowerRight(itemToRender.x + itemToRender.width, itemToRender.y + itemToRender.height);
		  bounding_box_shape.setPoint(2, lowerRight);
		  sf::Vector2f lowerLeft(itemToRender.x, itemToRender.y + itemToRender.height);
		  bounding_box_shape.setPoint(3, lowerLeft);

		  //color the bounding box red
		  sf::Color redColor(255, 0, 0);
		  bounding_box_shape.setOutlineColor(redColor);
		  bounding_box_shape.setOutlineThickness(3.0f);
		  sf::Color transparent(0, 0, 0, 0);
		  bounding_box_shape.setFillColor(transparent);


          position.x += viewport.getViewportOffsetX();
          position.y += viewport.getViewportOffsetY();

          sf::Vector2u size = texture.getSize();
          rect.left = 0;
          rect.top = 0;
          rect.width = size.x;
          rect.height = size.y;

          // ADJUST FOR THE GUI OFFSET
          if (position.x < viewport.getViewportOffsetX()) {
            int dx = viewport.getViewportOffsetX() - position.x;
            rect.left = dx;
            rect.width -= dx;
            position.x += dx;
          }

          if (position.y < viewport.getViewportOffsetY()) {
            int dy = viewport.getViewportOffsetY() - position.y;
            rect.top = dy;
            rect.height -= dy;
            position.y += dy;
          }
          // This is a hackish form of clipping; when you start rotating and
          // scaling objects, this *will* stop being useful.

          sprite.setTexture(texture, true);
          sprite.setTextureRect(rect);
          sprite.setOrigin(0, 0);
          sprite.setColor(sf::Color(255, 255, 255, itemToRender.a));
          sprite.setPosition(position.x, position.y);
		  sprite.setRotation(itemToRender.roation);
          // Not the most efficient way to render, but it'll do.
          // Maybe you'll get to make this faster!

          window.draw(sprite, blend);
		  window.draw(bounding_box_shape);
        }
        worldRenderList.clear();
      }
    }
  }
}

