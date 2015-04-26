#pragma once
#include <Box2D/Collision/b2Collision.h>
namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace physics {
		//TODO: Create Replacement
        /// <remarks>
        /// Once you start using Box2D, this class won't be very useful anymore;
        /// you'll likely be using b2AABB instead.
        /// </remarks>
        class AABB {
        public:
			
          AABB() {
			  aabb.lowerBound.SetZero();
			  aabb.upperBound.SetZero();
          }
          AABB(const float cx, const float cy, const float w, const float h)
          {
			  aabb.upperBound.Set((cx - (w / 2)), (cy + (h / 2)));
			  aabb.lowerBound.Set((cx + (w / 2)), (cy - (h / 2)));
          }
          ~AABB() {}

          // INLINED METHODS

		  void incCenterX(const float xInc) {
	          aabb.upperBound.Set(aabb.upperBound.x + xInc,aabb.upperBound.y);
			  aabb.lowerBound.Set(aabb.lowerBound.x + xInc, aabb.lowerBound.y);
          }
          void incCenterY(const float yInc) {
			  aabb.upperBound.Set(aabb.upperBound.x, aabb.upperBound.y + yInc);
			  aabb.lowerBound.Set(aabb.lowerBound.x, aabb.lowerBound.y + yInc);
          }
          void incCenter(const float x, const float y) {
            incCenterX(x);
            incCenterY(y);
          }

          float getWidth() const { return abs(aabb.upperBound.x - aabb.lowerBound.x); }
		  float getHeight() const { return abs(aabb.upperBound.y - aabb.lowerBound.y); }
          float getCenterX() const { return aabb.GetCenter().x; }
		  float getCenterY() const { return aabb.GetCenter().y; }

          void setWidth(const float w) {
			  float w2 = w / 2;
			  aabb.upperBound.x -= w2;
			  aabb.lowerBound.x += w2;
          }
          void setHeight(const float h) {
			  float h2 = h / 2;
			  aabb.upperBound.y += h2;
			  aabb.lowerBound.y -= h2;
          }
          void setSize(const float w, const float h) {
			  setWidth(w);
			  setHeight(h);
          }

          void setCenterX(const float cx) {
			  float diff = aabb.upperBound.x - cx;
			  aabb.upperBound.x += diff;
			  aabb.lowerBound.x += diff;
          }
          void setCenterY(const float cy) {
			  float diff = aabb.upperBound.y - cy;
			  aabb.upperBound.y += diff;
			  aabb.lowerBound.y += diff;
          }
          void setCenter(const float x, const float y) {
			  setCenterX(x);
			  setCenterY(y);
          }

		  float getLeft() const { return aabb.upperBound.x; }
          float getRight() const { return aabb.lowerBound.x; }
          float getTop() const { return aabb.upperBound.y; }
          float getBottom() const { return aabb.lowerBound.y; }

          bool overlaps(const AABB&) const;
          bool overlapsX(const AABB&) const;
          bool overlapsY(const AABB&) const;

		  b2AABB getAABB() const { return aabb; }
        private:
		  b2AABB aabb;
        };
      }
    }
  }
}
