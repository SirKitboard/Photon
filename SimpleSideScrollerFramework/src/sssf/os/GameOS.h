/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameOS.h
    */

#pragma once

#include <string>

namespace cse380 {
  namespace sssf {
    namespace os {

      /// <summary>
      /// Provides a framework for managing OS messages between this application
      /// and the operating system. Each platform would have its own implementation.
      /// </summary>
      class GameOS {
      public:
        virtual ~GameOS() {}
        virtual void processOSMessages() = 0;
        virtual void shutdown() = 0;
        virtual bool isFullscreen() const = 0;
        // The const at the end means this method won't modify the GameOS
        virtual void setApplicationName(const std::wstring&) = 0;
      };
    }
  }
}
