#ifndef STANDARDTIMER_H
#define STANDARDTIMER_H

#include "sssf/timer/GameTimer.h"

#include <chrono>

namespace cse380 {
  namespace sssf {
    namespace cpp {
      /// <summary>
      /// An implementation of GameTimer that uses the chrono and thread libraries
      /// in the C++11 standard.
      /// </summary>
      class StandardTimer : public time::GameTimer {
      public:
        StandardTimer(const uint16_t fps);
        StandardTimer() : StandardTimer(33) {}
        virtual ~StandardTimer() {}
        uint16_t getMinSystemTimerResolution() const override;
        void resetTimer() override;
        void timeGameLoop() override;

        unsigned int calculateAverageActualFramerate() const override;
        unsigned int calculateAverageSleepTime() const override;
        unsigned int calculateAverageSleeplessFramerate() const override;
        uint64_t getSleeplessTotalTime() const override;
        uint64_t getTotalTime() const override;
        uint64_t getLastFrameTime() const override;
        uint64_t getSleeplessLastFrameTime() const override;
      private:
        typedef std::chrono::steady_clock ClockType;
        // ^ Typedef in case you want to use system_clock or high_resolution_clock

        static const ClockType::duration ZERO;
        static const ClockType::duration ONE;

        ClockType::time_point _start;

        /// THE TIME TAKEN AT THE START OF THE FRAME, INCLUDING SLEEP TIME
        ClockType::time_point gameLoopStartTime;

        /// THE TIME TAKEN AT THE START OF THE FRAME, NOT INCLUDING SLEEP TIME
        ClockType::time_point sleeplessGameLoopStartTime;

        /// THE TIME TAKEN AT THE END OF THE FRAME
        ClockType::time_point gameLoopEndTime;

        /// TOTAL TIME TAKEN SINCE ENTERING THE GAME LOOP, INCLUDING SLEEP TIME
        ClockType::duration totalTime;

        /// TOTAL TIME TAKEN SINCE ENTERING THE GAME LOOP, NOT INCLUDING SLEEP TIME
        ClockType::duration sleeplessTotalTime;

        /// THE TOTAL TIME A SINGLE FRAME TOOK TO RUN, INCLUDING SLEEP TIME
        ClockType::duration loopTime;

        /// THE TOTAL TIME A SINGLE FRAME TOOK TO RUN, NOT INCLUDING SLEEP TIME
        ClockType::duration sleeplessLoopTime;

        /// THE TOTAL AMOUNT OF TIME WE HAVE MADE OUR APPLICATION
        /// SLEEP SINCE THE TIMER WAS RESET
        ClockType::duration totalSleepTime;

        /// THE AMOUNT WE WILL SET TO SLEEP EACH FRAME IF THE GAME IS RUNNING
        /// FASTER THAN THE TARGET FPS
        ClockType::duration sleepTime;
      };
    }
  }
}
#endif // STANDARDTIMER_H
