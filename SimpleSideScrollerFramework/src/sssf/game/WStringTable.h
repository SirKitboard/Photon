/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    WStringTable.h
    */

#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace cse380 {
  namespace sssf {
    namespace game {
      using std::vector;
      using std::wstring;

      /// <summary>
      /// Manages an array of wstrings in a vector. Useful for storing the names
      /// of loaded resources so that all the game objects don't have to; they
      /// can instead just store the indices of the resources in this table in
      /// order to save memory and ask for the resource's ID when needed.
      /// </summary>
      /// <remarks>
      /// Don't assume that the IDs will be given in increasing order; if you do,
      /// then if you change the implementation for WStringTable you'll break
      /// lots of other things.
      /// </remarks>
      class WStringTable {
      public:
        // INLINED ACCESSOR METHOD
        const wstring& getWStringFromTable(const size_t index) const {
          return wStringTable[index];
        }

        // Copying allowed, but only if you explicitly call the copy ctor
        explicit WStringTable(const WStringTable&) = default;

        // NO DATA TO INITIALIZE OR DESTROY
        WStringTable() {}
        ~WStringTable() {}

        // METHODS DEFINED IN WStringTable.cpp

        /**
         * Clears out the string table vector, deleting all of the contents.
         */
        void clear();

        /**
         * Returns true if the given wstring is already in this string table.
         */
        bool isInTable(const wstring& testWString) const;

        /**
         * Searches for the key parameter in the wstring table. If found, it
         * returns its index, if not found it returns the max size_t value.
         */
        size_t getIndexOfWString(const wstring& key) const;

        /// <summary>
        /// Adds an element to the wstring table. Since the wstring table doesn't
        /// allow duplicate elements, this method first checks to see if the key
        /// wstring is already inside. If it is, it simply returns its index. If
        /// it's not found, it adds it and returns the index of the location
        /// where it was added.
        /// </summary>
        size_t putWStringInTable(const wstring& key);

        size_t getNumWStringsInTable() const { return wStringTable.size(); }

        static const size_t NOT_FOUND;

      private:
        vector<wstring> wStringTable;
      };
    }
  }
}
