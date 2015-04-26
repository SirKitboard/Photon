/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    TextFileWriter.cpp

    See TextFileWriter.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/text/TextFileWriter.h"

namespace cse380 {
  namespace sssf {
    namespace text {
      TextFileWriter::TextFileWriter() {
        outStreamOpen = false;
      }

      TextFileWriter::~TextFileWriter() {
        closeWriter();
      }

      /*
      closeWriter - This method closes the stream used for
      reading from a file and then deletes the inFile stream. If
      you want to read again from the stream, you must call initFile
      first, which will re-initialize everything.
      */
      void TextFileWriter::closeWriter() {
        if (outStreamOpen) {
          outFile.flush();
          outFile.close();
        }
      }

      /*
          initFile - This method initializes the stream for writing
          to a file using the file name provided as an argument. After
          calling this method, we are ready to write text.
          */
      void TextFileWriter::initFile(const string& initFileName) {
        fileName = initFileName;
        outFile.open(initFileName);
        outStreamOpen = true;
      }

      /*
          writeText - This method appends the textToWrite argument
          to the end of the text file this object writes to.
          */
      void TextFileWriter::writeText(const string& textToWrite) {
        outFile << textToWrite;
      }
    }
  }
}
