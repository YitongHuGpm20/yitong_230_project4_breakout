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
#include <string>
#include <sstream>
#include <iostream>

using namespace sf;
using namespace std;

void update_state(float);
void render_frame(RectangleShape, Texture&, Texture&);

template <typename T>
string toString(T arg) {
	stringstream ss;
	ss << arg;
	return ss.str();
}

RenderWindow window;
Paddle paddle;
Ball ball;
Lives lives[10];
Bricks bricks[72];
String printlevel;
String printscore;
bool startBall;
float randStart;
int livesLeft;
int level;
int hitBricks;
int score;
int combo;
bool playpaddle = false;
bool playdestroy = false;
bool playdemage = false;
bool playwall = false;
bool playlevel = false;
bool playwin = false;
bool playloselife = false;
bool playlost = false;

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
	Text leveltext;
	leveltext.setFont(font);
	leveltext.setCharacterSize(24);
	leveltext.setString("LEVEL: 1");
	leveltext.setPosition(10, 10);
	leveltext.setFillColor(Color::Green);
	leveltext.setStyle(Text::Bold);
	Text scoretext;
	scoretext.setFont(font);
	scoretext.setCharacterSize(24);
	scoretext.setString("SCORE: 0");
	scoretext.setPosition(600, 10);
	scoretext.setFillColor(Color::Green);
	scoretext.setStyle(Text::Bold);

	Texture brickArt;
	brickArt.loadFromFile("brick.png");
	Texture stoneArt;
	stoneArt.loadFromFile("stone.png");
	Texture zoomArt;
	zoomArt.loadFromFile("zoom.png");
	Texture bombArt;
	bombArt.loadFromFile("bomb.png");
	Texture backgroundArt;
	backgroundArt.loadFromFile("starsky.png");
	Texture ballArt;
	ballArt.loadFromFile("starball.png");
	Texture paddleArt;
	paddleArt.loadFromFile("paddle.png");

	SoundBuffer paddlebuffer;
	paddlebuffer.loadFromFile("breakout_paddle.wav");
	Sound paddlesound;
	paddlesound.setBuffer(paddlebuffer);
	SoundBuffer destroybuffer;
	destroybuffer.loadFromFile("breakout_brick.wav");
	Sound destroysound;
	destroysound.setBuffer(destroybuffer);
	SoundBuffer demagebuffer;
	demagebuffer.loadFromFile("breakout_demage.wav");
	Sound demagesound;
	demagesound.setBuffer(demagebuffer);
	SoundBuffer wallbuffer;
	wallbuffer.loadFromFile("breakout_wall.wav");
	Sound wallsound;
	wallsound.setBuffer(wallbuffer);
	SoundBuffer levelbuffer;
	levelbuffer.loadFromFile("breakout_level.wav");
	Sound levelsound;
	levelsound.setBuffer(levelbuffer);
	SoundBuffer winbuffer;
	winbuffer.loadFromFile("breakout_win.wav");
	Sound winsound;
	winsound.setBuffer(winbuffer);
	SoundBuffer loselifebuffer;
	loselifebuffer.loadFromFile("breakout_loselife.wav");
	Sound loselifesound;
	loselifesound.setBuffer(loselifebuffer);
	SoundBuffer lostbuffer;
	lostbuffer.loadFromFile("breakout_lost.wav");
	Sound lostsound;
	lostsound.setBuffer(lostbuffer);
	
	Clock clock;
	RectangleShape background;
	background.setTexture(&backgroundArt);
	background.setSize(Vector2f(900, 900));
	background.setPosition(Vector2f(0, 0));
	ball.loc.y = paddle.loc.y - ball.radius * 2;
	srand(time(0));
	randStart = rand() % 100 - 60;
	ball.loc.x = 900 / 2 - ball.radius + randStart;
	startBall = false;
	livesLeft = 3;
	level = 1;
	hitBricks = 0;
	score = 0;
	combo = 0;
	for (int i = 0; i < 10; i++)
		lives[i].loc.x = 10 + lives[i].space * i;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 9; j++) {
			bricks[i * 9 + j].loc.x = j * 100;
			bricks[i * 9 + j].loc.y = 50 + i * 50;
			bricks[i * 9 + j].health = 3;
		}
	}

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		float dt = clock.restart().asSeconds();

		//display
		update_state(dt);
		render_frame(background, paddleArt, ballArt);
		for (int i = 0; i < 72; i++) {
			if (level == 1) {
				if (i == 2 || i == 6 || i == 10 || i == 12 || i == 14 || i == 16 || i == 19 || i == 21 || i == 23 || i == 25 || i == 28 || i == 34) {
					window.draw(bricks[i].SpawnZoom(zoomArt));
					bricks[i].type = 3;
				}
				else if (i == 31 || i == 40 || i == 49 || i == 58 || i == 67) {
					window.draw(bricks[i].SpawnBricks(stoneArt));
					bricks[i].type = 2;
				}
				else if (i == 38 || i == 42) {
					window.draw(bricks[i].SpawnBricks(bombArt));
					bricks[i].type = 4;
				}
				else {
					window.draw(bricks[i].SpawnBricks(brickArt));
					bricks[i].type = 1;
				}
			}
			else if (level == 2) {
				if (i == 1 || i == 7 || i == 11 || i == 15) {
					window.draw(bricks[i].SpawnBricks(bombArt));
					bricks[i].type = 4;
				}
				else if (i == 40 || i == 48 || i == 50 || i == 57 || i == 59 || i == 67) {
					window.draw(bricks[i].SpawnZoom(zoomArt));
					bricks[i].type = 3;
				}
				else if (i == 21 || i == 23 || i == 30 || i == 32) {
					window.draw(bricks[i].SpawnBricks(stoneArt));
					bricks[i].type = 2;
				}
				else {
					window.draw(bricks[i].SpawnBricks(brickArt));
					bricks[i].type = 1;
				}
			}
			else if (level == 3) {
				if (i == 21 || i == 23 || i == 39 || i == 41 || i == 67) {
					window.draw(bricks[i].SpawnBricks(bombArt));
					bricks[i].type = 4;
				}
				else if (i == 12 || i == 14 || i == 30 || i == 32 || i == 48 || i == 50 || i == 57 || i == 59) {
					window.draw(bricks[i].SpawnZoom(zoomArt));
					bricks[i].type = 3;
				}
				else if (i == 2 || i == 3 || i == 4 || i == 5 || i == 6 || i == 65 || i == 66 || i == 68 || i == 69) {
					window.draw(bricks[i].SpawnBricks(stoneArt));
					bricks[i].type = 2;
				}
				else {
					window.draw(bricks[i].SpawnBricks(brickArt));
					bricks[i].type = 1;
				}
			}
		}
			
		printlevel = "LEVEL: " + toString<int>(level);
		leveltext.setString(printlevel);
		window.draw(leveltext);
		printscore = "SCORE: " + toString<int>(score);
		scoretext.setString(printscore);
		window.draw(scoretext);
		window.display();
		
		//play sounds;
		if (playpaddle) {
			paddlesound.play();
			playpaddle = false;
		}
		if (playdestroy) {
			destroysound.play();
			playdestroy = false;
		}
		if (playdemage) {
			demagesound.play();
			playdemage = false;
		}
		if (playwall) {
			wallsound.play();
			playwall = false;
		}
		if (playlevel) {
			levelsound.play();
			playlevel = false;
		}
		if (playwin) {
			winsound.play();
			playwin = false;
		}
		if (playloselife) {
			loselifesound.play();
			playloselife = false;
		}

		//restart game
		if (livesLeft <= 0) {
			lostsound.play();
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
				livesLeft = 3;
				for (int i = 0; i < 8; i++) {
					for (int j = 0; j < 9; j++) {
						bricks[i * 9 + j].loc.x = j * 100;
						bricks[i * 9 + j].loc.y = 50 + i * 50;
						bricks[i * 9 + j].health = 3;
						bricks[i * 9 + j].size.x = 100;
						bricks[i * 9 + j].size.y = 50;
					}
				}
				paddle.loc = Vector2f((900 - paddle.paddleW) / 2, 900 - paddle.paddleH - 40);
				randStart = rand() % 100 - 60;
				ball.loc.x = 900 / 2 - ball.radius + randStart;
				level = 1;
				score = 0;
				hitBricks = 0;
				combo = 0;
				startBall = false;
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
		ball.vel.x = ball.speed * (randStart / 80);
		ball.vel.y = -abs(sqrtf(ball.speed * ball.speed - ball.vel.x * ball.vel.x));
		playpaddle = true;
	}

	//ball movement
	if (startBall) {
		//bounce on wall
		if (ball.loc.y < 0 && ball.vel.y < 0) {
			ball.vel.y = -ball.vel.y;
			playwall = true;
		}
		if (ball.loc.x < 0 && ball.vel.x < 0) {
			ball.vel.x = abs(ball.vel.x);
			playwall = true;
		}
		if (ball.loc.x + ball.radius * 2 > 900 && ball.vel.x > 0) {
			ball.vel.x = -abs(ball.vel.x);
			playwall = true;
		}
		//bounce on paddle
		if (ball.loc.y + 2 * ball.radius >= paddle.loc.y && ball.loc.y < paddle.loc.y && ball.vel.y > 0) {
			if (ball.loc.x + ball.radius >= paddle.loc.x + 60 && ball.loc.x + ball.radius <= paddle.loc.x + 80) {
				ball.vel.x = -ball.speed * 20 / 80;
				ball.vel.y = -abs(sqrtf(ball.speed * ball.speed - ball.vel.x * ball.vel.x));
				combo = 0;
				playpaddle = true;
			}
			else if (ball.loc.x + ball.radius > paddle.loc.x + 80 && ball.loc.x + ball.radius <= paddle.loc.x + 100) {
				ball.vel.x = ball.speed * 20 / 80;
				ball.vel.y = -abs(sqrtf(ball.speed * ball.speed - ball.vel.x * ball.vel.x));
				combo = 0;
				playpaddle = true;
			}
			else if (ball.loc.x + ball.radius >= paddle.loc.x && ball.loc.x + ball.radius <= paddle.loc.x + 20) {
				ball.vel.x = -ball.speed * 60 / 80;
				ball.vel.y = -abs(sqrtf(ball.speed * ball.speed - ball.vel.x * ball.vel.x));
				combo = 0;
				playpaddle = true;
			}
			else if (ball.loc.x + ball.radius >= paddle.loc.x + 140 && ball.loc.x + ball.radius <= paddle.loc.x + 160) {
				ball.vel.x = ball.speed * 60 / 80;
				ball.vel.y = -abs(sqrtf(ball.speed * ball.speed - ball.vel.x * ball.vel.x));
				combo = 0;
				playpaddle = true;
			}
			else if (ball.loc.x + ball.radius >= paddle.loc.x && ball.loc.x + ball.radius <= paddle.loc.x + paddle.paddleW) {
				ball.vel.x = ball.speed * ((ball.loc.x + ball.radius - paddle.loc.x - paddle.paddleW / 2) / 80);
				ball.vel.y = -abs(sqrtf(ball.speed * ball.speed - ball.vel.x * ball.vel.x));
				combo = 0;
				playpaddle = true;
			}
		}
		//bounce on side of paddle
		if (ball.loc.y <= paddle.loc.y + paddle.paddleH && ball.loc.y + ball.radius * 2 >= paddle.loc.y && ball.vel.y > 0) {
			if (ball.loc.x + ball.radius < paddle.loc.x && ball.loc.x + ball.radius * 2 >= paddle.loc.x) {
				ball.vel.x = -abs(ball.vel.x);
				combo = 0;
				playpaddle = true;
			}
			else if (ball.loc.x <= paddle.loc.x + paddle.paddleW && ball.loc.x + ball.radius > paddle.loc.x + paddle.paddleW) {
				ball.vel.x = abs(ball.vel.x);
				combo = 0;
				playpaddle = true;
			}
		}
		if (ball.loc.y + ball.radius * 2 <= paddle.loc.y && ball.loc.y > paddle.loc.y + paddle.paddleH && ball.vel.y > 0) {
			if (ball.loc.x + ball.radius * 2 >= paddle.loc.x) {
				ball.vel.x = -abs(ball.vel.x);
				combo = 0;
				playpaddle = true;
			}
			else if (ball.loc.x <= paddle.loc.x + paddle.paddleW) {
				ball.vel.x = abs(ball.vel.x);
				combo = 0;
				playpaddle = true;
			}
		}
		//bounce off bricks
		for (int i = 0; i < 72; i++) {
			if (ball.loc.x + ball.radius <= bricks[i].loc.x + bricks[i].size.x && ball.loc.x + ball.radius >= bricks[i].loc.x) {
				if (ball.loc.y <= bricks[i].loc.y + bricks[i].size.y && ball.loc.y + ball.radius * 2 > bricks[i].loc.y + bricks[i].size.y && ball.vel.y < 0) {
					ball.vel.y = abs(ball.vel.y);
					if (bricks[i].type == 3)
						playdemage = true;
					else
						playdestroy = true;
					if (bricks[i].type == 1 || (bricks[i].type == 3 && bricks[i].health == 1)) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
					}
					else if (bricks[i].type == 3 && bricks[i].health > 1) {
						if (bricks[i].health == 3) {
							bricks[i].loc.x += 10;
							bricks[i].loc.y += 5;
						}
						else if (bricks[i].health == 2) {
							bricks[i].loc.x += 8;
							bricks[i].loc.y += 4;
						}
						bricks[i].health--;
						bricks[i].size.x = bricks[i].size.x * 0.8;
						bricks[i].size.y = bricks[i].size.y * 0.8;
					}
					else if (bricks[i].type == 4) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
						if (i > 9) {
							bricks[i - 9].loc.x = 0;
							bricks[i - 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i < 64) {
							bricks[i + 9].loc.x = 0;
							bricks[i + 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 1) {
							bricks[i - 1].loc.x = 0;
							bricks[i - 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 0) {
							bricks[i + 1].loc.x = 0;
							bricks[i + 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
					}
				}
				else if (ball.loc.y < bricks[i].loc.y && ball.loc.y + ball.radius * 2 >= bricks[i].loc.y &&ball.vel.y > 0) {
					ball.vel.y = -abs(ball.vel.y);
					if (bricks[i].type == 3)
						playdemage = true;
					else
						playdestroy = true;
					if (bricks[i].type == 1 || (bricks[i].type == 3 && bricks[i].health == 1)) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
					}
					else if (bricks[i].type == 3 && bricks[i].health > 1) {
						if (bricks[i].health == 3) {
							bricks[i].loc.x += 10;
							bricks[i].loc.y += 5;
						}
						else if (bricks[i].health == 2) {
							bricks[i].loc.x += 8;
							bricks[i].loc.y += 4;
						}
						bricks[i].health--;
						bricks[i].size.x = bricks[i].size.x * 0.8;
						bricks[i].size.y = bricks[i].size.y * 0.8;
					}
					else if (bricks[i].type == 4) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
						if (i > 9) {
							bricks[i - 9].loc.x = 0;
							bricks[i - 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i < 64) {
							bricks[i + 9].loc.x = 0;
							bricks[i + 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 1) {
							bricks[i - 1].loc.x = 0;
							bricks[i - 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 0) {
							bricks[i + 1].loc.x = 0;
							bricks[i + 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
					}
				}
			}
			if (ball.loc.y + ball.radius <= bricks[i].loc.y + bricks[i].size.y && ball.loc.y + ball.radius >= bricks[i].loc.y) {
				if (ball.loc.x + ball.radius * 2 >= bricks[i].loc.x && ball.loc.x < bricks[i].loc.x && ball.vel.x > 0) {
					ball.vel.x = -abs(ball.vel.x);
					if (bricks[i].type == 3)
						playdemage = true;
					else
						playdestroy = true;
					if (bricks[i].type == 1 || (bricks[i].type == 3 && bricks[i].health == 1)) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
					}
					else if (bricks[i].type == 3 && bricks[i].health > 1) {
						if (bricks[i].health == 3) {
							bricks[i].loc.x += 10;
							bricks[i].loc.y += 5;
						}
						else if (bricks[i].health == 2) {
							bricks[i].loc.x += 8;
							bricks[i].loc.y += 4;
						}
						bricks[i].health--;
						bricks[i].size.x = bricks[i].size.x * 0.8;
						bricks[i].size.y = bricks[i].size.y * 0.8;
					}
					else if (bricks[i].type == 4) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
						if (i > 9) {
							bricks[i - 9].loc.x = 0;
							bricks[i - 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i < 64) {
							bricks[i + 9].loc.x = 0;
							bricks[i + 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 1) {
							bricks[i - 1].loc.x = 0;
							bricks[i - 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 0) {
							bricks[i + 1].loc.x = 0;
							bricks[i + 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
					}
				}
				else if (ball.loc.x <= bricks[i].loc.x + bricks[i].size.x && ball.loc.x + ball.radius * 2 > bricks[i].loc.x + bricks[i].size.x && ball.vel.x < 0) {
					ball.vel.x = abs(ball.vel.x);
					if (bricks[i].type == 3)
						playdemage = true;
					else
						playdestroy = true;
					if (bricks[i].type == 1 || (bricks[i].type == 3 && bricks[i].health == 1)) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
					}
					else if (bricks[i].type == 3 && bricks[i].health > 1) {
						if (bricks[i].health == 3) {
							bricks[i].loc.x += 10;
							bricks[i].loc.y += 5;
						}
						else if (bricks[i].health == 2) {
							bricks[i].loc.x += 8;
							bricks[i].loc.y += 4;
						}
						bricks[i].health--;
						bricks[i].size.x = bricks[i].size.x * 0.8;
						bricks[i].size.y = bricks[i].size.y * 0.8;
					}
					else if (bricks[i].type == 4) {
						bricks[i].loc.x = 0;
						bricks[i].loc.y = -100;
						hitBricks++;
						score += 100 + combo * 50;
						combo++;
						if (i > 9) {
							bricks[i - 9].loc.x = 0;
							bricks[i - 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i < 64) {
							bricks[i + 9].loc.x = 0;
							bricks[i + 9].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 1) {
							bricks[i - 1].loc.x = 0;
							bricks[i - 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
						if (i % 9 != 0) {
							bricks[i + 1].loc.x = 0;
							bricks[i + 1].loc.y = -100;
							hitBricks++;
							score += 100 + combo * 50;
						}
					}
				}
			}
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
		combo = 0;
		if(livesLeft > 0)
			playloselife = true;
	}

	//finish level
	if ((level == 1 && hitBricks == 71) || (level == 2 && hitBricks == 75) || (level == 3 && hitBricks == 71)) {
		if(livesLeft < 10)
			livesLeft++;
		startBall = false;
		if (level < 3) {
			playlevel = true;
			level++;
		}
		else {
			playwin = true;
			level = 1;
		}
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 9; j++) {
				bricks[i * 9 + j].loc.x = j * 100;
				bricks[i * 9 + j].loc.y = 50 + i * 50;
				bricks[i * 9 + j].health = 3;
				bricks[i * 9 + j].size.x = 100;
				bricks[i * 9 + j].size.y = 50;
			}
		}
		paddle.loc = Vector2f((900 - paddle.paddleW) / 2, 900 - paddle.paddleH - 40);
		randStart = rand() % 100 - 60;
		ball.loc.x = 900 / 2 - ball.radius + randStart;
		ball.loc.y = paddle.loc.y - ball.radius * 2;
		hitBricks = 0;
		ball.speed += 50;
	}
}

void render_frame(RectangleShape background, Texture &paddleArt, Texture &ballArt)
{
	window.clear();
	window.draw(background);
	window.draw(paddle.SpawnPaddle(paddleArt));
	window.draw(ball.SpawnBall(ballArt));
	for (int i = 0; i < livesLeft; i++)
		window.draw(lives[i].SpawnLives(paddleArt));
}