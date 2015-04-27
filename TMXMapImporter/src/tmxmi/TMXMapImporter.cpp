#include "tmxmi_VS/stdafx.h"
#include "tmxmi/TMXMapImporter.h"

#include "tinyxml/tinyxml2.h"
#include "sssf/game/Game.h"
#include "sssf/gsm/world/TiledLayer.h"
#include "sssf/gsm/world/Tile.h"
#include "sssf/graphics/TextureManager.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/gsm/world/World.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/util/Util.h"

namespace cse380 {
  namespace tmxmi {
    // XML TEXT
    const char* MAP_ELEMENT = "map";
    const char* ORIENTATION_ATT = "orientation";
    const char* ORTHOGONAL_VAL = "orthogonal";
    const char* ISOMETRIC_VAL = "isometric";
    const char* ID_ATT = "id";
    const char* WIDTH_ATT = "width";
    const char* HEIGHT_ATT = "height";
    const char* TILEWIDTH_ATT = "tilewidth";
    const char* TILEHEIGHT_ATT = "tileheight";
    const char* IMAGEWIDTH_ATT = "imagewidth";
    const char* IMAGEHEIGHT_ATT = "imageheight";
    const char* TILESET_ELEMENT = "tileset";
    const char* FIRSTGID_ATT = "firstgid";
    const char* NAME_ATT = "name";
    const char* TILEOFFSET_ELEMENT = "tileoffset";
    const char* OBJECTLAYER_ELEMENT = "objectlayer";
    const char* IMAGE_ELEMENT = "image";
    const char* SOURCE_ATT = "source";
    const char* IMAGELAYER_ELEMENT = "imagelayer";
    const char* LAYER_ELEMENT = "layer";
    const char* DATA_ELEMENT = "data";
    const char* PROPERTIES_ELEMENT = "properties";
    const char* PROPERTY_ELEMENT = "property";
    const char* COLLIDABLE_ATT = "collidable";
    const char* VALUE_ATT = "value";
    const char* TILE_ELEMENT = "tile";
    const char* GID_ATT = "gid";

    using std::cout;
    using std::endl;
    using namespace sssf::gsm::world;
    using namespace sssf::util;
    using sssf::game::Game;
    using sssf::graphics::TextureManager;
    using tinyxml2::XMLDocument;
    using tinyxml2::XMLError;

    bool TMXMapImporter::loadWorld(Game* game, const wstring& path) {
      this->path = path;
      if (!loadMapInfo())  {
#ifdef DEBUG
        cout << "Failed to load map info for level \"" << to_string(path) << '"' << endl;
#endif
        return false;
      }

      if (!buildWorldFromInfo(game)) {
#ifdef DEBUG
        cout << "Failed to build a World out of \"" << to_string(path) << '"' << endl;
#endif
        return false;
      }

#ifdef DEBUG
      cout << "Loaded level \"" << to_string(path) << '"' << endl;
#endif
      return true;
    }

    bool TMXMapImporter::loadMapInfo() {
      // LOAD THE XML DOC
      string s = to_string(path);
      XMLDocument doc;

      if (doc.LoadFile(s.c_str()) == XMLError::XML_NO_ERROR) {
        const XMLElement* map = doc.FirstChildElement(MAP_ELEMENT);
        if (map) {
          const char* orientation = map->Attribute(ORIENTATION_ATT);
          if (strcmp(orientation, ORTHOGONAL_VAL) == 0) {
            mapType = MapType::ORTHOGONAL_MAP;
            return loadOrthographicMap(map);
          }
          else if (strcmp(orientation, ISOMETRIC_VAL) == 0) {
            mapType = MapType::ISOMETRIC_MAP;
            return loadIsometricMap(map);
          }
          else {
#ifdef DEBUG
            cout << "Unrecognized map type \"" << orientation << "\" in TMX file \""
              << s << '"' << endl;
#endif
            return false;
          }
        }

#ifdef DEBUG
        cout << "No <map> element in TMX file \"" << s << '"' << endl;
#endif
        return false;
      }

#ifdef DEBUG
      cout << "Couldn't find or parse TMX file \"" << s << '"' << endl;
#endif
      return false;
    }

    bool TMXMapImporter::loadOrthographicMap(const XMLElement* map) {
      assert(map != nullptr && strcmp(map->Value(), MAP_ELEMENT) == 0);
      // We want a <map> element

      this->width = map->UnsignedAttribute(WIDTH_ATT);
      this->height = map->UnsignedAttribute(HEIGHT_ATT);
      this->tileWidth = map->UnsignedAttribute(TILEWIDTH_ATT);
      this->tileHeight = map->UnsignedAttribute(TILEHEIGHT_ATT);

      for (const XMLElement* n = map->FirstChildElement(); n; n = n->NextSiblingElement()) {
        string eName = n->Value();
        if (eName == TILESET_ELEMENT) { // <tileset>
          loadTileSetInfo(n);
        }
        else if (eName == IMAGELAYER_ELEMENT) { // <imagelayer>
          loadImageLayerInfo(n);
        }
        else if (eName == LAYER_ELEMENT) { // <layer>
          loadTiledLayerInfo(n);
        }
        else if (eName == PROPERTIES_ELEMENT) { // <properties>
          for (const XMLElement* p = n->FirstChildElement(); p; p = p->NextSiblingElement()) {
            if (strcmp(p->Value(), PROPERTY_ELEMENT) == 0) { // <property>
              this->custom[p->Attribute(NAME_ATT)] = p->Attribute(VALUE_ATT);
            }
          }
        }
        else if (eName == OBJECTLAYER_ELEMENT) { // <objectlayer>
          // You would use object layers for placing arbitrary shapes.
          // Very useful, but not implemented right now. No reason you can't do it!

#ifdef DEBUG
          cout << "<objectlayer> elements are not supported at this time" << endl;
#endif
        }
      }
      return true;
    }

    void TMXMapImporter::loadImageLayerInfo(const XMLElement* imagelayer) {
      assert(imagelayer != nullptr && strcmp(imagelayer->Value(), IMAGELAYER_ELEMENT) == 0);
      // We want a <imagelayer> element

      ImageLayerInfo imageLayerInfo;
      imageLayerInfo.name = imagelayer->Attribute(NAME_ATT);
      // WE DON'T ACTUALLY CARE ABOUT THE TMX WIDTH AND HEIGHT FOR THIS

      // NOW GET THE IMAGE ELEMENT
      const XMLElement* image = imagelayer->FirstChildElement(IMAGE_ELEMENT);
      imageLayerInfo.imageSource = image->Attribute(SOURCE_ATT);

      // AND NOW GET IMAGE LAYER PROPERTIES
      const XMLElement* properties = imagelayer->FirstChildElement(PROPERTIES_ELEMENT);

      if (properties) {
        for (const XMLElement* p = properties->FirstChildElement(); p; p = p->NextSiblingElement()) {
          if (strcmp(p->Name(), PROPERTY_ELEMENT) == 0) {
            // If this is really a <property> element...
            string att = p->Attribute(NAME_ATT);
            if (att == COLLIDABLE_ATT) {
              imageLayerInfo.collidable = p->BoolAttribute(VALUE_ATT);
            }
            else if (att == IMAGEHEIGHT_ATT) {
              imageLayerInfo.imageheight = p->UnsignedAttribute(VALUE_ATT);
            }
            else if (att == IMAGEWIDTH_ATT) {
              imageLayerInfo.imagewidth = p->UnsignedAttribute(VALUE_ATT);
            }
            else { // This is a custom property
              imageLayerInfo.properties[att] = p->Attribute(VALUE_ATT);
            }
          }
        }
      }
      // OK, NOW LOAD THE LAYER
      imageLayerInfos[imageLayerInfo.name] = imageLayerInfo;
    }

    void TMXMapImporter::loadTiledLayerInfo(const XMLElement* layer) {
      assert(layer != nullptr && strcmp(layer->Name(), LAYER_ELEMENT) == 0);
      // We want a <layer> element

      TiledLayerInfo tiledLayerInfo;
      tiledLayerInfo.name = layer->Attribute(NAME_ATT);
      tiledLayerInfo.width = layer->UnsignedAttribute(WIDTH_ATT);
      tiledLayerInfo.height = layer->UnsignedAttribute(HEIGHT_ATT);

      // NOW GET THE PROPERTIES
      const XMLElement* properties = layer->FirstChildElement(PROPERTIES_ELEMENT);
      if (properties) {
        for (const XMLElement* p = properties->FirstChildElement(); p; p = p->NextSiblingElement()) {
          if (strcmp(p->Name(), PROPERTY_ELEMENT) == 0) {
            // If this is really a <property> element...
            string att = p->Attribute(NAME_ATT);
            if (att == COLLIDABLE_ATT) {
              tiledLayerInfo.collidable = p->BoolAttribute(VALUE_ATT);
            }
            else {
              tiledLayerInfo.properties[att] = p->Attribute(VALUE_ATT);
            }
          }
        }
      }

      // NOW GET THE TILE DATA
      const XMLElement* data = layer->FirstChildElement(DATA_ELEMENT);
      if (!data) {
#ifdef DEBUG
        cout << "No <data> found for tiled layer " << tiledLayerInfo.name << endl;
        return;
#endif
      }

      for (const XMLElement* t = data->FirstChildElement(); t; t = t->NextSiblingElement()) {
        if (strcmp(t->Name(), TILE_ELEMENT) == 0) {
          // If this is really a <tile>...
          int gid = t->IntAttribute(GID_ATT);
          tiledLayerInfo.gids.push_back(gid);
        }
      }
      tiledLayerInfo.tileSetInfo = getTileSetForId(tiledLayerInfo.gids[0]);
      tiledLayerInfos[tiledLayerInfo.name] = tiledLayerInfo;
    }

    void TMXMapImporter::loadTileSetInfo(const XMLElement* tileset) {
      assert(tileset != nullptr && strcmp(tileset->Value(), TILESET_ELEMENT) == 0);
      // We want a <tileset> element

      TileSetInfo tileSetInfo;
      tileSetInfo.firstgid = tileset->IntAttribute(FIRSTGID_ATT);
      tileSetInfo.name = tileset->Attribute(NAME_ATT);
      tileSetInfo.tilewidth = tileset->UnsignedAttribute(TILEWIDTH_ATT);
      tileSetInfo.tileheight = tileset->UnsignedAttribute(TILEHEIGHT_ATT);

      // NOW GET THE IMAGE INFO
      const XMLElement* image = tileset->FirstChildElement(IMAGE_ELEMENT);

      if (!image) {
#ifdef DEBUG
        cout << "No <image> attribute found for \"" << tileSetInfo.name << '"' << endl;
#endif
        return;
      }
      tileSetInfo.sourceImage = image->Attribute(SOURCE_ATT);
      tileSetInfo.sourceImageWidth = image->UnsignedAttribute(WIDTH_ATT);
      tileSetInfo.sourceImageHeight = image->UnsignedAttribute(HEIGHT_ATT);

      // Rows and columns in the TILESET, not the level
      int cols = tileSetInfo.sourceImageWidth / tileSetInfo.tilewidth;
      int rows = tileSetInfo.sourceImageHeight / tileSetInfo.tileheight;
      vector<bool> tilesAdded(rows * cols, false);

      for (const XMLElement* t = tileset->FirstChildElement(TILE_ELEMENT); t; t = t->NextSiblingElement()) {
        if (strcmp(t->Name(), TILE_ELEMENT) == 0) {
          // If this is really a <tile> element...
          const XMLElement* tileProps = t->FirstChildElement(PROPERTIES_ELEMENT);
          TileInfo tileInfo;

          for (const XMLElement* p = tileProps->FirstChildElement(); p; p = p->NextSiblingElement()) {
            if (strcmp(p->Name(), PROPERTY_ELEMENT) == 0) {
              // If this is really a <property> element...
              tileInfo.properties[p->Attribute(NAME_ATT)] = p->Attribute(VALUE_ATT);
            }
          }

          int id = t->IntAttribute(ID_ATT);
          tilesAdded[id] = true;
          tileSetInfo.tileInfo[id] = tileInfo;
        }
      }

      for (size_t i = 0; i < tilesAdded.size(); ++i) {
        if (!tilesAdded[i]) {
          tileSetInfo.tileInfo[i] = TileInfo();
        }
      }

      const XMLElement* properties = tileset->FirstChildElement(PROPERTIES_ELEMENT);
      if (properties) {
        for (const XMLElement* p = properties->FirstChildElement(); p; p = p->NextSiblingElement()) {
          if (strcmp(p->Name(), PROPERTY_ELEMENT) == 0) {
            // If this is really a <property> element...
            tileSetInfo.properties[p->Attribute(NAME_ATT)] = p->Attribute(VALUE_ATT);
          }
        }
      }

      tileSetInfos[tileSetInfo.name] = tileSetInfo;
    }

    bool TMXMapImporter::loadIsometricMap(const XMLElement*) {
#ifdef DEBUG
      cout << "Isometric maps are not supported at this time" << endl;
#endif
      return false;
    }

    bool TMXMapImporter::buildWorldFromInfo(Game* game) {
      TextureManager* worldTextureManager = game->getGraphics()->getWorldTextureManager();
      World& world = game->getGSM().getWorld();
      for (const auto& prop : this->custom) {
        world.setProperty(prop.first, prop.second);
      }
	  b2World* gameWorld = game->getGameWorld();
      if (mapType == MapType::ORTHOGONAL_MAP) {
        uint16_t largestLayerWidth = 0;
        uint16_t largestLayerHeight = 0;

        // LET'S FIRST FIGURE OUT THE WORLD WIDTH AND HEIGHT

        // FIRST THE IMAGE LAYERS
        for (const auto& i : imageLayerInfos) {
          const ImageLayerInfo& ili = imageLayerInfos[i.first];
          largestLayerWidth = std::max(ili.imagewidth, largestLayerWidth);
          largestLayerHeight = std::max(ili.imageheight, largestLayerHeight);
        }
        // AND THE TILED LAYERS
        for (const auto& i : tiledLayerInfos) {
          const TiledLayerInfo& tli = tiledLayerInfos[i.first];
          uint16_t layerWidth = tli.width * tli.tileSetInfo->tilewidth;
          uint16_t layerHeight = tli.height * tli.tileSetInfo->tileheight;

          largestLayerWidth = std::max(largestLayerWidth, layerWidth);
          largestLayerHeight = std::max(largestLayerHeight, layerHeight);
        }

        wstring dir = path.substr(0, path.rfind('/') + 1);
        // FIRST LOAD ALL THE TILE SETS
        for (const auto& i : tileSetInfos) {
          const TileSetInfo& tsi = tileSetInfos[i.first];
          wstring sourceImageW = to_wstring(tsi.sourceImage);
          wstring fullPath = dir + sourceImageW;
          worldTextureManager->loadTexture(fullPath);
          bool success = worldTextureManager->loadTileSetFromTexture(game, fullPath, tsi.tilewidth, tsi.tileheight);
          if (!success) {
            return false;
          }
        }

        // NOW LOAD THE IMAGE LAYERS, IF THERE ARE ANY
        for (const auto& i : imageLayerInfos) {
          const ImageLayerInfo& ili = imageLayerInfos[i.first];

          TiledLayer* layer = new TiledLayer(
            1,
            1,
            ili.imagewidth,
            ili.imageheight,
            0,
            ili.collidable,
            largestLayerWidth,
            largestLayerHeight,
            1
            );
          for (const auto& prop : ili.properties) {
            layer->setProperty(prop.first, prop.second);
          }
          world.addLayer(layer);

          Tile tile;
          tile.collidable = ili.collidable;
          tile.tileID = 0;
          wstring imageSourceW = to_wstring(ili.imageSource);
          tile.textureID = worldTextureManager->loadTexture(dir + imageSourceW);
          layer->addTile(1, tile);
          layer->setTile(tile, 0);
        }

        // AND NOW LOAD THE TILED LAYERS, WHICH REFERENCE THE TILE SETS
        for (const auto& i : tiledLayerInfos) {
          const TiledLayerInfo& tli = tiledLayerInfos[i.first];
          TiledLayer* layer = new TiledLayer(
            tli.width,
            tli.height,
            tli.tileSetInfo->tilewidth,
            tli.tileSetInfo->tileheight,
            0,
            tli.collidable,
            largestLayerWidth,
            largestLayerHeight,
            tli.tileSetInfo->firstgid
            );

          for (const auto& prop : tli.properties) {
            layer->setProperty(prop.first, prop.second);
          }

          // WE HAVE TO ADD ALL THE TILES
          TileSetInfo* tileSet = tli.tileSetInfo;

          size_t baseTextureID = worldTextureManager->loadTexture(dir + to_wstring(tileSet->sourceImage));
		  int counter = 0;
          for (const auto& t : tli.tileSetInfo->tileInfo) {
            Tile tile;
            tile.tileID = t.first;
            tile.textureID = baseTextureID + t.first + 1;
            tile.properties = t.second.properties;
            int specifies = tile.properties.count(COLLIDABLE_ATT);
            bool c = (specifies) ? tile.properties.at(COLLIDABLE_ATT) == "true" : false;
			tile.collidable = c;
			layer->addTile(t.first, tile);
			counter++;
          }

          size_t size = layer->getRows() * layer->getColumns();
          for (size_t i = 0; i < tli.gids.size(); ++i) {
            layer->setTile(tli.gids[i], i);
          }

          world.addLayer(layer);
//		  layer->generateCollidableTiles(game->getGameWorld());
        }

        // AND MAKE THE WORLD DIMENSIONS THE SIZE OF THE LARGEST LAYER
        world.setWorldSize(largestLayerWidth, largestLayerHeight);
      }
      return true;
    }

    TileSetInfo* TMXMapImporter::getTileSetForId(const int id) {
      for (const auto& i : tileSetInfos) {
        const TileSetInfo& tsi = tileSetInfos.at(i.first);
        int columns = tsi.sourceImageWidth / tsi.tilewidth;
        int rows = tsi.sourceImageHeight / tsi.tileheight;
        int numTiles = rows * columns;

        if ((id >= tsi.firstgid) && (id < (tsi.firstgid + numTiles))) {
          return &tileSetInfos.at(i.first);
        }
      }

      return nullptr;
    }
  }
}