/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    WindowsTimer.cpp

    See WindowsTimer.h for a class description.
    */

#include "sssf_VS/stdafx.h"
#include "sssf/timer/GameTimer.h"

#include "sssf/game/Game.h"
#include "sssf/platforms/Windows/WindowsTimer.h"
#include "sssf/text/TextFileWriter.h"

namespace cse380 {
  namespace sssf {
    namespace win32 {
      /*
          WindowsTimer - Default constructor, this method
          initializes the timer resolution to the best the
          system has to offer. In order to start timing,
          call the resetTimer method outside the main game
          loop, right before it starts. The timeGameLoop
          method is the only one needed inside the game
          loop.
          */
      WindowsTimer::WindowsTimer(const uint16_t fps) : GameTimer(fps) {
        // GET INFORMATION ABOUT HOW GOOD THE SYSTEM'S TIMER IS
        timerResolution = this->getMinSystemTimerResolution();

        //  USE THAT RESOLUTION FOR ALL SUBSEQUENT TIMING
        timeBeginPeriod(timerResolution);
      }

      uint16_t WindowsTimer::getMinSystemTimerResolution() const {
        TIMECAPS timer;
        timeGetDevCaps(&timer, sizeof(TIMECAPS));
#pragma warning(suppress: 6102)
        UINT min = timer.wPeriodMin;
        return min;
      }

      void WindowsTimer::resetTimer() {
        gameLoopStartTime = timeGetTime();
        sleeplessGameLoopStartTime = timeGetTime();

        totalTime = 0;
        sleeplessTotalTime = 0;
        loopTime = 0;
        sleeplessLoopTime = 0;
        totalSleepTime = 0;
        gameLoopEndTime = 0;
        sleepTime = 0;

        GameTimer::resetTimer();
      }

      unsigned int WindowsTimer::calculateAverageActualFramerate() const {
        if (loopCounter == 0) {
          // If we haven't actually started the game loop yet...
          return 0;
        }
        else {
          uint64_t averageTime = totalTime / loopCounter;

          if (averageTime) {
            return 1000 / averageTime;
          }
          else {
            // IT'S AT LEAST 1000, BUT WE REALLY DON'T KNOW DUE TO TIMER RESOLUTION
            return 1000;
          }
        }
      }

      unsigned int WindowsTimer::calculateAverageSleepTime() const {
        return (loopCounter) ? totalSleepTime / loopCounter : 0;
      }

      unsigned int WindowsTimer::calculateAverageSleeplessFramerate() const {
        if (loopCounter == 0) {
          // If we haven't started the game loop yet...
          return 0;
        }
        else {
          float averageTime = static_cast<float>(sleeplessTotalTime) / loopCounter;
          if (round(averageTime) != 0) {
            return 1000 / averageTime;
          }
          else {
            // IT'S AT LEAST 1000, BUT WE REALLY DON'T KNOW DUE TO TIMER RESOLUTION
            return 1000;
          }
        }
      }

      void WindowsTimer::timeGameLoop() {
        // GET THE END OF FRAME TIME
        gameLoopEndTime = timeGetTime();

        // HOW MUCH TIME PASSED DURING THE LAST FRAME?
        loopTime = gameLoopEndTime - gameLoopStartTime;

        // GET THE START TIME FOR NEXT FRAME, IF THERE IS ONE
        gameLoopStartTime = timeGetTime();

        // ADD THE LAST FRAME'S TIME TO THE TOTAL
        totalTime += loopTime;

        // HOW MUCH TIME PASSED NOT INCLUDING OUR FORCED SLEEPING?
        sleeplessLoopTime = gameLoopEndTime - sleeplessGameLoopStartTime;

        // ADD THE LAST FRAME'S SLEEPLESS TIME TO THE TOTAL
        sleeplessTotalTime += sleeplessLoopTime;

        if (loopTime == 0)
          actualLoopRate = 1000;
        else
          actualLoopRate = 1000 / loopTime;

        if (sleeplessLoopTime == 0)
          actualLoopRate = 1000;
        else
          sleeplessLoopRate = 1000 / sleeplessLoopTime;

        // IF THIS PAST FRAME RAN TOO FAST THE NEXT FRAME WILL LIKELY RUN FAST ALSO
        if (targetMillisPerFrame > sleeplessLoopTime) {
          // SO LET'S CLAMP IT TO OUR TARGET FRAME RATE
          sleepTime = targetMillisPerFrame - sleeplessLoopTime;
          totalSleepTime += sleepTime;
          Sleep(sleepTime);
          timeScaleFactor = 1;
        }
        else {
          sleepTime = 0;

          // WE MIGHT USE THIS timeScaleFactor TO SCALE MOVEMENTS OF GAME SPRITES
          // AND SCROLLING TO MAKE UP FOR THE SLOWING DOWN OF THE GAME LOOP
          timeScaleFactor = ((float)targetFPS) /
            ((float)sleeplessLoopRate);
        }

        // GET THE START TIME FOR THE LOOP
        // NOT INCLUDING THE SLEEP TIME FROM THE LAST LOOP
        sleeplessGameLoopStartTime = timeGetTime();
        loopCounter++;
      }
    }
  }
}
