#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#pragma once

using namespace sf;

class Ball
{
public:
	Vector2f loc;
	float radius = 15;
	float speed = 400;
	Vector2f vel = Vector2f(0, -speed);

	CircleShape SpawnBall(Texture&);
};

