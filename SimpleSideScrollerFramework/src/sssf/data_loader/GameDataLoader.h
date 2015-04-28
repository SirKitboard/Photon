/*
    Author: Richard McKenna
    Stony Brook University
    Computer Science Department

    GameDataLoader.h
    */

#pragma once

#include <string>
#include <unordered_map>

const static wchar_t DEFAULT_DELIMITER = ':';


namespace cse380 {
  namespace sssf {

    namespace game { class Game; }

    namespace data {

      using std::wstring;
      using std::unordered_map;
      using game::Game;

      /// <summary>
      /// Reads data from files and uses them to load game data for a game. Data
      /// for rendering the game GUI is loaded when the game starts, data for
      /// displaying each level would be loaded as needed.
      /// </summary>
      class GameDataLoader {
      public:
        // SIMPLE INLINED CONSTRUCTOR/DESTRUCTOR
        GameDataLoader() {}
        virtual ~GameDataLoader() {}

        // No copying allowed!
        GameDataLoader(const GameDataLoader&) = delete;
        GameDataLoader& operator=(const GameDataLoader&) = delete;

        // ACCESSOR AND MUTATOR
        const wstring& getGameDataPathName() const {
          return gameDataPathName;
        }

        void setGameDataPathName(const wstring& path) { gameDataPathName = path; }

        // METHOD DEFINED IN GameDataLoader.cpp

        /// <summary>
        /// Loads all the delimited properties into the map argument, which the
        /// calling method may then take and use to setup the game application.
        /// Should only be called once, when the game begins.
        /// </summary>
        void loadGameProperties(Game* game,
          unordered_map<wstring, wstring>& properties,
          const wstring& gameInitFile);

        // Loads the line argument as a (name, value) pair in to the map
        void loadProperty(const wstring&, unordered_map<wstring, wstring>&);

        // METHODS TO BE DEFINED INSIDE CHILD CLASSES
        virtual void loadGame(Game*, const wstring&) = 0;
        virtual void loadGUI(Game*, const wstring&) = 0;
        virtual void loadWorld(Game*, const wstring&) = 0;
		virtual void loadLevel1(Game*) = 0;
		virtual void loadLevel2(Game*) = 0;
		virtual void loadLevel3(Game*) = 0;

      private:
        // THIS IS THE LOCATION OF ALL GAME DATA TO BE LOADED
        wstring gameDataPathName;
      };
    }
  }
}
