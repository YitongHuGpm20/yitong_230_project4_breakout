#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#pragma once

using namespace sf;

class Lives
{
public:
	int num = 3;
	int liveW = 60;
	int liveH = 10;
	int space = 70;
	Vector2f loc;

	RectangleShape SpawnLives(Texture&);
};

