#include "sssf_VS/stdafx.h"
#include "sssf/graphics/Light.h"
#include "sssf/game/Game.h"
#include "sssf/graphics/RaycastCallback.h"
//#include "sssf/gui/Viewport.h"

const float TWOPI = PI*2.0f;

namespace cse380 {
	namespace sssf {
		namespace graphics {
			using std::vector;

			Light::Light(Game* game,
				b2Vec2 position,
				float reach,
				float angle,
				float angleSpread,
				bool active)
			{
				this->game = game;
				this->position = position;
				this->reach = reach;
				this->angle = normalizeAngle(angle);
				this->halfAngleSpread = normalizeAngle(angleSpread / 2.0f);
				this->active = active;
			}

			float Light::normalizeAngle(float angle)
			{
				if (angle > 0)
				{
					return fmod(angle, TWOPI);
				}
				else if (angle < 0)
				{
					return TWOPI - fabs(fmod(angle, TWOPI));
				} 
				else
				{
					return 0.0f;
				}
			}

			bool Light::angleIsLess(b2Vec2 center, b2Vec2 a, b2Vec2 b)
			{
				if (a.x - center.x >= 0 && b.x - center.x < 0)
					return true;
				if (a.x - center.x < 0 && b.x - center.x >= 0)
					return false;
				if (a.x - center.x == 0 && b.x - center.x == 0) {
					if (a.y - center.y >= 0 || b.y - center.y >= 0)
						return a.y > b.y;
					return b.y > a.y;
				}

				// compute the cross product of vectors (center -> a) x (center -> b)
				float det = (a.x - center.x) * (b.y - center.y) - (b.x - center.x) * (a.y - center.y);
				if (det < 0)
					return true;
				if (det > 0)
					return false;

				// points a and b are on the same line from the center
				// check which point is closer to the center
				float d1 = (a.x - center.x) * (a.x - center.x) + (a.y - center.y) * (a.y - center.y);
				float d2 = (b.x - center.x) * (b.x - center.x) + (b.y - center.y) * (b.y - center.y);
				return d1 > d2;
			}

			bool Light::angleIsLessFinal(b2Vec2 center, b2Vec2 a, b2Vec2 b)
			{
				float left = normalizeAngle(this->angle - this->halfAngleSpread);
				float right = normalizeAngle(this->angle + this->halfAngleSpread);
				if (left < right)
				{
					return getPointAngle(center, a) <= getPointAngle(center, b);
				}
				else
				{
					float pA = getPointAngle(center, a);
					float pB = getPointAngle(center, b);
					if (pB <= right)
					{
						if (pA <= right)
						{
							return pA <= pB;
						}
						else
						{
							return true;
						}
					}
					else
					{
						if (pA >= left)
						{
							return pA <= pB;
						}
						else
						{
							return false;
						}
					}
				}
			}

			float Light::getPointAngle(b2Vec2 center, b2Vec2 point)
			{
				b2Vec2 polarISHCoor = point - center;
				float angle = atan2(polarISHCoor.x, polarISHCoor.y);
				if (angle < 0)
				{
					return TWOPI + angle;
				}
				else
				{
					return angle;
				}
			}

			bool Light::pointInAngleRange(b2Vec2 center, float angle, float halfAngleSpread, b2Vec2 point)
			{
				float left = normalizeAngle(angle - halfAngleSpread);
				float right = normalizeAngle(angle + halfAngleSpread);
				float pointAngle = getPointAngle(center, point);
				if (left < right){
					return (pointAngle>left && pointAngle<right);
				}
				else
				{
					return ((pointAngle>left && pointAngle>right) || (pointAngle<right && pointAngle<left));
				}
				
			}

			b2Vec2 Light::generatePointFromAngle(b2Vec2 center, float reach, float angle)
			{
				angle -= PI / 2.0f;
				angle = normalizeAngle(angle);
				float32 y = (center.y) + (reach*sin(angle));
				float32 x = (center.x) + (reach*cos(angle));
				return b2Vec2(x, y);
			}

			void Light::prePopulatePoints(std::vector<b2Vec2>* points, float angle, float halfAngleSpread, int resolution)
			{
				float left = normalizeAngle(angle - halfAngleSpread);
				float right = normalizeAngle(angle + halfAngleSpread);
				float step = (2.0f*halfAngleSpread) / (float)resolution;
				if (halfAngleSpread < PI){
					b2Vec2 leftPoint = generatePointFromAngle(this->position, this->reach, left);
					points->push_back(leftPoint);
					for (int i = 1; i < resolution - 1; i++){
						b2Vec2 point = generatePointFromAngle(this->position, this->reach, normalizeAngle(left + (i*step)));
						points->push_back(point);
					}
					b2Vec2 rightPoint = generatePointFromAngle(this->position, this->reach, right);
					points->push_back(rightPoint);
				}
				else{
					b2Vec2 leftPoint = generatePointFromAngle(this->position, this->reach, 0.0f);
					points->push_back(leftPoint);
					for (int i = 1; i < resolution - 1; i++){
						b2Vec2 point = generatePointFromAngle(this->position, this->reach, normalizeAngle(0.0f + (i*(2*PI)/resolution)));
						points->push_back(point);
					}
					b2Vec2 rightPoint = generatePointFromAngle(this->position, this->reach, 0.0f);
					points->push_back(rightPoint);
				}
			}

			void Light::sortPoints(std::vector<b2Vec2>* points, int head, int tail)
			{
				/*
				std::random_device rd;
				std::mt19937 gen(rd());
				std::uniform_int_distribution<> dis(0, points->size);
				int r = dis(gen);
				*/
				std::vector<b2Vec2> pointV = *points;
				int r;
				if (head < tail)
				{
					b2Vec2 x = pointV[head];
					int i = head;
					int j;

					for (j = head + 1; j < tail; j++)
					{
						if (angleIsLessFinal(this->position, pointV[j], x))
						{
							i = i + 1;
							std::swap(pointV[i], pointV[j]);
						}
					}
					std::swap(pointV[i], pointV[head]);
					r = i;
					sortPoints(points, head, r);
					sortPoints(points, r + 1, tail);
				}
				
			}

			float Light::distance(b2Vec2 a, b2Vec2 b)
			{
				b2Vec2 ba = b - a;
				return sqrtf(powf(ba.x, 2.0f) + powf(ba.y, 2.0f));
			}

			float Light::pointRectDist(b2Vec2 pointVec)
			{
				//sf::Vector2f pv = b2WorldToViewport(pointVec);
				float px = pointVec.x;
				float py = pointVec.y;
				gui::Viewport vp = game->getGUI().getViewport();
				//py += vp.getViewportOffsetY();
				float rx = (float)vp.getViewportX();
				float ry = (float)vp.getViewportY();
				float rwidth = (float)vp.getViewportWidth();
				float rheight = (float)vp.getViewportHeight();
				float cx = fmaxf(fminf(px, rx + rwidth), rx);
				float cy = fmaxf(fminf(py, ry + rheight), ry);
				return sqrtf((px - cx)*(px - cx) + (py - cy)*(py - cy));
			}

			sf::Vector2f Light::b2WorldToViewport(b2Vec2 point)
			{
				gui::Viewport viewport = game->getGUI().getViewport();
				sf::Vector2f newVect = sf::Vector2f((point.x)-viewport.getViewportX(),
					((point.y)-viewport.getViewportY()));
				return newVect;
			}

			sf::VertexArray Light::computeAndGetLightShape(int r, int g, int b)
			{
				std::vector<b2Vec2> points = std::vector<b2Vec2>();
				prePopulatePoints(&points, this->angle, this->halfAngleSpread, 100);
				

				/*gathering all of the bodies, and adding those points if they're in the light's area
				b2Body* body = game->getGameWorld()->GetBodyList();
				while (body != nullptr)
				{
					b2Fixture* fixt = body->GetFixtureList();
					while (fixt != nullptr)
					{
						b2Shape *shape = fixt->GetShape();
						if (shape->GetType() == shape->e_polygon)
						{
							b2PolygonShape *polygon = (b2PolygonShape*)shape;
							int count = polygon->GetVertexCount();
							for (int i = 0; i < count; i++)
							{
								b2Vec2 vert = polygon->GetVertex(i);
								if (pointInAngleRange(this->position, this->angle, this->halfAngleSpread, vert)
									&& distance(this->position, vert))
								{
									float angle = getPointAngle(this->getPosition(), vert);
									b2Vec2 leftish = generatePointFromAngle(this->position,
																			this->reach,
																			angle-0.0001);
									b2Vec2 rightish = generatePointFromAngle(this->position,
																			 this->reach,
																			angle + 0.0001);
									points.push_back(leftish);
									points.push_back(polygon->GetVertex(i));
									points.push_back(rightish);
								}
							}

						}
						else if (shape->GetType() == shape->e_edge)
						{
							b2EdgeShape* edge = (b2EdgeShape*)shape;
							//EDGES APPARENTLY HAVE 4 VERTICES UWATM8
						}

						fixt = fixt->GetNext();
					}

					body = body->GetNext();
				}
				*/
				//must deal with raycasting now... WAHHHHHH
				b2World* box2DWorld = game->getGameWorld();
				for (int i = 0; i < points.size(); i++)
				{
					RaycastCallback rCallback;
					b2Vec2 originalPoint = b2Vec2(points[i]);
					box2DWorld->RayCast(&rCallback, this->position, points[i]);
					if (rCallback.m_fraction != 1.0f)
					{
						points[i] = (b2Vec2(rCallback.m_point));
					}
				}

				sortPoints(&points, 0, points.size());
				//std::cout <<"Number of raycasts..."<< points.size()<<std::endl;
				//std::cout << "Number of bodies..." << game->getGameWorld()->GetBodyCount() << std::endl;

				sf::VertexArray fan(sf::TrianglesFan, points.size()+1);
				fan[0].position = b2WorldToViewport(this->position);
				fan[0].color = sf::Color(r,g,b, 255);
				for (int i = 1; i < points.size()+1; i++)
				{
					fan[i].position = b2WorldToViewport(points[i-1]);
					int intensity = (int)(255*(1.0f-(distance(this->position, points[i-1]) / this->reach)));
					fan[i].color = sf::Color(r,g,b, intensity);
				}

				return fan;



				/*
				if light in range of viewport
					var points
					prePopulate(points, 15)
					For each body in bodies
					for each fixture in body
						for each point in fixture
							if point in angleRange and within radius
								angle = getAngle(point)
								points.add(generatePointFromAngle(angle - 0.0001))
								points.add(point)
								points.add(generatePointFromAngle(angle + 0.0001))
								sortPoints(points)
								add edge points to front and back of points
								cleanPoints(points)//removes points at exact same angle to center


								//raycasting and finding the points that makeup the light shape
								var lightPoints
								for point in points
									raycastHit = raycast(light.center, point)
									add raycastHit.point to lightPoints

									//constructing lightShape
									var lightShape
									var centerVertex = vertex(WorldToVP(light.center), color(255, 255, 255, 255))
									lightShape.add(centerVertex)
									for point in lightPoints
										var intensity = (distance(light.center, point) / light.radius)
										var whiteValue = int(intensity * 255)
										lightShape.add(vertex(worldToViewport(point), color(whiteValue, whiteValue, whiteValue, 255)))

										return lightShape
				*/
			}



		}
	}
}