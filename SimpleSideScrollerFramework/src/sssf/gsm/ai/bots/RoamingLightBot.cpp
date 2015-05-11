#include "sssf_VS/stdafx.h"
#include "sssf/gsm/ai/bots/RoamingLightBot.h"

#include "sssf/game/Game.h"
#include "sssf/gsm/physics/Physics.h"
#include "sssf/gsm/state/GameStateManager.h"
#include <LuaPlusFramework/LuaObject.h>
#include <time.h>

namespace cse380 {
	namespace sssf {
		namespace gsm {
			namespace ai {
				namespace bots {
					uniform_real_distribution<float> RoamingLightBot::_random(0, 2 * PI);
					using game::Game;
					using gsm::state::GameStateManager;
					using physics::Physics;

					// This private constructor is only to be used for cloning bots, note
					// that this does not setup the velocity for this bot.
					RoamingLightBot::RoamingLightBot(unsigned int initMin,
						unsigned int initMax,
						unsigned int initMaxVelocity,
						unsigned int direction) {
						// INIT THE BASIC STUFF
						initBot(initMin, initMax, initMaxVelocity, direction);
						this->getPhysicalProperties().setUserData(new char('s'));
					}

					/*
					This is the public constructor used by other classes for
					creating these types of bots.
					*/
					RoamingLightBot::RoamingLightBot(Game* game,
						unsigned int initMin,
						unsigned int initMax,
						unsigned int initMaxVelocity,
						unsigned int direction) {
						// INIT THE BASIC STUFF
						destinations[0].x = 200; destinations[0].y = 300;
						destinations[1].x = 1000; destinations[1].y = 350;
						destinations[2].x = 1200; destinations[2].y = 1350;
						destinations[3].x = 120; destinations[3].y = 1250;
						destinations[4].x = 220; destinations[4].y = 1250;
						destination = rand() % 5;
						origin = 0;
						followPath = true;
						reachedDest = true;
						initBot(initMin, initMax, initMaxVelocity,direction);

						// AND START THE BOT OFF IN A RANDOM DIRECTION AND VELOCITY
						// AND WITH RANDOM INTERVAL UNTIL IT THINKS AGAIN
						//pickRandomVelocity(game->getGSM().getPhysics());
						//pickRandomCyclesInRange();
					}

					/*
					clone - this method makes another RandomFloatingBot object, but does
					not completely initialize it with similar data to this. Most of the
					object, like velocity and position, are left uninitialized.
					*/
					Bot* RoamingLightBot::clone() const {
						RoamingLightBot* botClone = new RoamingLightBot(minCyclesBeforeThinking,
							maxCyclesBeforeThinking,
							maxVelocity, 
							direction);
						return botClone;
					}

					/*
					initBot - this initialization method sets up the basic bot
					properties, but does not setup its velocity.
					*/
					void RoamingLightBot::initBot(unsigned int initMin,
						unsigned int initMax,
						unsigned int initMaxVelocity,
						unsigned int direction) {
						this->direction = direction;
						// IF THE MAX IS SMALLER THAN THE MIN, SWITCH THEM
						if (initMax < initMin) {
							unsigned int temp = initMax;
							initMax = initMin;
							initMin = temp;
						}
						// IF THEY ARE THE SAME, ADD 100 CYCLES TO THE MAX
						else if (initMax == initMin)
							initMax += 100;

						// INIT OUR RANGE VARIABLES
						minCyclesBeforeThinking = initMin;
						maxCyclesBeforeThinking = initMax;

						// AND OUR VELOCITY CAPPER
						maxVelocity = initMaxVelocity;
					}

					void RoamingLightBot::rotate()
					{
						float vX = this->getPhysicalProperties().getVelocityX();
						float vY = this->getPhysicalProperties().getVelocityY();
						this->getPhysicalProperties().setVelocity(-vY, vX);
					}

					/*
					pickRandomCyclesInRange - a randomized method for determining when this bot
					will think again. This method sets that value.
					*/
					void RoamingLightBot::pickRandomCyclesInRange() {
						cyclesRemainingBeforeThinking = maxCyclesBeforeThinking -
							minCyclesBeforeThinking + 1;
						cyclesRemainingBeforeThinking = rand() % cyclesRemainingBeforeThinking;
						cyclesRemainingBeforeThinking += minCyclesBeforeThinking;
					}

					/*
					pickRandomVelocity - calculates a random velocity vector for this
					bot and initializes the appropriate instance variables.
					*/
					void RoamingLightBot::pickRandomVelocity(Physics& physics) {
						// FIRST GET A RANDOM float FROM 0.0 TO 1.0
						float randomAngleInRadians = ((float)rand()) / ((float)RAND_MAX);

						// NOW SCALE IT FROM 0 TO 2 PI
						randomAngleInRadians *= 2.0f;
						randomAngleInRadians *= PI;

						// NOW WE CAN SCALE OUR X AND Y VELOCITIES
						this->pp.setVelocity(maxVelocity * sin(randomAngleInRadians),
							maxVelocity * cos(randomAngleInRadians));
					}

					/*
					think - called once per frame, this is where the bot performs its
					decision-making. Note that we might not actually do any thinking each
					frame, depending on the value of cyclesRemainingBeforeThinking.
					*/
					bool inRange(physics::PhysicalProperties* ppb, physics::PhysicalProperties* ppp, int maxDist) {
						int x1 = ppb->getX();
						int x2 = ppp->getX();
						int y1 = ppb->getY();
						int y2 = ppp->getY();

						float dist = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
						if (dist < maxDist) {
							return true;
						}
						return false;
					}

					void RoamingLightBot::think(Game* game) {
						physics::PhysicalProperties* pp = &getPhysicalProperties();
						LuaPlus::LuaState* luaState = game->getLuaState();
						LuaPlus::LuaObject luaVelocity = luaState->GetGlobal("botVelocity");
						LuaPlus::LuaObject luaDisatnce = luaState->GetGlobal("maxDistance");
						//int incre = luaVelocity.GetInteger();
						int velx = 0;
						int vely = 0;
						int botVelocity = luaVelocity.GetInteger();
						int maxDist = luaDisatnce.GetInteger();
						physics::PhysicalProperties* player = &(game->getGSM().getSpriteManager().getPlayer().getPhysicalProperties());
						if (followPath) {
							followPath = !inRange(pp, player,maxDist);
						}
						if (followPath) {
							followPath = !inRange(pp, player,maxDist);
							if (pp->getX() > destinations[destination].x + 20) {
								velx -= botVelocity;
							}
							else if (pp->getX() < destinations[destination].x - 20) {
								velx += botVelocity;
							}

							if (pp->getY() > destinations[destination].y + 20) {
								vely -= botVelocity;
							}
							else if (pp->getY() < destinations[destination].y - 20) {
								vely += botVelocity;
							}

							if (velx == 0 && vely == 0) {
								origin = destination;
								destination = rand() % 5;
							}
			
						}
						else {
							if (pp->getX() > player->getX() + 20) {
								velx -= botVelocity;
							}
							else if (pp->getX() < player->getX() - 20) {
								velx += botVelocity;
							}

							if (pp->getY() > player->getY() + 20) {
								vely -= botVelocity;
							}
							else if (pp->getY() < player->getY() - 20) {
								vely += botVelocity;
							}
						}
						pp->setVelocity(velx, vely);
						
						// EACH FRAME WE'LL TEST THIS BOT TO SEE IF WE NEED
						// TO PICK A DIFFERENT DIRECTION TO FLOAT IN

//						if (cyclesRemainingBeforeThinking == 0) {
//							LuaPlus::LuaState* luaState = game->getLuaState();
//						}
//						else
//							cyclesRemainingBeforeThinking--;
					}
				}
			}
		}
	}
}
