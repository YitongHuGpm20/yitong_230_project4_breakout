#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Bricks.h"

using namespace sf;

RectangleShape Bricks::SpawnBricks(Texture &brick) {
	RectangleShape shape;
	shape.setPosition(loc);
	shape.setSize(size);
	shape.setTexture(&brick);
	return shape;
}