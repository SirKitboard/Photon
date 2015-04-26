
#pragma once

#include <cstdint>
#include <functional>
#include <unordered_map>
#include <vector>

#include "sssf/gsm/ai/Bot.h"

static const unsigned int RECYCLABLE_BOT_INCREMENT = 10;

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace ai {
        using std::unordered_map;
        using std::vector;
        using std::size_t;

        /// <summary>
        /// Provides premade lists of commonly used bots to avoid dynamic
        /// construction during gameplay.
        /// </summary>
        class BotRecycler {
        public:
          BotRecycler();

          // No copying allowed
          BotRecycler(const BotRecycler&) = delete;
          BotRecycler& operator=(const BotRecycler&) = delete;

          ~BotRecycler();
          void addBots(Bot*, const unsigned int);
          bool isBotTypeRegistered(Bot*) const;
          void recycleBot(Bot*);
          void registerBotType(Bot*, const int = RECYCLABLE_BOT_INCREMENT);
          Bot* retrieveBot(Bot*);
          void clear();
        private:
          struct BotType {
            /// Bots that are on reserve
            vector<Bot*> pool;

            /// Bots that are in use (a Bot should never be in both lists at once)
            vector<Bot*> active;

            BotType(const int count = RECYCLABLE_BOT_INCREMENT) {
              pool.reserve(count);
              active.reserve(count);
            }
          };

          struct BotHash {
            typedef Bot* argument_type;
            typedef std::size_t result_type;

            result_type operator()(argument_type const& s) const {
              if (s == nullptr) {
                return 0;
              }
              else {
                return _hash(s->type());
              }
            }
          private:
            std::hash<size_t> _hash;
          };

          unordered_map<Bot*, BotType, BotHash> botTypes;
        };
      }
    }
  }
}
