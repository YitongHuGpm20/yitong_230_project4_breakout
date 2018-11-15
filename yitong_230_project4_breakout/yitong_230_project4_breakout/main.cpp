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
#include <ctime>;

using namespace sf;

void update_state(float dt);
void render_frame();

RenderWindow window;
Paddle paddle;
Ball ball;
bool startBall;
float randStart;

//CircleShape ball;
//Vector2f vel(100, 300);

int main()
{
	window.create(VideoMode(1000, 1000), "Zodiac Breakout");
	Clock clock;
	ball.loc.y = paddle.loc.y - ball.radius * 2;
	srand(time(0));
	randStart = rand() % 100 - 50;
	ball.loc.x = 1000 / 2 - ball.radius + randStart;
	startBall = false;

	//ball.setRadius(16);
	//ball.setOrigin(16, 16);
	//ball.setFillColor(Color::Red);
	//ball.setPosition(400, 300);

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
	/*Vector2f pos = ball.getPosition();
	pos += vel * dt;

	if (pos.y > window.getSize().y && vel.y > 0)
		vel.y = -vel.y;
	if (pos.y < 0 && vel.y < 0)
		vel.y = -vel.y;
	if (pos.x > window.getSize().x && vel.x > 0)
		vel.x = -vel.x;
	if (pos.x < 0 && vel.x < 0)
		vel.x = -vel.x;

	ball.setPosition(pos);*/

	//player control
	if (Keyboard::isKeyPressed(Keyboard::A) && paddle.loc.x >= 0) {
		paddle.loc.x -= paddle.vel.x * dt;
		if (!startBall)
			ball.loc.x -= paddle.vel.x * dt;
	}
	else if (Keyboard::isKeyPressed(Keyboard::D) && paddle.loc.x + paddle.paddleW <= 1000) {
		paddle.loc.x += paddle.vel.x * dt;
		if (!startBall)
			ball.loc.x += paddle.vel.x * dt;
	}
	if (Keyboard::isKeyPressed(Keyboard::Space)) {
		startBall = true;
		ball.vel.x = 600 * (randStart / 60);
	}

	//ball movement
	if (startBall) {
		if (ball.loc.y < 0 && ball.vel.y < 0)
			ball.vel.y = -ball.vel.y;
		if (ball.loc.x < 0 && ball.vel.x < 0)
			ball.vel.x = abs(ball.vel.x);
		if (ball.loc.x + ball.radius * 2 > 1000 && ball.vel.x > 0)
			ball.vel.x = -abs(ball.vel.x);
		ball.loc.x += ball.vel.x * dt;
		ball.loc.y += ball.vel.y * dt;
	}
}

void render_frame()
{
	window.clear();
	//window.draw(ball);
	window.draw(paddle.SpawnPaddle());
	window.draw(ball.SpawnBall());
}