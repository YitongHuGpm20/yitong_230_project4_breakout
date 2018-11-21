#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Paddle.h"

using namespace sf;

RectangleShape Paddle::SpawnPaddle(Texture &paddle) {
	RectangleShape shape;
	shape.setPosition(loc.x, loc.y);
	shape.setSize(Vector2f(paddleW, paddleH));
	shape.setTexture(&paddle);
	return shape;
}