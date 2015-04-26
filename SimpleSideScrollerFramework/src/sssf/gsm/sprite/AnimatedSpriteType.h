/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    AnimatedSpriteType.h
    */

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "sssf/util/Util.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace sprite {
        using std::string;
        using std::to_string;
        using std::unordered_map;
        using std::vector;
        using std::wstring;

        /// <summary>
        /// Stores data to be shared between similar types of sprites. Individual
        /// sprites will be in different locations and have different state
        /// variables, but they may share things like animation sequences.
        /// </summary>
        class AnimatedSpriteType {
        public:
          // Nothing to construct
          AnimatedSpriteType(const wstring& name) : _name(name) {}
          ~AnimatedSpriteType() {}

          // You can copy AnimatedSpriteTypes if you want to, but you need
          // to use constructor syntax explicitly (to prevent accidental
          // copying).
          explicit AnimatedSpriteType(const AnimatedSpriteType&) = default;

          // INLINED ACCESSOR METHODS
          size_t getSequenceSize(const wstring& state) const {
            return animationSequences.at(state).size();
          }

          const wstring& getSpriteTypeName() const { return _name; }

          uint16_t getTextureHeight() const { return textureHeight; }
          uint16_t getTextureWidth() const { return textureWidth; }

          void setTextureSize(const uint16_t width, const uint16_t height) {
            textureWidth = width;
            textureHeight = height;
          }

          /**
           * Adds an animation tuple (image, duration) to an existing animation
           * state for this type of sprite.
           */
          void addAnimationFrame(const wstring&, const size_t, const uint16_t);

          /**
           * Adds a new animation state. It will set up a vector for storing the
           * data, but will not initialize it with any.
           */
          void addAnimationSequence(const wstring& animationState);

          /**
           * Gets the image id of the animation frame represented by the frameIndex value.
           */
          size_t getAnimationFrameID(const wstring&, const size_t) const;

          /**
           * Gets the duration of the animation frame represented by the frameIndex value.
           */
          uint16_t getDuration(const wstring&, const size_t) const;
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
          // Small helper struct so you don't need to interleave values in a vector
          // (which is error-prone)
          struct AnimationFrame {
            size_t imageId;
            uint16_t duration;

            AnimationFrame(const size_t id, const uint16_t d) :
              imageId(id), duration(d) {}
          };

          // EACH SPRITE TYPE HAS A UNIQUE NAME
          wstring _name;

          // animationSequences WILL BE A map OF JAGGED vectors STORING THE
          // IMAGE FLIPPING DATA FOR EACH ANIMATION STATE. BASICALLY EACH
          // (key,value) PAIR IN THE MAP RESPRESENTS A NAMED ANIMATION STATE,
          // LIKE "IDLE", WHICH WOULD BE THE KEY. THE VALUE WOULD BE THE VECTOR
          // STORING A SERIES OF (IMAGE ID, FRAMES DURATION) TUPLES IN THE
          // ANIMATION SEQUENCE FOR THAT GIVEN STATE. EACH TUPLE MEANS DISPLAY
          // THE IMAGE ID FOR FRAMES DURATION NUMBER OF FRAMES
          unordered_map<wstring, vector<AnimationFrame>> animationSequences;

          unordered_map<string, string> properties;

          // WIDTH & HEIGHT OF TEXTURES
          uint16_t textureHeight, textureWidth;
        };
      }
    }
  }
}
