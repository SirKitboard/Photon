#pragma once

#include <string>
#include <windows.h>

#include "sssf/os/GameOS.h"
#include "sssf/game/Game.h"
#include "sssf/graphics/GameGraphics.h"

namespace cse380 {
  namespace sssf {
    namespace win32 {
      using std::wstring;

      // THESE CONSTANTS ARE USED TO CONSTRUCT THE WINDOW
      // DEPENDING ON WHETHER WE WANT IT IN WINDOWED MODE
      // OR FULLSCREEN MODE
      const long windowedStyle = WS_OVERLAPPEDWINDOW;
      const long fullscreenStyle = WS_POPUP | WS_SYSMENU | WS_VISIBLE;

      /// <summary>
      /// Provides a Windows platform implementation of OS-message-to-game
      /// handling. This lets our program can handle all input accordingly.
      /// </summary>
      class WindowsOS : public os::GameOS {
        // 3 VARIABLES FROM Game
        bool fullscreen;

        // DATA USED TO INIT GAME DEVICES
        HINSTANCE hInstance;
        int nCmdShow;

        // windowHandle IS A POINTER TO THE APPLICATION
        // IT IS NEEDED BY DIRECTX TO CONSTRUCT CERTAIN
        // DEVICES, LIKE A GRAPHICS DEVICE
        HWND windowHandle;

        // wi HAS INFORMATION ABOUT THE WINDOW, LIKE THE
        // DIMENSIONS, AMONG OTHER THINGS
        WINDOWINFO wi;

        // applicationName STORES THE TEXT IN THE TITLE
        // OF THE WINDOW
        wstring applicationName;

      public:
        // INLINED ACCESSOR METHODS
        bool isFullscreen() const override { return fullscreen; }
        const HWND getWindowHandle() const { return windowHandle; }
        const WINDOWINFO& getWindowInfo() const { return wi; }

        // INLINED MUTATOR METHOD
        void setApplicationName(const wstring& initApplicationName) override {
          applicationName = initApplicationName;
          SetWindowText(windowHandle, applicationName.c_str());
        }

        // METHODS DEFINED IN WindowsOS.cpp
        WindowsOS(
          HINSTANCE,
          int,
          bool,
          const wstring&,
          int,
          int,
          game::Game*);
        virtual ~WindowsOS();
        WPARAM manageWindowsMessages();
        void processOSMessages();
        void shutdown();
      };
    }
  }
}
