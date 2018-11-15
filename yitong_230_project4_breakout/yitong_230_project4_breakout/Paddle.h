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
	float paddleW = 120;
	float paddleH = 30;
	Vector2f loc = Vector2f((1000 - paddleW) / 2, 1000 - paddleH - 40);
	Vector2f vel = Vector2f(300, 0);

	RectangleShape SpawnPaddle();
};

