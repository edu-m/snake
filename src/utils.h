#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "define.h"
#include "fruit.h"
#include "snake.h"
#include <cstdint>
#include <cstdlib>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
using namespace sf;
using namespace std;

inline RenderWindow *initWindow() {
    
  RenderWindow *window = new RenderWindow(
      VideoMode(WINDOW_SCALE * HOR_SIZE, WINDOW_SCALE * VER_SIZE), "Snake");
  window->setFramerateLimit(WINDOW_FRAMERATE);
  return window;
}

inline void windowEventHandler(Event &e, RenderWindow *window) {
  while (window->pollEvent(e)) {
    if (e.type == Event::Closed)
      window->close();
  }
  if (IS_KEY_PRESSED(Keyboard::Escape))
    exit(0);
}

inline void checkChangeDirectionEvent() {
  Snake &snake = Snake::getInstance();
  if (snake.getDirection() == Direction::down)
    snake.setYPos(snake.getPos()->at(0).second + 1);
  if (snake.getDirection() == Direction::left)
    snake.setXPos(snake.getPos()->at(0).first - 1);
  if (snake.getDirection() == Direction::right)
    snake.setXPos(snake.getPos()->at(0).first + 1);
  if (snake.getDirection() == Direction::up)
    snake.setYPos(snake.getPos()->at(0).second - 1);
}

inline void getUserInput() {
  Snake &snake = Snake::getInstance();
  if (IS_KEY_PRESSED(Keyboard::Left)) // direction control
    snake.setDirection(Direction::left);
  if (IS_KEY_PRESSED(Keyboard::Right))
    snake.setDirection(Direction::right);
  if (IS_KEY_PRESSED(Keyboard::Up))
    snake.setDirection(Direction::up);
  if (IS_KEY_PRESSED(Keyboard::Down))
    snake.setDirection(Direction::down);
  if ((IS_KEY_PRESSED(Keyboard::Space))) //_pause event
  {
    snake.togglePauseGame();
    usleep(500000);
  }
}

inline void checkWraparound() {
  Snake &snake = Snake::getInstance();
  // cout << snake.getPos()->at(0).first << " " << snake.getPos()->at(0).second
  //      << endl;

  if (snake.getPos()->at(0).first > HOR_SIZE)
    snake.setXPos(0);
  if (snake.getPos()->at(0).first < 0)
    snake.setXPos(HOR_SIZE);
  if (snake.getPos()->at(0).second > VER_SIZE)
    snake.setYPos(0);
  if (snake.getPos()->at(0).second < 0)
    snake.setYPos(VER_SIZE);
}

inline void checkFruitEatenEvent() {
  Snake &snake = Snake::getInstance();
  Fruit &fruit = Fruit::getInstance();
  if (snake.getPos()->at(0) == fruit.getPos()) // fruit eaten event
  {
    snake.setSize(snake.getSize() + 1);
    fruit.generateRandomPosition();
  }
}

inline void checkDeathEvent() {
  Snake &snake = Snake::getInstance();
  for (int i = 1; i < snake.getSize(); i++)
    if (snake.getPos()->at(0).first == snake.getPos()->at(i).first &&
        snake.getPos()->at(0).second == snake.getPos()->at(i).second) {
      snake.setSize(4);
      sleep(3);
      snake.generateRandomPosition();
      snake.setDirection(Direction::right);
    }
}

inline void Tick() {
  Snake &snake = Snake::getInstance();
  getUserInput();
  if (!snake.isGamePaused()) {
    for (int i = snake.getSize(); i > 0; i--)
      snake.setPosIndex(snake.getPos()->at(i - 1), i);
    checkChangeDirectionEvent();
    checkFruitEatenEvent();
    checkWraparound();
    checkDeathEvent();
  }
}
inline void drawWindow(Sprite &tileSprite, Sprite &snakeSprite, Sprite &foodSprite,
                RenderWindow *window) {

  Snake &snake = Snake::getInstance();
  Fruit &fruit = Fruit::getInstance();
  window->clear();
  for (int i = 0; i < HOR_SIZE; i++) // sprite drawing function
  {
    for (int j = 0; j < VER_SIZE; j++) {
      tileSprite.setPosition(i * WINDOW_SCALE, j * WINDOW_SCALE);
      window->draw(tileSprite);
    }
    const vector<pair<int, int>> *snakePos = snake.getPos();
    for (int i = 0; i < snake.getSize(); i++) {
      snakeSprite.setPosition(snakePos->at(i).first * WINDOW_SCALE,
                              snakePos->at(i).second * WINDOW_SCALE);
      window->draw(snakeSprite);
    }
    foodSprite.setPosition(fruit.getPos().first * WINDOW_SCALE,
                           fruit.getPos().second * WINDOW_SCALE);
    window->draw(foodSprite);
  }
  window->display();
}

inline void mainLoopGame() {
  float time, timer = 0;
  Event e;
  Clock gameClock;
  Sprite tileSprite, snakeSprite, foodSprite;

  RenderWindow *window = initWindow();
  Snake &snake = Snake::getInstance();

  Texture tileTexture, snakeTexture, foodTexture;
  tileTexture.loadFromFile("images/tile.png");
  snakeTexture.loadFromFile("images/snake.png");
  foodTexture.loadFromFile("images/food.png");

  tileSprite = Sprite(tileTexture);
  snakeSprite = Sprite(snakeTexture);
  foodSprite = Sprite(foodTexture);

  while (window->isOpen()) {
    time = gameClock.getElapsedTime().asSeconds();
    gameClock.restart();
    timer += time;

    windowEventHandler(e, window);
    if (timer > snake.getSpeed()) {
      timer = 0;
      Tick();
    }
    drawWindow(tileSprite, snakeSprite, foodSprite, window);
  }
}