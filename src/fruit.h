#pragma once

#include <iostream>
#include <utility>
#include "define.h"
using namespace std;

class Fruit {
public:
  // Metodo statico per ottenere l'istanza unica della classe
  static Fruit &getInstance() {
    static Fruit instance; // Istanza unica viene creata solo una volta
    return instance;
  }

  // Returns pair with first x and second y.
  pair<int, int> getPos() const { return make_pair(this->x_pos, this->y_pos); }

  // Sets position assuming first element of pair is x and second y.
  void setPos(pair<int, int> _pos) {
    this->x_pos = _pos.first;
    this->y_pos = _pos.second;
  }

  void setXPos(int _xPos) { this->x_pos = _xPos; }

  void setYPos(int _yPos) { this->y_pos = _yPos; }

  void generateRandomPosition() {
    this->setPos(make_pair(rand() % HOR_SIZE, rand() % VER_SIZE));
  }

private:
  int x_pos;
  int y_pos;

  Fruit() { this->generateRandomPosition(); }
  Fruit(const Fruit &) = delete;
  Fruit &operator=(const Fruit &) = delete;
};
