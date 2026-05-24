#include "pet.hpp"

Pet::Pet() {

  // Stats
  hunger = 0;
  happiness = 100;
  energy = 100;

  // Long term states
  age = 0;
  weight = 20;

  // Modifiers
  sleeping = false;
  sick = false;

  // Timers
  last_age_update = millis();
  last_food_update = millis();
  last_happiness_update = millis();
  last_energy_update = millis();
}

// -- GETTERS --
bool Pet::is_sleeping() { return sleeping; }
bool Pet::is_hungry() { return hunger >= 50; }
bool Pet::is_underweight() { return weight <= 20; }
bool Pet::is_overweight() { return weight >= 80; }
bool Pet::is_sad() { return happiness < 30; }
bool Pet::is_exhausted() { return energy < 15; }

// -- Actions --

// FOOD
void Pet::feed() {
  uint8_t amount = 20;

  if (!is_hungry()) {
    weight += 10;
  }

  long spoiled_food_roll = random(0, 100);
  bool is_food_spoiled = spoiled_food_roll <= 5;
  if (is_food_spoiled) {
    sick = true;
  }

  if (hunger <= amount) {
    hunger = 0;
  } else {
    hunger -= 20;
  }
}

// PLAY
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

// -- TIME BASED UPDATES --

void Pet::update() {

  update_energy(false);
  update_food();
  update_age();
}

// ENERGY
void Pet::update_energy(bool is_dark) {

  unsigned long current_time = millis();

  if (current_time - last_energy_update < 5000) {
    return;
  }

  if (sleeping) {
    energy += is_dark ? 2 : 1;
    if (energy >= 80) {
      sleeping = false;
    }
  } else {
    energy -= is_dark ? 2 : 1;
    if (is_exhausted()) {
      sleeping = true;
    }
  }

  energy = constrain(energy, 0, 100);
  last_energy_update = current_time;
}

void Pet::update_happiness() {

  unsigned long current_time = millis();

  if (current_time - last_happiness_update < 5000) {
    return;
  }

  if (hunger >= 80) {
    happiness -= 2;
  }

  if (sick) {
    happiness -= 3;
  }

  if (energy <= 20 && !sleeping) {
    happiness -= 2;
  }

  if (is_overweight()) {
    happiness -= 1;
  }

  happiness = constrain(happiness, 0, 100);

  last_happiness_update = current_time;
}

void Pet::update_food() {

  unsigned long current_time = millis();

  if (current_time - last_food_update < 5000) {
    return;
  }

  hunger += 1;

  if (energy < 30) {
    hunger += 1;
  }

  if (is_underweight()) {
    hunger += 1;
  }

  hunger = constrain(hunger, 0, 100);

  if (hunger >= 90) {
    happiness -= 1;
  }

  last_food_update = current_time;
}

// AGE
void Pet::update_age() {
  unsigned long current_time = millis();

  const unsigned long DAY_DURATION = 60000;

  if (current_time - last_food_update <= DAY_DURATION) {
    return;
  }

  age++;
  hunger += 5;
  happiness -= 5;
  last_age_update = current_time;
}
