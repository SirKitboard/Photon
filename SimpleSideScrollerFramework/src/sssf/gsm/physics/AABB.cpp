#include "sssf_VS/stdafx.h"
#include "sssf/gsm/physics/AABB.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace physics {
        bool AABB::overlaps(const AABB& other) const {
			return b2TestOverlap(aabb, other.getAABB());
        }
      }
    }
  }
}
