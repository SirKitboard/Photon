/*
Author: Aditya balwani
		Stony Brook University
		Computer Science Department
		*/

#pragma once
#include <string>
#include <SFML/Audio.hpp>

namespace sf{
	class Music;
}

namespace cse380 {
	namespace sssf {

		namespace game { class Game; }
		namespace os { class GameOS; }
		namespace text {
			class GameText;
			class TextToDraw;
		}

		namespace audio {

			using game::Game;
			using os::GameOS;
			using text::TextToDraw;
			using text::GameText;
			using std::unordered_map;
			using std::wstring;

			// Provides a framework for technology-specific classes that will manage
			// the rendering of all game textures and text. It provides methods for
			// the initialization of objects necessary for rendering images and text.
			//
			// Rendering should be done by generating a list of RenderItem objects
			// each frame and putting them into a list. These objects store information
			// about the textures that have to be drawn to the screen.
			class GameAudio {
			private:
				std::map<std::string, sf::Music*> soundEffects;
				sf::Music* soundArray[10];
			public:
				GameAudio();
				void initSoundEffect(int num, std::basic_string<char> path);
				void playSound(int num);
				void initSoundEffect(std::string name, std::basic_string<char> path);
				void addSound(std::string name, sf::Music*);
				void playSound(std::string name);
			};
		}
	}
}