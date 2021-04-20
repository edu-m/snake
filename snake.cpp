#include <SFML/Graphics.hpp>
#include <time.h>
#include <windows.h>
#include <iostream>
using namespace sf;
using namespace std;

int i1;
int i2;
int N = 70, M = 38;
int sz = 8;
int w = sz * N;
int h = sz * M;
double delay;
int dir = 1, num = 4;
Clock clock1;

bool pause = false;

struct Snake //might change to class?
{
	int x, y;
}  s[150];

struct Fruit
{
	int x, y;
} f;

void Direction()
{
	if ((Keyboard::isKeyPressed(Keyboard::Left) || Keyboard::isKeyPressed(Keyboard::A))) //direction control
	{
		if (dir == 3 || dir == 0)
		{
			dir = 1;
		}
	}
	if ((Keyboard::isKeyPressed(Keyboard::Right) || Keyboard::isKeyPressed(Keyboard::D)))
	{
		if (dir == 3 || dir == 0)
		{
			dir = 2;
		}
	}
	if ((Keyboard::isKeyPressed(Keyboard::Up) || Keyboard::isKeyPressed(Keyboard::W)))
	{
		if (dir == 1 || dir == 2)
		{
			dir = 3;
		}
	}
	if ((Keyboard::isKeyPressed(Keyboard::Down)) || (Keyboard::isKeyPressed(Keyboard::S)))
	{
		if (dir == 1 || dir == 2)
		{
			dir = 0;
		}
	}
	if ((Keyboard::isKeyPressed(Keyboard::Space))) //pause event 
	{
		pause = true;
		Sleep(1000);
	}
}

void wait()
{

	if ((Keyboard::isKeyPressed(Keyboard::Space))) //unpause event 
	{
		float t1 = 0;
		while (t1 < 0.8)
		{
			t1 = clock1.getElapsedTime().asSeconds();
		}
		pause = false;
	}
}

void Tick()
{
	if (pause == false)
	{
		for (int i1 = num; i1 > 0; --i1)
		{
			s[i1].x = s[i1 - 1].x;
			s[i1].y = s[i1 - 1].y;
		}

		if (dir == 0) s[0].y += 1;
		if (dir == 1) s[0].x -= 1;
		if (dir == 2) s[0].x += 1;
		if (dir == 3) s[0].y -= 1;

		if ((s[0].x == f.x) && (s[0].y == f.y)) //fruit eaten event
		{
			num++;
			f.x = rand() % N;
			f.y = rand() % M;
		}

		if (s[0].x > N) s[0].x = 0;  if (s[0].x < 0) s[0].x = N; //wraparound
		if (s[0].y > M) s[0].y = 0;  if (s[0].y < 0) s[0].y = M;


		for (int i2 = 1; i2 < num; i2++)
		{
			if (s[0].x == s[i2].x && s[0].y == s[i2].y) //death event
			{
				num = 4;
				delay = 0.1;
				Sleep(3000);
				s[0].x = rand() % N;
				s[0].y = rand() % M;
				dir = rand() % 4 + 1; //random direction
			}
		}
	}
}

int main()
{
	FreeConsole();
	RenderWindow window(VideoMode(w, h), "Snake");

	Texture t1, t2, t3;
	t1.loadFromFile("images/tile.png");
	t2.loadFromFile("images/snake.png");
	t3.loadFromFile("images/food.png");

	Sprite sprite1(t1);
	Sprite sprite2(t2);
	Sprite sprite3(t3);


	float timer = 0;

	Font font;
	font.loadFromFile("pixelated.ttf");

	Text text;
	text.setCharacterSize(24);
	text.setPosition(540, 2);
	text.setFillColor(sf::Color::Black);

	f.x = 10;
	f.y = 10;

	while (window.isOpen())
	{
		float time = clock1.getElapsedTime().asSeconds();
		clock1.restart();

		timer += time;

		if (pause == true) wait();
		else Direction();

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			return 0;
		}

		if (num < 19) //speed-size correlation
		{
			delay = 0.1;
		}
		if (num == 27)
		{
			delay = 0.07;
		}
		if (num == 39)
		{
			delay = 0.05;
		}
		if (num == 56)
		{
			delay = 0.03;
		}
		if (num == 69)
		{
			delay = 0.015;
		}

		if (timer > delay)
		{
			timer = 0;
			Tick();
		}

		window.clear();

		for (int i = 0; i < N; i++) //sprite drawing function
		{
			for (int j = 0; j < M; j++)
			{
				sprite1.setPosition(i * sz, j * sz);  window.draw(sprite1);
			}

			for (int i = 0; i < num; i++)
			{
				sprite2.setPosition(s[i].x * sz, s[i].y * sz);  window.draw(sprite2);
			}
		}

		text.setFont(font); //score count
		int score = num - 4;
		string str = to_string(score);
		text.setString(str);
		if (score >= 10)
		{
			text.setPosition(535, 2);
		}
		else
		{
			text.setPosition(540, 2);
		}

		sprite3.setPosition(f.x * sz, f.y * sz);  window.draw(sprite3);
		window.draw(text);
		window.display();
	}
	return 0;
}
