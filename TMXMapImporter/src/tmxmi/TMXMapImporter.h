#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "tinyxml/tinyxml2.h"
#include "xmlfi/XMLFileImporter.h"

namespace cse380 {
  namespace sssf {
    namespace game { class Game; }
  }
  namespace tmxmi {
    using std::map;
    using std::string;
    using std::wstring;
    using std::vector;
    using std::unordered_map;
    using tinyxml2::XMLElement;
    using tinyxml2::XMLNode;

    using sssf::game::Game;

    enum MapType { ORTHOGONAL_MAP, ISOMETRIC_MAP };

    struct TileInfo {
      unordered_map<string, string> properties;
    };

    struct TileSetInfo {
      string name;
      string sourceImage;
      int firstgid;
      uint16_t tilewidth;
      uint16_t tileheight;
      uint16_t sourceImageWidth;
      uint16_t sourceImageHeight;
      map<size_t, TileInfo> tileInfo;
      unordered_map<string, string> properties;
    };

    struct TiledLayerInfo {
      string name;
      vector<size_t> gids;
      TileSetInfo* tileSetInfo;
      uint16_t width;
      uint16_t height;
      bool collidable;
      unordered_map<string, string> properties;
    };

    struct ImageLayerInfo {
      string name;
      string imageSource;
      uint16_t imagewidth;
      uint16_t imageheight;
      bool collidable;
      unordered_map<string, string> properties;
    };

    class TMXMapImporter : public xmlfi::XMLFileImporter {
    public:
      TMXMapImporter() {}
      ~TMXMapImporter() {}
      bool loadWorld(Game*, const wstring&);
    protected:
      MapType mapType;
      uint16_t width;
      uint16_t height;
      uint16_t tileWidth;
      uint16_t tileHeight;
      unordered_map<string, TileSetInfo> tileSetInfos;
      unordered_map<string, TiledLayerInfo> tiledLayerInfos;
      unordered_map<string, ImageLayerInfo> imageLayerInfos;
      unordered_map<string, string> custom;
      wstring path;
      // THESE ARE ALL THE HELPER METHODS
    private:
      bool buildWorldFromInfo(Game*);
      bool loadMapInfo();
      void loadTileSetInfo(const XMLElement*);
      void loadTiledLayerInfo(const XMLElement*);
      void loadImageLayerInfo(const XMLElement*);
      bool loadOrthographicMap(const XMLElement*);
      bool loadIsometricMap(const XMLElement*);
      TileSetInfo* getTileSetForId(const int);
    };
  }
}