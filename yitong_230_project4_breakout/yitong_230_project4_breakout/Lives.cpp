#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Lives.h"

using namespace sf;

RectangleShape Lives::SpawnLives(Texture &paddle) {
	RectangleShape shape;
	loc.y = 900 - liveH - 10;
	shape.setPosition(loc.x, loc.y);
	shape.setSize(Vector2f(liveW, liveH));
	shape.setTexture(&paddle);
	return shape;
}