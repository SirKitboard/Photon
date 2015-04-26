#include "sssf_VS/stdafx.h"
#include "sssf/data_loader/GameDataLoader.h"

#include "sssf/game/Game.h"
#include "sssf/text/BufferedTextFileReader.h"

namespace cse380 {
  namespace sssf {
    namespace data {

      using game::Game;
      using text::BufferedTextFileReader;

      void GameDataLoader::loadGameProperties(Game* game,
        unordered_map<wstring, wstring>& properties,
        const wstring& gameInitFile) {
        // OPEN THE READER AND LOAD IT WITH THE CONTENTS OF gameInitFile
        BufferedTextFileReader reader;
        reader.initFile(gameInitFile);

        // NOW WE CAN READ IN ALL THE GAME DATA. NOTE THAT THE GAME DATA FILE
        // WE'RE READING NEEDS TO BE IN THE PROPER FORMAT, OBVIOUSLY
        wstring line;

        // LOAD ALL THE PROPERTIES
        while (reader.hasMoreLines()) {
          line = reader.getNextLine();
          loadProperty(line, properties);
        }
      }

      void GameDataLoader::loadProperty(const wstring& line,
        unordered_map<wstring, wstring>& properties) {
        unsigned int delimiterIndex = line.find(DEFAULT_DELIMITER);
        wstring name = line.substr(0, delimiterIndex);
        wstring value = line.substr(delimiterIndex + 1);
        properties[name] = value;
      }
    }
  }
}
