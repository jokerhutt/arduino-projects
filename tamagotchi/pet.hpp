#pragma once

#include <Arduino.h>

class Pet {

public:
  uint8_t hunger;

  uint8_t happiness;

  uint8_t energy;

  bool sick;

  uint8_t age;

  uint8_t weight;

  unsigned long poop_cooldown;

  unsigned long time_since_poop;

  unsigned long last_update;

  Pet();

  void feed();

  void play();

  bool is_hungry();

  bool is_overweight();

  bool is_underweight();

  void update();
};
