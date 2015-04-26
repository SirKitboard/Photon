/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    IllegalArgumentException.h
    */

#pragma once

#include <stdexcept>
#include <string>

namespace cse380 {
  namespace sssf {
    namespace game {
      using std::string;
      using std::invalid_argument;
      // Represents an exception thrown when an illegal argument is given to a method.
      class IllegalArgumentException : public invalid_argument {
      public:
        // INLINED CONSTRUCTOR
        IllegalArgumentException(const string& errorMessage)
          : invalid_argument(errorMessage) {

        }
      };
    }
  }
}
