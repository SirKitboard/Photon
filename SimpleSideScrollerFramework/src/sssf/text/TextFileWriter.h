/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    TextFileWriter.h
    */

#pragma once

#include <fstream>
#include <string>

namespace cse380 {
  namespace sssf {
    namespace text {
      using std::string;
      /**
       A wrapper class for writing text to a file. Use the init method to
       initialize the stream. Text can then be written to the file using the
       writeText method. Re-init the object to write to a new file. Also,
       remember to delete pointers of this object when done.
       */
      class TextFileWriter {
      public:
        // METHODS DEFINED IN TextFileWriter.cpp

        /**
         This constructor doesn't initialize a stream, so it just marks that it
         cannot currently write.
         */
        TextFileWriter();

        /**
         Makes sure the text stream is closed.
         */
        ~TextFileWriter();

        // No copying allowed!
        TextFileWriter(const TextFileWriter&) = delete;
        TextFileWriter& operator=(const TextFileWriter&) = delete;

        void closeWriter();
        void initFile(const string& initFileName);
        void writeText(const string& textToWrite);
      private:
        // THE NAME OF THE TEXT FILE THIS OBJECT WILL WRITE TO
        string fileName;

        // THE OUTPUT STREAM WE WILL USE TO WRITE
        std::ofstream outFile;

        // THIS KEEPS TRACK OF WHEN THE STREAM IS ALL SETUP AND IT'S SAFE TO WRITE TEXT
        bool outStreamOpen;
      };
    }
  }
}
