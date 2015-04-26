#pragma once

#include <algorithm>
#include <cassert>
#include <vector>

#include "sssf/input/KeyEventHandler.h"

namespace cse380 {
  namespace sssf {
    namespace input {

      /// <summary>
      /// A multiplexer for KeyEventHandlers; swap them out at will, or just
      /// split up functionality so you don't have a big-ass handler. This class
      /// does NOT own the KeyEventHandlers passed into it; that is, it won't
      /// delete them when it itself dies.
      /// </summary>
      class MultiKeyEventHandler : public KeyEventHandler {
      public:
        MultiKeyEventHandler() {}
        ~MultiKeyEventHandler() {}

        void addHandler(KeyEventHandler* handler) {
          assert(handler != nullptr);
          this->_handlers.push_back(handler);
        }

        void removeHandler(KeyEventHandler* handler) {
          this->_handlers.erase(
            std::find(_handlers.begin(), _handlers.end(), handler)
            );
        }

        void clear() {
          this->_handlers.clear();
        }

        void handleKeyEvents(game::Game* game) override;
      private:
        std::vector<KeyEventHandler*> _handlers;
      };
    }
  }
}
