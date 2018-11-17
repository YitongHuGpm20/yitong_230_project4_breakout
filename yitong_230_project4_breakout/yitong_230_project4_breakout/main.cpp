#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Paddle.h"
#include "Ball.h"
#include "Bricks.h"
#include "Lives.h"
#include <ctime>
#include <cmath>

using namespace sf;

void update_state(float dt);
void render_frame();

RenderWindow window;
Paddle paddle;
Ball ball;
Lives lives[10];
bool startBall;
float randStart;
int livesLeft;

int main()
{
	window.create(VideoMode(900, 900), "Zodiac Breakout");
	Font font;
	font.loadFromFile("arial.ttf");
	Text lostMessage;
	lostMessage.setFont(font);
	lostMessage.setCharacterSize(36);
	lostMessage.setString("Sorry, you lost :(");
	lostMessage.setPosition(60, 5);
	lostMessage.setFillColor(Color::Yellow);
	lostMessage.setStyle(Text::Bold);
	Text restart;
	restart.setFont(font);
	restart.setCharacterSize(24);
	restart.setString("Press Enter to restart...");
	restart.setPosition(70, 55);
	restart.setFillColor(Color::Yellow);
	restart.setStyle(Text::Italic);
	
	Clock clock;
	ball.loc.y = paddle.loc.y - ball.radius * 2;
	srand(time(0));
	randStart = rand() % 100 - 60;
	ball.loc.x = 900 / 2 - ball.radius + randStart;
	startBall = false;
	for (int i = 0; i < 10; i++)
		lives[i].loc.x = 10 + lives[i].space * i;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();
		livesLeft = 1;

		update_state(dt);
		render_frame();
		window.display();
		if (livesLeft <= 0) {
			RenderWindow lost(VideoMode(400, 100), "GAME OVER!");
			while (lost.isOpen()) {
				Event eventl;
				while (lost.pollEvent(eventl)) {
					if (Keyboard::isKeyPressed(Keyboard::Enter))
						lost.close();
				}
				lost.clear();
				lost.draw(lostMessage);
				lost.draw(restart);
				lost.display();
			}
		}
	}

	return 0;
}

void update_state(float dt)
{
	//player control
	if (Keyboard::isKeyPressed(Keyboard::A) && paddle.loc.x >= 0) {
		paddle.loc.x -= paddle.vel.x * dt;
		if (!startBall)
			ball.loc.x -= paddle.vel.x * dt;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D) && paddle.loc.x + paddle.paddleW <= 900) {
		paddle.loc.x += paddle.vel.x * dt;
		if (!startBall)
			ball.loc.x += paddle.vel.x * dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space) && !startBall) {
		startBall = true;
		ball.vel.x = 400 * (randStart / 80);
		ball.vel.y = -abs(sqrtf(400 * 400 - ball.vel.x * ball.vel.x));
	}

	//ball movement
	if (startBall) {
		//bounce on wall
		if (ball.loc.y < 0 && ball.vel.y < 0)
			ball.vel.y = -ball.vel.y;
		if (ball.loc.x < 0 && ball.vel.x < 0)
			ball.vel.x = abs(ball.vel.x);
		if (ball.loc.x + ball.radius * 2 > 900 && ball.vel.x > 0)
			ball.vel.x = -abs(ball.vel.x);
		//bounce on paddle
		if (ball.loc.y + 2 * ball.radius >= paddle.loc.y && ball.vel.y > 0) {
			if (ball.loc.x + ball.radius >= paddle.loc.x + 60 && ball.loc.x + ball.radius <= paddle.loc.x + 80) {
				ball.vel.x = -400 * 20 / 80;
				ball.vel.y = -abs(sqrtf(400 * 400 - ball.vel.x * ball.vel.x));
			}
			else if (ball.loc.x + ball.radius > paddle.loc.x + 80 && ball.loc.x + ball.radius <= paddle.loc.x + 100) {
				ball.vel.x = 400 * 20 / 80;
				ball.vel.y = -abs(sqrtf(400 * 400 - ball.vel.x * ball.vel.x));
			}
			else if (ball.loc.x + ball.radius >= paddle.loc.x && ball.loc.x + ball.radius <= paddle.loc.x + 20) {
				ball.vel.x = -400 * 60 / 80;
				ball.vel.y = -abs(sqrtf(400 * 400 - ball.vel.x * ball.vel.x));
			}
			else if (ball.loc.x + ball.radius >= paddle.loc.x + 140 && ball.loc.x + ball.radius <= paddle.loc.x + 160) {
				ball.vel.x = 400 * 60 / 80;
				ball.vel.y = -abs(sqrtf(400 * 400 - ball.vel.x * ball.vel.x));
			}
			else if (ball.loc.x + ball.radius >= paddle.loc.x && ball.loc.x + ball.radius <= paddle.loc.x + paddle.paddleW) {
				ball.vel.x = 400 * ((ball.loc.x + ball.radius - paddle.loc.x - paddle.paddleW / 2) / 80);
				ball.vel.y = -abs(sqrtf(400 * 400 - ball.vel.x * ball.vel.x));
			}
		}
		//bounce on side of paddle
		if (ball.loc.y <= paddle.loc.y + paddle.paddleH && ball.loc.y + ball.radius * 2 >= paddle.loc.y) {
			if (ball.loc.x + ball.radius < paddle.loc.x && ball.loc.x + ball.radius * 2 >= paddle.loc.x)
				ball.vel.x = -abs(ball.vel.x);
			else if(ball.loc.x <= paddle.loc.x + paddle.paddleW && ball.loc.x + ball.radius > paddle.loc.x + paddle.paddleW)
				ball.vel.x = abs(ball.vel.x);
		}
		if (ball.loc.y + ball.radius * 2 <= paddle.loc.y && ball.loc.y > paddle.loc.y + paddle.paddleH) {
			if (ball.loc.x + ball.radius * 2 >= paddle.loc.x)
				ball.vel.x = -abs(ball.vel.x);
			else if (ball.loc.x <= paddle.loc.x + paddle.paddleW)
				ball.vel.x = abs(ball.vel.x);
		}
		ball.loc.x += ball.vel.x * dt;
		ball.loc.y += ball.vel.y * dt;
	}

	//lose life
	if (ball.loc.y >= 900) {
		livesLeft--;
		ball.loc.y = paddle.loc.y - ball.radius * 2;
		srand(time(0));
		randStart = rand() % 100 - 60;
		ball.loc.x = paddle.loc.x + paddle.paddleW / 2 + randStart;
		startBall = false;
	}
}

void render_frame()
{
	window.clear();
	window.draw(paddle.SpawnPaddle());
	window.draw(ball.SpawnBall());
	for (int i = 0; i < livesLeft; i++)
		window.draw(lives[i].SpawnLives());
}