#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Main.hpp>
#include "Paddle.h";
#include "Ball.h";
#include "Bricks.h";
#include "Lives.h";
#include <ctime>;
#include <cmath>;

using namespace sf;

void update_state(float dt);
void render_frame();

RenderWindow window;
Paddle paddle;
Ball ball;
bool startBall;
float randStart;

int main()
{
	window.create(VideoMode(900, 900), "Zodiac Breakout");
	Clock clock;
	ball.loc.y = paddle.loc.y - ball.radius * 2;
	srand(time(0));
	randStart = rand() % 100 - 60;
	ball.loc.x = 900 / 2 - ball.radius + randStart;
	startBall = false;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();

		update_state(dt);
		render_frame();
		window.display();
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
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
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
}

void render_frame()
{
	window.clear();
	window.draw(paddle.SpawnPaddle());
	window.draw(ball.SpawnBall());
}