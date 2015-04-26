#include "sssf_VS/stdafx.h"
#include "sssf/gsm/physics/CollidableObject.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace physics {
        void CollidableObject::updateSweptShape(const float
          percentageOfFrameTimeRemaining) {
          // NOTE THAT IF WE ARE MID-FRAME WE DON'T WISH TO
          // APPLY THE FULL VELOCITY
          float scaledVx = pp.getVelocityX() * percentageOfFrameTimeRemaining;
          float scaledVy = pp.getVelocityY() * percentageOfFrameTimeRemaining;

          // NOTE THE DIMENSIONS OF THE SWEPT SHAPE ARE CALCULATED USING
          // THE ABOLUTE VALUE OF THE VELOCITY
          sweptShape.setWidth(boundingVolume.getWidth() + abs(scaledVx));
          sweptShape.setHeight(boundingVolume.getHeight() + abs(scaledVy));

          // UPDATE ITS CENTER?
          float centerX = boundingVolume.getCenterX() + scaledVx / 2.0f;
          float centerY = boundingVolume.getCenterY() + scaledVy / 2.0f;
          sweptShape.setCenter(centerX, centerY);
        }
      }
    }
  }
}
