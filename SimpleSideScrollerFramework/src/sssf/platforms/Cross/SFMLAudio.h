#pragma once
#include "sssf_VS/stdafx.h"
#include <SFML/Audio.hpp>

namespace cse380 {
	namespace sssf {
		namespace game { class Game;  }
			namespace sfml {
				namespace audio {
					using game::Game;
					class SFMLAudio {
					public:
						SFMLAudio(Game* initGame);
						~SFMLAudio(){}

						bool loadSoundBuffer(std::string const& filename);
						sf::SoundBuffer& getSoundBuffer(std::string const& filename);
						void clear();

					private:
						Game* game;
						std::map<std::string, sf::SoundBuffer> soundBuffers;
					};
				}
			}
	}
}

