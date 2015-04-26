#include "psti_VS/stdafx.h"
#include "psti/PoseurSpriteTypesImporter.h"

#include "sssf/game/Game.h"
#include "sssf/game/WStringTable.h"
#include "sssf/gsm/sprite/AnimatedSpriteType.h"
#include "tinyxml/tinyxml2.h"
#include "xmlfi/XMLFileImporter.h"
#include "sssf/graphics/TextureManager.h"
#include "sssf/graphics/GameGraphics.h"
#include "sssf/gsm/state/GameStateManager.h"
#include "sssf/util/Util.h"

namespace cse380 {
  namespace psti {
    // XML TEXT
    const char* XML_FILE_EXTENSION = ".xml";
    const char* SPRITE_TYPE_LIST_ELEMENT = "sprite_type_list";
    const char* SPRITE_TYPE_ELEMENT = "sprite_type";
    const char* WIDTH_ELEMENT = "width";
    const char* HEIGHT_ELEMENT = "height";
    const char* IMAGES_LIST_ELEMENT = "images_list";
    const char* IMAGE_FILE_ELEMENT = "image_file";
    const char* ID_ATT = "id";
    const char* FILE_NAME_ATT = "file_name";
    const char* ANIMATIONS_LIST_ELEMENT = "animations_list";
    const char* ANIMATION_STATE_ELEMENT = "animation_state";
    const char* STATE_ELEMENT = "state";
    const char* ANIMATION_SEQUENCE_ELEMENT = "animation_sequence";
    const char* POSE_ELEMENT = "pose";
    const char* IMAGE_ID_ATT = "image_id";
    const char* DURATION_ATT = "duration";

    using namespace sssf::game;
    using namespace sssf::gsm::sprite;
    using namespace sssf::util;
    using sssf::graphics::TextureManager;
    using std::cout;
    using std::endl;
    using tinyxml2::XMLDocument;
    using tinyxml2::XMLElement;
    using tinyxml2::XMLNode;

    bool PoseurSpriteTypesImporter::loadSpriteTypes(const wstring& typesFile) {
      int slashIndex = typesFile.rfind('/');
      string s = to_string(typesFile);
      dir = s.substr(0, slashIndex + 1);

      if (!loadSpriteTypesListInfo(s)) {
#ifdef DEBUG
        cout << "Failed to load sprite type list at \"" << s << '"' << endl;
#endif
        return false;
      }

      for (const auto& type : _types) {
        if (!loadSpriteTypeInfo(type.first)) {
#ifdef DEBUG
          cout << "Failed to load sprite type \"" << type.first << '"' << endl;
#endif
          return false;
        }
#ifdef DEBUG
        cout << "Loaded sprite type \"" << type.first << '"' << endl;
#endif
      }

      TextureManager* tm = _game->getGraphics()->getWorldTextureManager();
      const WStringTable& wStringTable = tm->getWStringTable();
      size_t offset = wStringTable.getNumWStringsInTable();

      // NOW LET'S USE ALL THE INFO WE'VE LOADED
      // LET'S START BY LOADING THE TEXTURES INTO THE WORLD TEXTURE MANAGER
      for (auto& type : _types) {
        AnimatedSpriteType* ast = new AnimatedSpriteType(to_wstring(type.first));
        _game->getGSM().getSpriteManager().addSpriteType(ast);
        ast->setTextureSize(type.second.width, type.second.height);

        for (const auto& a : type.second.animations_list) {
          wstring key = to_wstring(a.first);
          ast->addAnimationSequence(key);

          for (const Pose& pose : a.second) {
            ast->addAnimationFrame(
              key,
              type.second.images_list[pose.frameID].second,
              pose.duration
              );
          }
        }
      }
      return true;
    }

    bool PoseurSpriteTypesImporter::loadSpriteTypeInfo(const string& type) {
      string fullPath = dir + type;
      string spriteTypeFile = type + XML_FILE_EXTENSION;
      string spriteTypePath = fullPath + "/" + spriteTypeFile;
      XMLDocument doc;
      TextureManager* tm = _game->getGraphics()->getWorldTextureManager();

      if (doc.LoadFile(spriteTypePath.c_str()) == XMLError::XML_NO_ERROR) {
        const XMLElement* sprite_type = doc.FirstChildElement(SPRITE_TYPE_ELEMENT);

        if (sprite_type) {
          SpriteTypeInfo& info = _types[type];

          const XMLElement* width = sprite_type->FirstChildElement(WIDTH_ELEMENT);
          const XMLElement* height = sprite_type->FirstChildElement(HEIGHT_ELEMENT);
          const XMLElement* images_list = sprite_type->FirstChildElement(IMAGES_LIST_ELEMENT);
          const XMLElement* animations_list = sprite_type->FirstChildElement(ANIMATIONS_LIST_ELEMENT);

          if (width && height) {
            info.width = std::stoi(width->GetText());
            info.height = std::stoi(height->GetText());
          }
          else {
#ifdef DEBUG
            cout << "<width> and <height> must both be defined, but one or both"
              << " are missing in sprite type \"" << type << '"' << endl;
#endif
            _types.erase(type);
            return false;
          }

          if (images_list) {
            loadImagesListInfo(images_list, fullPath, info);
          }

          if (animations_list) {
            loadAnimationsListsInfo(animations_list, info);
          }

          return true;
        }
        else {
#ifdef DEBUG
          cout << "Failed to load sprite type \"" << type
            << "\" (no <sprite_type> element)" << endl;
#endif
          return false;
        }
      }

#ifdef DEBUG
      cout << "Error loading XML: " << doc.GetErrorStr1() << endl << doc.GetErrorStr2() << endl;
#endif

      return false;
    }

    bool PoseurSpriteTypesImporter::loadAnimationsListsInfo(
      const XMLElement* animations_list,
      SpriteTypeInfo& info) {
      assert(animations_list != nullptr && strcmp(animations_list->Name(), ANIMATIONS_LIST_ELEMENT) == 0);
      // We want a <images_list> element

      for (const XMLElement* a = animations_list->FirstChildElement(); a; a = a->NextSiblingElement()) {
        // FIRST GET THE STATE NAME
        if (strcmp(a->Name(), ANIMATION_STATE_ELEMENT) == 0) {
          // If this is really a <animation_state> element...
          const XMLElement* state = a->FirstChildElement(STATE_ELEMENT);

          if (state) {
            // If we found a <state> element...
            if (!loadAnimationStateInfo(a, state->GetText(), info)) {
              return false;
            }
          }
          else {
#ifdef DEBUG
            cout << "Couldn't load animation state, no <state> element found" << endl;
#endif
            return false;
          }
        }
      }
      return true;
    }

    bool PoseurSpriteTypesImporter::loadAnimationStateInfo(
      const XMLElement* animation_state,
      const string& state,
      SpriteTypeInfo& info) {
      assert(animation_state != nullptr && strcmp(animation_state->Name(), ANIMATION_STATE_ELEMENT) == 0);
      // We want a <animation_state> element

      // ADD THE ANIMATION SEQUENCE FOR THAT STATE
      const XMLElement* animation_sequence = animation_state->FirstChildElement(ANIMATION_SEQUENCE_ELEMENT);
      for (const XMLElement* p = animation_sequence->FirstChildElement(); p; p = p->NextSiblingElement()) {
        if (strcmp(p->Name(), POSE_ELEMENT) == 0) {
          // If this is really a <pose> element...
          Pose poseToAdd;
          poseToAdd.frameID = p->UnsignedAttribute(IMAGE_ID_ATT);
          poseToAdd.duration = p->UnsignedAttribute(DURATION_ATT);
          info.animations_list[state].push_back(poseToAdd);
        }
      }

      return true;
    }

    bool PoseurSpriteTypesImporter::loadSpriteTypesListInfo(const string& file) {
      XMLDocument doc;
      if (doc.LoadFile(file.c_str()) == XMLError::XML_NO_ERROR) {
        const XMLElement* sprite_type_list = doc.FirstChildElement(SPRITE_TYPE_LIST_ELEMENT);

        if (sprite_type_list) {

          // NOW GET THE SPRITE TYPES
          for (const XMLElement* s = sprite_type_list->FirstChildElement(); s; s = s->NextSiblingElement()) {
            if (strcmp(s->Name(), SPRITE_TYPE_ELEMENT) == 0) {
              // If this is really a <sprite_type> element...
              _types[s->GetText()] = SpriteTypeInfo();
            }
          }
        }
        else {
#ifdef DEBUG
          cout << "Couldn't load sprite type list \"" << file
            << "\": no <sprite_type_list> found" << endl;
#endif
          return false;
        }
      }
      else {
#ifdef DEBUG
        cout << "Error loading XML: " << doc.GetErrorStr1() << endl << doc.GetErrorStr2() << endl;
#endif
        return false;
      }
      return true;
    }

    bool PoseurSpriteTypesImporter::loadImagesListInfo(
      const XMLElement* images_list,
      const string& fullPath,
      SpriteTypeInfo& info) {
      assert(images_list != nullptr && strcmp(images_list->Name(), IMAGES_LIST_ELEMENT) == 0);
      // We want a <images_list> element

      TextureManager* tm = _game->getGraphics()->getWorldTextureManager();
      for (const XMLElement* i = images_list->FirstChildElement(); i; i = i->NextSiblingElement()) {
        if (strcmp(i->Name(), IMAGE_FILE_ELEMENT) == 0) {
          // If this is indeed a <image_file> element...
          uint16_t imageID = i->UnsignedAttribute(ID_ATT);
          string fileName = fullPath + "/" + i->Attribute(FILE_NAME_ATT);
          info.images_list[imageID] = std::make_pair(fileName, tm->loadTexture(to_wstring(fileName)));
        }
      }

      return true;
    }
  }
}