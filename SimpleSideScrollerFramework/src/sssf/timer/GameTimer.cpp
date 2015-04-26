#include "sssf_VS/stdafx.h"
#include "sssf/timer/GameTimer.h"

namespace cse380 {
  namespace sssf {
    namespace time {
      GameTimer::GameTimer(const uint16_t fps) :
        targetFPS(fps),
        timeScaleFactor(1),
        loopCounter(0),
        timerResolution(1),
        actualLoopRate(0),
        sleeplessLoopRate(0) {
        // CALCULATE THE NUMBER OF MILLISECONDS WE
        // WANT PER FRAME ACCORDING TO THE TARGET_FPS
        targetMillisPerFrame = 1000 / fps;
      }

      void GameTimer::resetTimer() {
        timeScaleFactor = 1;
        loopCounter = 0;
        actualLoopRate = 0;
        sleeplessLoopRate = 0;
      }
    }
  }
}
