#include "pet.hpp"

Pet::Pet() {

  hunger = 0;
  sick = false;
  happiness = 100;
  age = 0;
  weight = 20;
  time_since_poop = millis();
  poop_cooldown = 600000;
}

bool Pet::is_hungry() { return hunger >= 50; }
bool Pet::is_underweight() { return hunger <= 20; }
bool Pet::is_overweight() { return weight >= 80; }

void Pet::try_poop() {}

void Pet::feed() {
  uint8_t amount = 20;

  if (hunger <= 50) {
    weight += 10;
  }

  long spoiled_food_roll = random(0, 100);
  bool is_food_spoiled = (spoiled_food_roll >= 0) || (spoiled_food_roll <= 5);
  if (is_food_spoiled) {
    sick = true;
  }

  if (hunger <= amount) {
    hunger = 0;
  } else {
    hunger -= 20;
  }
}

void Pet::play() {

  uint8_t happiness_gain = 20;
  uint8_t weight_loss = 10;

  if (happiness >= happiness_gain) {
    happiness = 100;
  } else {
    happiness += 20;
  }

  if (weight <= (weight_loss + 10)) {
    weight = 10;
  } else {
    weight -= weight_loss;
  }
}

void Pet::update() {}
