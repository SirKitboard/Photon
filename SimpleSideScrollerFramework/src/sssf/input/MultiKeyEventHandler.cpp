#include "sssf_VS/stdafx.h"
#include "sssf/input/MultiKeyEventHandler.h"

namespace cse380 {
  namespace sssf {
    namespace input {
      void MultiKeyEventHandler::handleKeyEvents(game::Game* game) {
        for (KeyEventHandler* handler : this->_handlers) {
          handler->handleKeyEvents(game);
        }
      }
    }
  }
}