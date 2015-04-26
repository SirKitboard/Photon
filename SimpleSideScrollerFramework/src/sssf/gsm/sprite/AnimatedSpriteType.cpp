/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    AnimatedSpriteType.cpp

    See AnimatedSpriteType.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/gsm/sprite/AnimatedSpriteType.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace sprite {
        void AnimatedSpriteType::addAnimationFrame(
          const wstring& animationState,
          const size_t imageId,
          const uint16_t duration) {
          animationSequences.at(animationState).emplace_back(imageId, duration);
        }

        void AnimatedSpriteType::addAnimationSequence(const wstring& animationState) {
          animationSequences[animationState] = std::vector<AnimationFrame>();
        }

        size_t AnimatedSpriteType::getAnimationFrameID(
          const wstring& animationState,
          const size_t frameIndex) const {
          return animationSequences.at(animationState).at(frameIndex).imageId;
        }

        uint16_t AnimatedSpriteType::getDuration(
          const wstring& animationState,
          const size_t frameIndex) const {
          return animationSequences.at(animationState).at(frameIndex).duration;
        }
      }
    }
  }
}
