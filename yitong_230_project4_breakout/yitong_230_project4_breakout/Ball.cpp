#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Ball.h"

using namespace sf;

CircleShape Ball::SpawnBall() {
	CircleShape shape;
	shape.setPosition(loc.x, loc.y);
	shape.setRadius(radius);
	shape.setFillColor(Color::White);
	return shape;
}
