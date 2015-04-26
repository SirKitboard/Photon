/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    WorldLayer.h
    */
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "sssf/util/Util.h"

// FORWARD DECLARATIONS OF CLASSES USED BY WorldLayer

namespace cse380 {
  namespace sssf {

    namespace graphics { class RenderList; }
    namespace gui { class Viewport; }

    namespace gsm {

      namespace physics {
        class CollidableObject;
        class Physics;
      }

      namespace world {
        using std::round;
        using std::string;
        using std::stoi;
        using std::stol;
        using std::stoll;
        using std::stoul;
        using std::stoull;
        using std::stof;
        using std::stod;
        using std::to_string;
        using std::unordered_map;
        using std::vector;
        using std::wstring;
        using physics::CollidableObject;
        using physics::Physics;

        /// <summary>
        /// <para>
        /// Provides a framework for classes that manage data for one layer of
        /// texture rendering in a game world. Subclasses would use their own
        /// algorithms for rendering tiles.
        /// </para>
        /// <para>
        /// This class has two instance variables, parallaxFactorX and
        /// parallaxFactorY, which should be used to control parallax scrolling
        /// if desired. Parallax scrolling is when a background scrolls at a
        /// slower pace than objects in the foreground, like a player, to give
        /// the illusion of distance.
        /// </para>
        /// <para>
        /// parallaxFactorX and parallaxFactorY are real numbers and represent
        /// a scaling of scrolling in the x and y axes. So, if parallaxFactorX
        /// is 2, scrolling in the x axis will move for this layer at half the
        /// speed as those with factors of 1. The default for all layers is 1.
        /// If it is set to 0 or a negative number, parallax scrolling is not
        /// used for this layer. If it is set to a positive number less than 1,
        /// scrolling is sped up, by that factor.
        /// </para>
        /// <para>
        /// Generally speaking, the parallax factors for tiled layers should be
        /// calculated based on the dimensions of the layer relative to the
        /// world as a whole. For a sparse layer, this will have to be set since
        /// a sparse layer has no defined dimensions.
        /// </para>
        /// </summary>
        class WorldLayer {
        public:
          // BY DEFAULT, TILES ARE NOT COLLIDABLE
          WorldLayer() : collidableTiles(false) {}

          virtual ~WorldLayer() {}

          // INLINED ACCESSOR METHOD
          bool hasCollidableTiles() const { return collidableTiles; }
          uint16_t getWorldWidth() const { return worldWidth; }
          uint16_t getWorldHeight() const { return worldHeight; }

          // INLINED MUTATOR METHOD
          void setCollidableTiles(const bool on) { collidableTiles = on; }
          void setWorldWidth(const uint16_t w) { worldWidth = w; }
          void setWorldHeight(const uint16_t h) { worldHeight = h; }
          void setWorldSize(const uint16_t w, const uint16_t h) {
            worldWidth = w;
            worldHeight = h;
          }

          void setProperty(const string& key, const string& value) {
            this->properties[key] = value;
          }

          void setProperty(const string& key, const char* value) {
            this->properties[key] = value;
          }

          void setProperty(const string& key, const wstring& value) {
            this->properties[key] = util::to_string(value);
          }

          void setProperty(const string& key, const float value) {
            this->properties[key] = to_string(value);
          }

          void setProperty(const string& key, const int value) {
            this->properties[key] = to_string(value);
          }

          void setProperty(const string& key, const bool value) {
            this->properties[key] = (value) ? "true" : "false";
          }

          const string& getString(const string& key) const {
            return this->properties.at(key);
          }

          int getInteger(const string& key) const {
            return stoi(this->properties.at(key));
          }

          float getFloat(const string& key) const {
            return stof(this->properties.at(key));
          }

          bool getBool(const string& key) const {
            return this->properties.at(key) == "true";
          }

          // METHOD DEFINED IN CHILD CLASSES
          virtual void addRenderItemsToRenderList(graphics::RenderList&, const gui::Viewport&) const = 0;
          virtual bool willSpriteCollideOnTile(Physics&, const CollidableObject&) const = 0;
          virtual void findTileCollisionsForSprite(Physics&, CollidableObject&) = 0;
        protected:
          unordered_map<string, string> properties;
          bool collidableTiles;
          uint16_t worldWidth;
          uint16_t worldHeight;
        };
      }
    }
  }
}
