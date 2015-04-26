/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameTimer.h
    */

#pragma once

#include <cstdint>

namespace cse380 {
  namespace sssf {
    namespace time {
      /// <summary>
      /// <para>
      /// Provides all game timing. You give it a target frame rate, and then
      /// once per frame call timeGameLoop. This will clamp the frame rate to
      /// your target loop. If the loop is running slower than the target rate,
      /// you'll have to either settle for a slower game, which means you should
      /// improve performance in other parts of the game, or you should scale all
      /// game movements proportionally to the slowdown.
      /// </para>
      /// <para>
      /// This timer works by calculating the total time a given frame has taken and
      /// comparing it to the target framerate. It will then sleep for the difference
      /// if the measured framerate is faster than the target.
      /// </para>
      /// <para>
      /// Note that this class is just a framework. You should either provide a
      /// platform-specific implementation or use the StandardTimer class.
      /// </summary>
      class GameTimer {
      public:
        GameTimer(const uint16_t);
        GameTimer() : GameTimer(33) {}
        virtual ~GameTimer() {}

        // No copying allowed
        GameTimer(const GameTimer&) = delete;
        GameTimer& operator=(const GameTimer&) = delete;

        // METHODS DEFINED IN CHILD CLASSES, WHICH MAY BE PLATFORM SPECIFIC

        /// <summary>
        /// Returns (in milliseconds) how long on average a frame takes in
        /// the game loop, not including time spent sleeping.
        /// </summary>
        virtual unsigned int calculateAverageSleeplessFramerate() const = 0;

        /// <summary>
        /// Calculates (in milliseconds) how long on average a frame takes in
        /// the game loop, including time spent sleeping.
        /// </summary>
        virtual unsigned int calculateAverageActualFramerate() const = 0;

        /// <summary>
        /// Calculates (in milliseconds) on average how much time is spent
        /// sleeping each frame since the game loop started.
        /// </summary>
        virtual unsigned int calculateAverageSleepTime() const = 0;

        /// <summary>
        /// Asks the operating system and asks how precise the system clock is,
        /// in milliseconds. Typically, the best you can get is 1 millisecond.
        /// </summary>
        virtual uint16_t getMinSystemTimerResolution() const = 0;

        /// <summary>
        /// Resets all timing statistics. Call this once just before the game
        /// loop starts.
        /// </summary>
        virtual void resetTimer();

        /// <summary>
        /// Calculates how long it's been since this method was last called,
        /// then if necessary sleeps to prevent our game from running too fast.
        /// </summary>
        virtual void timeGameLoop() = 0;

        // INLINED METHODS
        uint64_t getLoopCounter() const { return loopCounter; }
        float getTimeScaleFactor() const { return timeScaleFactor; }
        uint16_t getTargetFPS() const { return targetFPS; }
        virtual uint64_t getSleeplessTotalTime() const = 0;
        virtual uint64_t getTotalTime() const = 0;
        virtual uint64_t getLastFrameTime() const = 0;
        virtual uint64_t getSleeplessLastFrameTime() const = 0;

        void setTargetFPS(const uint16_t initTargetFPS) {
          targetFPS = initTargetFPS;
          targetMillisPerFrame = 1000 / targetFPS;
        }
      protected:
        /// TARGET FRAMES PER SECOND
        uint16_t targetFPS;

        /// WHEN A LOOP RUNS TOO FAST, WE CLAMP OUR FRAME RATE SO THIS FACTOR IS
        /// SIMPLY 1, BUT WHEN OUR LOOP RUNS TOO SLOW, THIS INCREASES
        /// PROPORTIONATELY SUCH THAT WE CAN SCALE ALL MOVEMENTS OF SPRITES & SCROLLING
        float timeScaleFactor;

        /// NUMBER OF ITERATIONS, USED FOR CALCULATIONS
        uint64_t loopCounter;

        /// THE MINIMUM TIMER INCREMENT
        uint16_t timerResolution;

        /// THE FRAME RATE OF THE LATEST FRAME,
        /// INCLUDING AND NOT INCLUDING SLEEP TIME
        uint64_t actualLoopRate;
        uint64_t sleeplessLoopRate;

        /// THE INVERSE OF THE TARGET FRAME RATE IN MILLISECONDS
        uint16_t targetMillisPerFrame;
      };
    }
  }
}
