#pragma once

#include <Arduino.h>

class Pet {

public:
  // stats
  uint8_t hunger;
  uint8_t happiness;
  uint8_t energy;

  // long term stats
  uint8_t age;
  uint8_t weight;

  // modifiers
  bool sick;
  bool sleeping;

  // timers
  unsigned long last_update;
  unsigned long last_age_update;
  unsigned long last_food_update;
  unsigned long last_happiness_update;
  unsigned long last_energy_update;

  Pet();

  void feed();
  void play();

  void update_food();
  void update_age();
  void update_happiness();
  void update_energy(bool is_dark);

  bool is_hungry();
  bool is_overweight();
  bool is_sad();
  bool is_exhausted();
  bool is_underweight();
  bool is_sleeping();

  void update();
};
