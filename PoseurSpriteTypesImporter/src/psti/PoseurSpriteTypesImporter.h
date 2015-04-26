#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "tinyxml/tinyxml2.h"
#include "xmlfi/XMLFileImporter.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
  }
  namespace psti {
    using std::pair;
    using std::string;
    using std::unordered_map;
    using std::vector;
    using std::wstring;

    using sssf::game::Game;
    using tinyxml2::XMLElement;
    using tinyxml2::XMLError;

    struct Pose {
      // Frame ID in the XML
      uint16_t frameID;

      uint16_t duration;
    };

    struct SpriteTypeInfo {
      uint16_t width;
      uint16_t height;

      // Corresponds to the <images_list> element; a mapping of frame IDs (NOT
      // TEXTURE IDS) to a pair consisting of the file name and its texture ID
      unordered_map<uint16_t, pair<string, size_t>> images_list;

      // Corresponds to the <animations_list> element; a mapping of state names
      // to their relevant pose lists
      unordered_map<string, vector<Pose>> animations_list;

      unordered_map<string, string> properties;
    };

    class PoseurSpriteTypesImporter : xmlfi::XMLFileImporter {
    public:
      PoseurSpriteTypesImporter(Game* game) : _game(game) {}
      ~PoseurSpriteTypesImporter() {}

      /// <summary>
      /// Loads all the sprite types found in the provided sprite type list file
      /// into the game state manager, including their images.
      /// </summary>
      bool loadSpriteTypes(const wstring&);
    protected:
      // Map of sprite type names to their relevant information
      unordered_map<string, SpriteTypeInfo> _types;
      string dir;
      // THESE ARE ALL THE HELPER METHODS
    private:
      Game* _game;
      bool loadSpriteTypesListInfo(const string&);
      bool loadSpriteTypeInfo(const string&);
      bool loadImagesListInfo(const XMLElement*, const string&, SpriteTypeInfo&);
      bool loadAnimationsListsInfo(const XMLElement*, SpriteTypeInfo&);
      bool loadAnimationStateInfo(const XMLElement*, const string&, SpriteTypeInfo&);
    };
  }
}