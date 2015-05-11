#include "sssf_VS/stdafx.h"
#include "sssf/audio/GameAudio.h"
#include "SFML/Audio.hpp"

namespace cse380 {
	namespace sssf {
		namespace audio {
			GameAudio::GameAudio() {
				
			}

			void GameAudio::initSoundEffect(std::string name, std::basic_string<char> path) {
				sf::Music* music = new sf::Music();
				music->openFromFile("data/audio/FastTalkin.ogg");
				soundEffects[name] = music;
				//addSound(name, &music);
			}


			void GameAudio::addSound(std::string name, sf::Music* music) {
				//soundEffects.at(name) = music;
			}

			void GameAudio::playSound(std::string name) {
				soundEffects[name]->play();
			}

			void GameAudio::initSoundEffect(int num, std::basic_string<char> path) {
				soundArray[num] = new sf::Music();
				soundArray[num]->openFromFile(path);
				
//				sf::Music music;
//				if (music.openFromFile("data/audio/FastTalkin.ogg")){
//					music.play();
//				}
//				sf::Music blah;
//				if (blah.openFromFile("data/audio/FastTalkin.ogg")) {
//					blah.setLoop(true);
//					blah.play();
//				}
			}


			void GameAudio::playSound(int num) {
				soundArray[num]->play();
			}

		}
	}
}