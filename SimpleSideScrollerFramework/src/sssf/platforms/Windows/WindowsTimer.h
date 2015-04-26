/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    WindowsTimer.h

    This class provides a Windows platform implementation
    for all game timing. You give it
    a target frame rate, and then once per frame cal
    timeGameLoop. This will clamp the frame rate to
    your target loop. If the loop is running slower
    than the target rate, you'll have to either settle
    for a slower game, which means you should improve
    performance in other parts of the game, or you should
    scale all game movements proportionally to the
    slowdown.

    This timer works by calculating the total time a given
    frame has taken and comparing it to the target framerate.
    It will then sleep for the difference if the measured
    framerate is faster than the target.
    */

#pragma once

#include <cstdint>

#include "sssf/timer/GameTimer.h"

namespace cse380 {
  namespace sssf {
    namespace win32 {
      class WindowsTimer : public time::GameTimer {
      public:
        // METHODS DEFINED IN WindowsTimer.cpp
        WindowsTimer(const uint16_t fps);
        WindowsTimer() : WindowsTimer(33) {}
        virtual ~WindowsTimer() {}

        uint16_t getMinSystemTimerResolution() const override;
        void    resetTimer() override;
        void    timeGameLoop() override;

        uint64_t getSleeplessTotalTime() const override { return sleeplessTotalTime; }
        uint64_t getTotalTime() const override { return totalTime; }
        uint64_t getLastFrameTime() const override { return loopTime; }
        uint64_t getSleeplessLastFrameTime() const override { return sleeplessLoopTime; }

        unsigned int calculateAverageActualFramerate() const override;
        unsigned int calculateAverageSleepTime() const override;
        unsigned int calculateAverageSleeplessFramerate() const override;
      protected:
        /// THE TIME TAKEN AT THE START OF THE FRAME, INCLUDING SLEEP TIME
        uint64_t gameLoopStartTime;

        /// THE TIME TAKEN AT THE START OF THE FRAME, NOT INCLUDING SLEEP TIME
        uint64_t sleeplessGameLoopStartTime;

        /// TOTAL TIME TAKEN SINCE ENTERING THE GAME LOOP, INCLUDING SLEEP TIME
        uint64_t totalTime;

        /// TOTAL TIME TAKEN SINCE ENTERING THE GAME LOOP, NOT INCLUDING SLEEP TIME
        uint64_t sleeplessTotalTime;

        /// THE TOTAL TIME A SINGLE FRAME TOOK TO RUN, INCLUDING SLEEP TIME
        uint64_t loopTime;

        /// THE TOTAL TIME A SINGLE FRAME TOOK TO RUN, NOT INCLUDING SLEEP TIME
        uint64_t sleeplessLoopTime;

        /// THE TOTAL AMOUNT OF TIME WE HAVE MADE OUR APPLICATION
        /// SLEEP SINCE THE TIMER WAS RESET
        uint64_t totalSleepTime;

        /// THE TIME TAKEN AT THE END OF THE FRAME
        uint64_t gameLoopEndTime;

        /// THE AMOUNT WE WILL SET TO SLEEP EACH FRAME IF THE GAME IS RUNNING
        /// FASTER THAN THE TARGET FPS
        uint64_t sleepTime;
      };
    }
  }
}
