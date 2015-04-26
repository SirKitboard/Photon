/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    BufferedTextFileReader.cpp

    See BufferedTextFileReader.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/text/BufferedTextFileReader.h"

namespace cse380 {
  namespace sssf {
    namespace text {
      /*
          Default Constructor - We don't know the name of the file
          to use yet, so we'll initialize everything to NULL.
          */
      BufferedTextFileReader::BufferedTextFileReader() :
        moreTextToRead(false),
        lineCounter(0) {}

      /*
          Deconstructor - Make sure the file stuff doesn't cause
          a memory leak.
          */
      BufferedTextFileReader::~BufferedTextFileReader() {
        closeReader();
      }

      /*
          closeReader - This method closes the stream used for
          reading from a file and then deletes the inFile stream. If
          you want to read again from the stream, you must call initFile
          first, which will re-initialize everything.
          */
      void BufferedTextFileReader::closeReader() {
        inFile.close();
        moreTextToRead = false;
      }

      /*
          initFile - This method initializes the stream for reading
          from a file using the file name provided as an argument. After
          calling this method, we are ready to read text.
          */
      void BufferedTextFileReader::initFile(const wstring& initFileName) {
        fileName = initFileName;
        inFile.clear();
        inFile.open(std::string(fileName.begin(), fileName.end()));
        loadBuffer();
        moreTextToRead = true;
      }

      /*
          loadBuffer - This method reads all the text in the fileName file and
          loads it into the buffer, one line at a time.
          */
      void BufferedTextFileReader::loadBuffer() {
        for (wstring line; getline(inFile, line);) {
          buffer.push_back(line);
        }
        lineCounter = 0;
      }

      wstring BufferedTextFileReader::getNextLine() {
        if (moreTextToRead) {
          wstring lineOfText = buffer.at(lineCounter);
          lineCounter++;
          if (lineCounter == buffer.size())
            moreTextToRead = false;
          return lineOfText;
        }
        else {
          return L"";
        }
      }
    }
  }
}
