/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    TextFileReader.h
    */

#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace cse380 {
  namespace sssf {
    namespace text {
      using std::wstring;

      /// <summary>
      /// A wrapper class for reading text from a file. This can be useful for
      /// reading game design files for building a game world.
      /// </summary>
      class BufferedTextFileReader {
      public:
        BufferedTextFileReader();
        virtual ~BufferedTextFileReader();

        // No copying allowed!
        BufferedTextFileReader(const BufferedTextFileReader&) = delete;
        BufferedTextFileReader& operator=(const BufferedTextFileReader&) = delete;

        // INLINED ACCESSOR
        bool hasMoreLines() const {
          return moreTextToRead;
        }

        // METHODS DEFINED INSIDE BufferedTextFileReader
        void     closeReader();

        /// <summary>
        /// Reads a line of text from the file and returns it.  initFile must
        /// have been called for this method to return meaningful data.
        /// </summary>
        wstring getNextLine();
        void initFile(const wstring& initFileName);
      protected:
        // IS SOMETHING READY TO READ?
        bool moreTextToRead;

        // FILE NAME TO READ FROM
        wstring fileName;

        // STREAM TO WRITE TO
        std::wifstream inFile;

        // TEMP PLACE TO PUT DATA
        std::vector<wstring> buffer;

        // WE'LL USE THIS, BUT WE DON"T WANT ANYONE ELSE USING IT
        void loadBuffer();

        // THIS WILL KEEP TRACK OF WHICH LINES IN THE BUFFER
        // HAVE BEEN ACCESSED
        unsigned int lineCounter;
      };
    }
  }
}
