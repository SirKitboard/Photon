/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    IllegalFileFormatException.h
    */

#pragma once

#include <stdexcept>
#include <string>

namespace cse380 {
  namespace sssf {
    namespace game {
      using std::string;
      using std::exception;

      class IllegalFileFormatException : exception {
      public:
        // Represents an exception thrown when data from a file that is being
        // loaded is in the wrong format.
        IllegalFileFormatException(const string& errorMessage) {
          exception(errorMessage);
        }
      };
    }
  }
}
