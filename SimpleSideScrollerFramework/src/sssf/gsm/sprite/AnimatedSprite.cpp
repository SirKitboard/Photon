/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    AnimatedSprite.cpp

    See AnimatedSprite.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/sprite/AnimatedSprite.h"

#include "sssf/gsm/sprite/AnimatedSpriteType.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace sprite {
        AnimatedSprite::AnimatedSprite() :
          spriteType(0),
          frameIndex(0),
          animationCounter(0) {}

        void AnimatedSprite::changeFrame() {
          // RESET THE COUNTER
          animationCounter = 0;

          // GO BACK TO FIRST INDEX IF NECESSARY
          frameIndex = (frameIndex + 1) % spriteType->getSequenceSize(currentState);
          if (frameIndex == 0 && this->_onStateFinished != nullptr) {
            this->_onStateFinished(*this, this->currentState);
          }
        }

        size_t AnimatedSprite::getCurrentImageID() const {
          return spriteType->getAnimationFrameID(currentState, frameIndex);
        }

        void AnimatedSprite::setCurrentState(const wstring& newState) {
          if (currentState != newState) {
            // SET THE ANIMATION STATE
            currentState = newState;

            // AND RESET OUR COUNTERS
            animationCounter = 0;
            frameIndex = 0;
          }
        }

        void AnimatedSprite::updateSprite() {
//          uint16_t duration = spriteType->getDuration(currentState, frameIndex);
//          animationCounter++;
//
//          // WE ONLY CHANGE THE ANIMATION FRAME INDEX WHEN THE
//          // ANIMATION COUNTER HAS REACHED THE DURATION
//          if (animationCounter >= duration)
//            changeFrame();
        }

        void AnimatedSprite::affixTightAABBBoundingVolume() {
          boundingVolume.setCenterX(pp.getX() + (spriteType->getTextureWidth() / 2));
          boundingVolume.setCenterY(pp.getY() + (spriteType->getTextureHeight() / 2));
          boundingVolume.setWidth(spriteType->getTextureWidth());
          boundingVolume.setHeight(spriteType->getTextureHeight());
        }

        void AnimatedSprite::correctToTightBoundingVolume() {
          pp.setX(boundingVolume.getLeft());
          pp.setY(boundingVolume.getTop());
        }
      }
    }
  }
}
