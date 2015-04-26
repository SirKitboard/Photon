#include "sssf_VS/stdafx.h"
#include "sssf/util/Util.h"

namespace cse380 {
  namespace sssf {
    namespace util {

      namespace random {
        std::random_device RANDOM_DEVICE;
        std::default_random_engine RANDOM(RANDOM_DEVICE());
      }

      std::string to_string(const std::wstring& w) {
        return std::string(w.begin(), w.end());
      }

      std::wstring to_wstring(const std::string& s) {
        return std::wstring(s.begin(), s.end());
      }
    }
  }
}