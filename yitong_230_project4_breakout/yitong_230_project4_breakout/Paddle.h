#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#pragma once

using namespace sf;

class Paddle
{
public:
	float paddleW = 160;
	float paddleH = 30;
	Vector2f loc = Vector2f((900 - paddleW) / 2, 900 - paddleH - 40);
	Vector2f vel = Vector2f(400, 0);

	RectangleShape SpawnPaddle(Texture&);
};

