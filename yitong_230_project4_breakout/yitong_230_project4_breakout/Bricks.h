#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#pragma once

using namespace sf;

class Bricks
{
public:
	Vector2f size = Vector2f(100, 50);
	Vector2f loc;

	RectangleShape SpawnBricks(Texture&);
};

