#include "sssf_VS/stdafx.h"
#include "sssf/platforms/Cross/SFMLGraphics.h"

#include "sssf/graphics/TextureManager.h"
#include "sssf/platforms/Cross/SFMLTextureManager.h"
#include "sssf/platforms/Cross/SFMLOS.h"
#include "sssf/game/Game.h"
#include "sssf/gsm/world/World.h"
#include "sssf/gsm/state/GameStateManager.h"

#include "sssf/graphics/Light.h"

namespace cse380 {
  namespace sssf {
    namespace sfml {
      // THIS IS THE COLOR WE WILL USE TO CLEAR THE SCREEN WITH
      const sf::Color BACKGROUND_COLOR(10,10,10);

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
		this->dynamicLights = vector<Light*>();
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
		renderDynamicLight();
        // RENDER THE WORLD RENDER LIST
        renderWorldRenderList();
		renderWalls();

        // RENDER THE GUI RENDER LIST
		if (!game->getPaused()) {
			renderGUIRenderList();
		}

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

	  void SFMLGraphics::renderDynamicLight() {
		  GameGUI& gui = game->getGUI();
		  const Viewport& viewport = gui.getViewport();
		  //SFMLTextureManager* manager = static_cast<SFMLTextureManager*>(this->guiTextureManager);
		  SFMLOS* os = static_cast<SFMLOS*>(this->game->getOS());
		  sf::RenderWindow& window = os->getWindow();
		  gsm::physics::PhysicalProperties pp = game->getGSM().getSpriteManager().getPlayer().getPhysicalProperties();
		  if (game->getGSM().isGameInProgress())
		  {
			  Light light = Light(game, b2Vec2(pp.getX(), pp.getY()), 500.0f, 3.14f, 6.5f, true);
			  std::cout << light.pointRectDist(light.getPosition()) << " " << light.getReach();
			  if (light.getReach() >= light.pointRectDist(light.getPosition()))
			  {
				  window.draw(light.computeAndGetLightShape(255,255,255));
			  }
			  vector<gsm::ai::Bot*> bots = game->getGSM().getSpriteManager().getBots();
			  for (gsm::ai::Bot* bot : bots) {
				  pp = bot->getPhysicalProperties();
				  double angle;
				  if (pp.getVelocityX() > 0 && pp.getVelocityY() == 0) 
					  angle = 1.57;
				  else if (pp.getVelocityX() < 0 && pp.getVelocityY() == 0) 
					  angle = -1.57;
				  else if (pp.getVelocityX() == 0 && pp.getVelocityY() > 0) 
					  angle = 3.14;
				  else if (pp.getVelocityX() == 0 && pp.getVelocityY() < 0) 
					  angle = 0;
				  else if (pp.getVelocityX() > 0 && pp.getVelocityY() > 0) 
					  angle = 2.335;
				  else if (pp.getVelocityX() > 0 && pp.getVelocityY() < 0) 
					  angle = 0.765;
				  else if (pp.getVelocityX() < 0 && pp.getVelocityY() > 0) 
					  angle = -2.335;
				  else 
					  angle = -0.765;
//				  if (pp.getVelocityX() < 0 && pp.getVelocityY() == 0) angle = 1.57;
//				  if (pp.getVelocityX() == 0 && pp.getVelocityY() > 0) angle = 3.14;
//				  if (pp.getVelocityX() == 0 && pp.getVelocityY() < 0) angle = 0;
				  light = Light(game, b2Vec2(pp.getX(), pp.getY()), 500.0f, angle, 2.0f, true);
				  if (light.getReach() >= light.pointRectDist(light.getPosition()))
				  {
					  window.draw(light.computeAndGetLightShape(255, 50, 50));
				  }
			  }
			  //window.draw(light.computeAndGetLightShape());
		  }
		  
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

		sf::RectangleShape gui(sf::Vector2f(window.getSize().x, window.getSize().y));
		gui.setPosition(0, 0);
		gui.setFillColor(sf::Color(0,0,0,180));
		window.draw(gui);

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

	  void SFMLGraphics::renderWalls() {
		  GameGUI& gui = game->getGUI();
		  const Viewport& viewport = gui.getViewport();
		  SFMLTextureManager* manager = static_cast<SFMLTextureManager*>(this->worldTextureManager);
		  SFMLOS* os = static_cast<SFMLOS*>(this->game->getOS());
		  sf::RenderWindow& window = os->getWindow();
		  sf::BlendMode blend = sf::BlendMultiply;
		  for (const RenderItem& itemToRender : tiles) {
			  sf::RectangleShape gui(sf::Vector2f(itemToRender.width, itemToRender.height));
			  gui.setPosition(itemToRender.x, itemToRender.y);
			  gui.setFillColor(sf::Color(0,0,0));
			  window.draw(gui,blend);
		  }
		  tiles.clear();
	  }


      void SFMLGraphics::renderWorldRenderList() {
        GameGUI& gui = game->getGUI();
        const Viewport& viewport = gui.getViewport();
        SFMLTextureManager* manager = static_cast<SFMLTextureManager*>(this->worldTextureManager);
        SFMLOS* os = static_cast<SFMLOS*>(this->game->getOS());
        sf::RenderWindow& window = os->getWindow();
		sf::BlendMode blend = sf::BlendMultiply;
        // GO THROUGH EACH ITEM IN THE LIST
        for (const RenderItem& itemToRender : worldRenderList) {
          // LET'S GET THE TEXTURE WE WANT TO RENDER
          sf::IntRect rect;
          sf::Sprite sprite;
          sf::Texture& texture = manager->getTexture(itemToRender.id);
          sf::Vector2i position(itemToRender.x, itemToRender.y);
		  
		 // sf::Transformable transformable(itemToRender.roation);
          sf::Vector2u size = texture.getSize();
          rect.left = 0;
          rect.top = 0;
          rect.width = size.x;
          rect.height = size.y;

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
        }
        worldRenderList.clear();
      }
    }
  }
}

