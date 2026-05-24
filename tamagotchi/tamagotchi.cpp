#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "pet.hpp"

enum class Button {
  BUTTON_FEED = 7,
  BUTTON_PLAY = 8,
  BUTTON_CLEAN_POOP = 9,
  BUTTON_LIGHTS_TOGGLE = 10
};

bool is_clicking(Button button_type) {
  int state = digitalRead(static_cast<int>(button_type));
  return state == LOW;
}

bool lights_on = false;

Pet pet;

void setup() {

  // Random Seed
  randomSeed(analogRead(A0));

  // Set Button Pins
  pinMode(static_cast<int>(Button::BUTTON_FEED), INPUT_PULLUP);
  pinMode(static_cast<int>(Button::BUTTON_PLAY), INPUT_PULLUP);
  pinMode(static_cast<int>(Button::BUTTON_CLEAN_POOP), INPUT_PULLUP);
  pinMode(static_cast<int>(Button::BUTTON_LIGHTS_TOGGLE), INPUT_PULLUP);
}
