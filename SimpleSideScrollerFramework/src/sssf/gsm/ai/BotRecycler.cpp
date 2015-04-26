#include "sssf_VS/stdafx.h"
#include "sssf/gsm/ai/BotRecycler.h"

#include "sssf/gsm/ai/Bot.h"

namespace cse380 {
  namespace sssf {
    namespace gsm {
      namespace ai {
        BotRecycler::BotRecycler() {}

        /*
            A BotRecycler is constructed only once while a game app is
            running, at startup. When it is deconstructed, it must delete
            all remaining bots.
            */
        BotRecycler::~BotRecycler() {
          this->clear();
        }

        /*
            It is assumed that a game will have many different types of Bots, thinking
            in different ways, some of them even custom bots defined by the game app rather
            than by this framework. So, in order to make these bots recyclable, we have to
            register them with this recycler so that this recycler knows how to make new
            ones as needed.
            */
        void BotRecycler::registerBotType(Bot* prototype, const int initial) {
          // FIRST ENSURE WE DON'T ALREADY HAVE RECYCLABLE BOTS FOR THIS TYPE.

          assert(prototype != nullptr);

          size_t count = botTypes.count(prototype);
          if (count) {
            // REGISTER THE BOT
            botTypes[prototype] = BotType(initial);
          }
        }

        bool BotRecycler::isBotTypeRegistered(Bot* type) const {
          // FIRST MAKE SURE THIS IS A REGISTERED BOT TYPE,
          // IF IT IS NOT, WE NEED TO RETURN NULL
          return type ? botTypes.count(type) > 0 : false;
        }

        Bot* BotRecycler::retrieveBot(Bot* type) {
          // FIRST MAKE SURE THIS IS A REGISTERED BOT TYPE,
          // IF IT IS NOT, WE CAN'T DO ANYTHING
          if (!type || !this->isBotTypeRegistered(type)) {
            return nullptr;
          }
          else {
            // GET THE CORRECT LIST OF BOT TYPES
            BotType& wanted = botTypes[type];

            if (wanted.pool.empty()) {
              // If we don't have any of this type of Bot to spare...
              this->addBots(type, RECYCLABLE_BOT_INCREMENT);
            }

            // NOW GET THE LAST ELEMENT
            Bot* bot = wanted.pool.back();
            wanted.pool.pop_back();
            wanted.active.push_back(bot);
            return bot;
          }
        }

        void BotRecycler::recycleBot(Bot* bot) {
          if (this->isBotTypeRegistered(bot)) {
            BotType& bots = botTypes[bot];

            assert(std::count(bots.pool.begin(), bots.pool.end(), bot) == 0);
            assert(std::count(bots.active.begin(), bots.active.end(), bot) == 1);
            // Ensure (at least in debug builds) that our bot is in the right
            // data structures!

            bots.pool.push_back(bot);
            std::remove(bots.active.begin(), bots.active.end(), bot);
          }
        }

        void BotRecycler::addBots(Bot* type, unsigned int amount) {
          // NOW MAKE THE BOTS
          BotType& botType = botTypes[type];
          botType.pool.reserve(botType.pool.size() + amount);

          for (unsigned int i = 0; i < amount; ++i) {
            Bot* clone = type->clone();
            botType.pool.push_back(clone);
          }
        }

        void BotRecycler::clear() {
          for (const auto& type : botTypes) {
            for (Bot* a : type.second.active) {
              assert(a != type.first);
              delete a;
            }

            for (Bot* p : type.second.pool) {
              assert(p != type.first);
              delete p;
            }

            delete type.first;
          }
        }
      }
    }
  }
}
