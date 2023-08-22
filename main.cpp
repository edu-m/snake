#include <SFML/Graphics.hpp>
#include <cstdint>
#include <time.h>
#include <unistd.h>
using namespace sf;
using namespace std;

#define SPEED1 0.1
#define SPEED2 0.07
#define SPEED3 0.05
#define SPEED4 0.03
#define SPEED5 0.015

#define IS_KEY_PRESSED(arg) Keyboard::isKeyPressed(Keyboard::arg)

int i1;
int i2;
int N = 70, M = 47;
int sz = 8;
int w = sz * N;
int h = sz * M;
double delay = SPEED1;
int dir = 1, snakeSize = 4;
bool overrideSpeed = false;
Clock clock1;

#define DOWN 0
#define LEFT 1
#define RIGHT 2
#define UP 3

bool containedWithin(int, int, int &);
void overrideSpeedFunc(const double &);
void Direction();
void wait();
void checkSpeed();
void Tick();

bool _pause = false;

struct Snake {
  int x, y;
} s[150];

struct Fruit {
  int x, y;
} f;

bool containedWithin(int lBound, int hBound, int &snakeSize) {
  return (snakeSize < hBound && snakeSize > lBound);
}

void overrideSpeedFunc(const double &speed) { delay = speed; }

void Direction() {
  if ((IS_KEY_PRESSED(Keyboard::Left) || IS_KEY_PRESSED(Keyboard::A)) &&
      (dir == UP || dir == DOWN)) // direction control
    dir = LEFT;
  if ((IS_KEY_PRESSED(Keyboard::Right) || IS_KEY_PRESSED(Keyboard::D)) &&
      (dir == UP || dir == DOWN))
    dir = RIGHT;
  if ((IS_KEY_PRESSED(Keyboard::Up) || IS_KEY_PRESSED(Keyboard::W)) &&
      (dir == LEFT || dir == RIGHT))
    dir = UP;
  if ((IS_KEY_PRESSED(Keyboard::Down)) ||
      ((IS_KEY_PRESSED(Keyboard::S)) && (dir == LEFT || dir == RIGHT)))
    dir = DOWN;
  if ((IS_KEY_PRESSED(Keyboard::Space))) //_pause event
  {
    _pause = !_pause;
    sleep(1); // functions as debounce when paused, a time for the player to get
              // ready when unpaused
  }
}
void checkSpeed() {
  if (containedWithin(1, 19, snakeSize))
    delay = SPEED1;
  else if (containedWithin(19, 27, snakeSize))
    delay = SPEED2;
  else if (containedWithin(27, 39, snakeSize))
    delay = SPEED3;
  else if (containedWithin(39, 56, snakeSize))
    delay = SPEED4;
  if (snakeSize >= 56)
    delay = SPEED5;
}

void checkChangeDirectionEvent() {
  if (dir == DOWN)
    s[0].y += 1;
  if (dir == LEFT)
    s[0].x -= 1;
  if (dir == RIGHT)
    s[0].x += 1;
  if (dir == UP)
    s[0].y -= 1;
}

void checkFruitEatenEvent() {
  if ((s[0].x == f.x) && (s[0].y == f.y)) // fruit eaten event
  {
    ++snakeSize;
    f.x = rand() % N;
    f.y = rand() % M;
    checkSpeed();
  }
}

void checkWraparound() {
  if (s[0].x > N)
    s[0].x = 0;
  if (s[0].x < 0)
    s[0].x = N; // wraparound
  if (s[0].y > M)
    s[0].y = 0;
  if (s[0].y < 0)
    s[0].y = M;
}

void checkDeathEvent() {
  for (int i2 = 1; i2 < snakeSize; i2++)
    if (s[0].x == s[i2].x && s[0].y == s[i2].y) // death event
    {
      snakeSize = 4;
      sleep(4);
      s[0].x = rand() % N;
      s[0].y = rand() % M;
      dir = rand() % 4 + 1; // random direction
    }
}

void Tick() {
  if (!_pause) {
    for (int i1 = snakeSize; i1 > 0; --i1) {
      s[i1].x = s[i1 - 1].x;
      s[i1].y = s[i1 - 1].y;
    }

    checkChangeDirectionEvent();

    checkFruitEatenEvent();

    checkWraparound();

    checkDeathEvent();
  }
}

int main() {
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
  text.setCharacterSize(sz*5);
  text.setFillColor(sf::Color::Black);

  f.x = 10;
  f.y = 10;
  sprite1.scale(float(sz) / 8, float(sz) / 8);
  sprite2.scale(float(sz) / 8, float(sz) / 8);
  sprite3.scale(float(sz) / 8, float(sz) / 8);
  while (window.isOpen()) {
    float time = clock1.getElapsedTime().asSeconds();
    clock1.restart();

    timer += time;

    Direction();
    window.setFramerateLimit(30);
    Event e;
    while (window.pollEvent(e)) {
      if (e.type == Event::Closed)
        window.close();
    }

    if (IS_KEY_PRESSED(Keyboard::Escape))
      return 0;

    if (timer > delay) {
      timer = 0;
      Tick();
    }

    window.clear();

    for (int i = 0; i < N; i++) // sprite drawing function
    {
      for (int j = 0; j < M; j++) {
        sprite1.setPosition(i * sz, j * sz);
        window.draw(sprite1);
      }

      for (int i = 0; i < snakeSize; i++) {
        sprite2.setPosition(s[i].x * sz, s[i].y * sz);
        window.draw(sprite2);
      }
    }

    text.setFont(font); // score count
    int score = snakeSize - 4;
    string str = to_string(score);
    text.setString(str);
    if (score >= 10)
      text.setPosition(sz*N-(sz*3), 2);
    else
      text.setPosition(sz*N-(sz*3), 2);

    sprite3.setPosition(f.x * sz, f.y * sz);
    window.draw(sprite3);
    window.draw(text);
    window.display();
  }
  return 0;
}
