#pragma once

#include "define.h"
#include <iostream>
#include <sys/types.h>
#include <utility>
#include <vector>

using namespace std;

enum class Direction : ushort { down, left, right, up };

class Snake {
public:
  // Metodo statico per ottenere l'istanza unica della classe
  static Snake &getInstance() {
    static Snake instance; // Istanza unica viene creata solo una volta
    return instance;
  }

  bool isGamePaused() const { return this->paused; }

  void togglePauseGame(){this->paused = !this->paused;}

  Direction getDirection() const {
    return this->direction;
  }

  int getSize() const { return this->size; }

  float getSpeed() const { return this->speed; }

  vector<pair<int, int>> *getPos() { return &this->pos; }

  void setSize(int _size) { this->size = _size; }

  int setPosIndex(pair<int, int> _pos, int _index) {
    if (_index > SNAKE_MAX_SIZE - 1)
      return -1;
    this->pos[_index] = _pos;
    return 1;
  }

  int setXPosIndex(int _xPos, int _index) {
    if (_index > SNAKE_MAX_SIZE - 1)
      return -1;
    this->pos[_index].first = _xPos;
    return 1;
  }

  int setYPosIndex(int _yPos, int _index) {
    if (_index > SNAKE_MAX_SIZE - 1)
      return -1;
    this->pos[_index].second = _yPos;
    return 1;
  }

  int setPos(pair<int, int> _pos) { return (this->setPosIndex(_pos, 0)); }

  int setXPos(int _xPos) { return (this->setXPosIndex(_xPos, 0)); }

  int setYPos(int _yPos) { return (this->setYPosIndex(_yPos, 0)); }

  int setDirection(Direction _direction) {
    if (this->direction == Direction::up && _direction != Direction::down) {
      this->direction = _direction;
      return 1;
    } else if (this->direction == Direction::left &&
               _direction != Direction::right) {
      this->direction = _direction;
      return 1;
    } else if (this->direction == Direction::right &&
               _direction != Direction::left) {
      this->direction = _direction;
      return 1;
    } else if (this->direction == Direction::down &&
               _direction != Direction::up) {
      this->direction = _direction;
      return 1;
    }

    return -1;
  }

  void generateRandomPosition() {
    this->setPos(make_pair(rand() % HOR_SIZE, rand() % VER_SIZE));
  }

private:
  bool paused;
  Direction direction;
  int size;
  float speed;
  vector<pair<int, int>> pos;

  Snake() {
    this->paused = false;
    this->size = DEFAULT_SNAKE_SIZE;
    this->pos = vector<pair<int, int>>(SNAKE_MAX_SIZE - 1);
    this->setPosIndex(make_pair(15, 10), 0);
    this->direction = Direction::left;
    this->speed = SPEED1;
  }
  Snake(const Snake &) = delete;
  Snake &operator=(const Snake &) = delete;
};
