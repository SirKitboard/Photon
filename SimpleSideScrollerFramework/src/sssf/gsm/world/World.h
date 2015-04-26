/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    World.h
    */
#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "sssf/util/Util.h"
#include <Box2D/Box2D.h>
#include <sssf/gsm/physics/Box2DWorld.h>


namespace cse380 {
  namespace sssf {

    namespace game { class Game; }

    namespace gsm {
      namespace world {
        using std::string;
        using std::to_string;
        using std::unordered_map;
        using std::vector;
        using std::wstring;
        using game::Game;

        class WorldLayer;

        /// <summary>
        /// <para>
        /// Mmanages the static data for the current game level. This means all
        /// game backgrounds, which are stored and manipulated in WorldLayer
        /// objects. A given level can have a background rendered using multiple
        /// layers of varying types (orthogonal, sparse, etc.).
        /// </para>
        /// <para>
        /// This class stores these layers and ensures they are rendered in the
        /// proper order, low index to high. Layers that need to be drawn first
        /// (the back-most layer) should be added first.
        /// </para>
        /// </summary>
        class World {
        public:
          /**
           * Constructs the layers vector, allowing new layers to be added.
           */
          World(const int, const int);
          World() : World(0, 0) {}
          ~World();

          // No copying allowed!
          World(const World&) = delete;
          World& operator=(const World&) = delete;

          // INLINED ACCESSOR METHODS
          const vector<WorldLayer*>& getLayers() const { return layers; }
          vector<WorldLayer*>& getLayers() { return layers; }
          size_t getNumLayers() const { return layers.size(); }
          int getWorldHeight() const { return worldHeight; }
          int getWorldWidth() const { return worldWidth; }

          // INLINED MUTATOR METHODS
          void setWorldHeight(const int h) { worldHeight = h; }
          void setWorldWidth(const int w) { worldWidth = w; }
          void setWorldSize(const int w, const int h) {
            worldWidth = w;
            worldHeight = h;
          }

          /// <summary>
          /// Adds a layer to the world. These layers might be TiledLayers,
          /// SparseLayers, or IsometricLayers, all of which are child classes of
          /// WorldLayer.
          /// </summary>
          /// <warning>
          /// Any added layers will become managed by this World. Don't delete
          /// them; the World will do so when it dies!
          /// </warning>
          void addLayer(WorldLayer* layerToAdd);

          /**
           * Sends the render list and viewport to each of the layers such that
           * they may fill it with RenderItems to draw.
           */
          void addWorldRenderItemsToRenderList(Game* game) const;

          /// <summary>
          /// Removes all data from the World. Should be called first when a
          /// level is unloaded or changed; otherwise, a game will have lots of
          /// extra data sitting around that may slow the progam down. Or, if the
          /// world thinks a level is still active, it might add items to the
          /// render list using image ids that have already been cleared from
          /// the GameGraphics' world texture manager. That would likely result
          /// in an exception.
          /// </summary>
          void unloadWorld();

          /**
           * This method should be called once per frame. Note that the World is
           * for static objects, so we don't have anything to update as/is. But,
           * should the need arise, one could add non-collidable layers to a game
           * world and update them here. For example, particle systems.
           */
          void update(Game* game);

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
        private:
          // NOTE, THE worldWidth & worldHeight REPRESENT THE FULL
          // SIZE OF THIS LEVEL. WHICH SHOULD BE AT LEAST THE SIZE OF
          // THE VIEWPORT, AND LIKELY MUCH LARGER, WHICH WOULD THEN
          // REQUIRE SCROLLING TO SEE IT.
          int worldWidth;
          int worldHeight;
		 // static Box2DWorld* physicalWorld;

          // THESE ARE THE BACKGROUND LAYERS
          vector<WorldLayer*> layers;

          unordered_map<string, string> properties;
        };
      }
    }
  }
}
