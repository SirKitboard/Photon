#include "sssf_VS/stdafx.h"
#include "sssf/platforms/cross/SFMLAudio.h"
//#include "sssf/game/Game.h"

const std::string pathRoot = "data/audio/";

namespace cse380 {
	namespace sssf {
		namespace sfml {
			namespace audio {

				SFMLAudio::SFMLAudio(Game* initGame)
				{
					this->game = initGame;
				}

				bool SFMLAudio::loadSoundBuffer(std::string const& filename)
				{
					sf::SoundBuffer* sb = new sf::SoundBuffer();
					std::cout <<"Path: " + pathRoot + filename<< std::endl;
					bool sucess = sb->loadFromFile(pathRoot+filename);
					//std::cout << sucess << std::endl;
					soundBuffers[filename] = *sb;
					return sucess;
				}
				sf::SoundBuffer& SFMLAudio::getSoundBuffer(std::string const& filename)
				{
					return soundBuffers[filename];
				}
				void SFMLAudio::clear()
				{
					soundBuffers.clear();
				}

			}
		}
	}
}