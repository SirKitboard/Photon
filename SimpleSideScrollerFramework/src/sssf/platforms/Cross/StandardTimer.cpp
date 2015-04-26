#include "sssf_VS/stdafx.h"
#include "sssf/platforms/Cross/StandardTimer.h"

#include <chrono>
#include <thread>

namespace cse380 {
  namespace sssf {
    namespace cpp {
      using std::chrono::milliseconds;
      using std::chrono::duration_cast;

      const StandardTimer::ClockType::duration StandardTimer::ZERO =
        StandardTimer::ClockType::duration::zero();

      const StandardTimer::ClockType::duration StandardTimer::ONE =
        StandardTimer::ClockType::duration(1);

      StandardTimer::StandardTimer(const uint16_t fps) : GameTimer(fps) {
        // GET INFORMATION ABOUT HOW GOOD THE SYSTEM'S TIMER IS
        timerResolution = this->getMinSystemTimerResolution();
      }

      uint16_t StandardTimer::getMinSystemTimerResolution() const {
        // std::chrono::steady_clock is always increasing
        return ClockType::period::num;
        // steady_clock::period is a ratio object that describes the clock
        // period in seconds; multiply it by 1000 to get it in milliseconds
      }

      void StandardTimer::resetTimer() {
        ClockType::time_point now = ClockType::now();
        gameLoopStartTime = now;
        sleeplessGameLoopStartTime = now;

        GameTimer::resetTimer();
      }

      uint64_t StandardTimer::getSleeplessTotalTime() const {
        return duration_cast<milliseconds>(this->sleeplessTotalTime).count();
      }
      uint64_t StandardTimer::getTotalTime() const {
        return duration_cast<milliseconds>(this->totalTime).count();
      }

      uint64_t StandardTimer::getLastFrameTime() const {
        return duration_cast<milliseconds>(this->loopTime).count();
      }
      uint64_t StandardTimer::getSleeplessLastFrameTime() const {
        return duration_cast<milliseconds>(this->sleeplessLoopTime).count();
      }

      unsigned int StandardTimer::calculateAverageActualFramerate() const {
        if (loopCounter == 0) {
          // If we haven't actually started the game loop yet...
          return 0;
        }
        else {
          ClockType::duration averageTime = duration_cast<milliseconds>(totalTime / loopCounter);

          if (averageTime > ZERO) {
            return ClockType::period::den / averageTime.count();
          }
          else {
            return ClockType::period::den;
          }
        }
      }

      unsigned int StandardTimer::calculateAverageSleepTime() const {
        return (loopCounter) ? duration_cast<milliseconds>(totalSleepTime).count() / loopCounter : 0;
      }

      unsigned int StandardTimer::calculateAverageSleeplessFramerate() const {
        if (loopCounter == 0) {
          // If we haven't started the game loop yet...
          return 0;
        }
        else {
          float averageTime = sleeplessTotalTime.count() / static_cast<float>(loopCounter);
          if (round(averageTime) != 0) {
            return ClockType::period::den / averageTime;
          }
          else {
            // IT'S AT LEAST 1000, BUT WE REALLY DON'T KNOW DUE TO TIMER RESOLUTION
            return ClockType::period::den;
          }
        }
      }

      void StandardTimer::timeGameLoop() {    // GET THE END OF FRAME TIME
        using namespace std::chrono;
        using std::this_thread::sleep_for;
        ClockType::time_point now = ClockType::now();

        gameLoopEndTime = now;

        // HOW MUCH TIME PASSED DURING THE LAST FRAME?
        loopTime = gameLoopEndTime - gameLoopStartTime;

        // GET THE START TIME FOR NEXT FRAME, IF THERE IS ONE
        gameLoopStartTime = now;

        // ADD THE LAST FRAME'S TIME TO THE TOTAL
        totalTime += loopTime;

        // HOW MUCH TIME PASSED NOT INCLUDING OUR FORCED SLEEPING?
        sleeplessLoopTime = gameLoopEndTime - sleeplessGameLoopStartTime;

        // ADD THE LAST FRAME'S SLEEPLESS TIME TO THE TOTAL
        sleeplessTotalTime += sleeplessLoopTime;

        if (loopTime == ClockType::duration::zero()) {
          actualLoopRate = ClockType::period::den;
        }
        else {
          actualLoopRate = ClockType::period::den / loopTime.count();
        }

        actualLoopRate = (loopTime > ZERO) ? ClockType::period::den / loopTime.count() : ClockType::period::den;

        if (sleeplessLoopTime == ZERO) {
          sleeplessLoopRate = ClockType::period::den;
        }
        else {
          sleeplessLoopRate = ClockType::period::den / sleeplessLoopTime.count();
        }


        // IF THIS PAST FRAME RAN TOO FAST IT'S LIKELY THE NEXT FRAME WILL RUN FAST ALSO
        if (milliseconds(targetMillisPerFrame) > duration_cast<milliseconds>(sleeplessLoopTime)) {
          // SO LET'S CLAMP IT TO OUR TARGET FRAME RATE
          sleepTime = milliseconds(targetMillisPerFrame) - sleeplessLoopTime;
          totalSleepTime += sleepTime;

          sleep_for(sleepTime);
          timeScaleFactor = 1;
        }
        else {
          sleepTime = ZERO;

          // WE MIGHT USE THIS timeScaleFactor TO SCALE MOVEMENTS OF GAME SPRITES
          // AND SCROLLING TO MAKE UP FOR THE SLOWING DOWN OF THE GAME LOOP
          timeScaleFactor = static_cast<float>(targetFPS) / sleeplessLoopRate;
        }

        // GET THE START TIME FOR THE LOOP
        // NOT INCLUDING THE SLEEP TIME FROM THE LAST LOOP
        sleeplessGameLoopStartTime = ClockType::now();
        loopCounter++;
      }
    }
  }
}
