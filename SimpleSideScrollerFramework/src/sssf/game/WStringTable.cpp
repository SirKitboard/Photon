/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    WStringTable.cpp

    See WStringTable.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/game/WStringTable.h"

namespace cse380 {
  namespace sssf {
    namespace game {
      using std::numeric_limits;
      using std::size_t;

      const size_t WStringTable::NOT_FOUND = numeric_limits<size_t>::max();

      void WStringTable::clear() {
        wStringTable.clear();
      }

      size_t WStringTable::getIndexOfWString(const wstring& key) const {
        size_t counter = 0;
        for (const wstring& w : wStringTable) {
          if (w == key) return counter;
          ++counter;
        }

        return numeric_limits<size_t>::max();
      }

      bool WStringTable::isInTable(const wstring& testWString) const {
        size_t index = getIndexOfWString(testWString);
        return (index < wStringTable.size());
      }

      size_t WStringTable::putWStringInTable(const wstring& key) {
        size_t index = getIndexOfWString(key);
        if (index < wStringTable.size()) return index;

        wStringTable.push_back(key);
        return wStringTable.size() - 1;
      }
    }
  }
}
