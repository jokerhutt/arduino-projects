#include "control_display.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

#include "pet.hpp"

enum class Button {
  BUTTON_FEED = 6,
  BUTTON_PLAY = 8,
  BUTTON_CLEAN_POOP = 5,
  BUTTON_LIGHTS_TOGGLE = 4
};

bool last_play_state = false;
bool last_feed_state = false;
bool last_clean_state = false;
bool last_lights_state = false;

bool is_clicking(Button button_type) {
  int state = digitalRead(static_cast<int>(button_type));
  return state == LOW;
}

bool lights_on = false;

Pet pet;

void setup() {

  // Random Seed
  randomSeed(analogRead(A0));

  init_eyes();

  // Set Button Pins
  pinMode(static_cast<int>(Button::BUTTON_FEED), INPUT_PULLUP);
  pinMode(static_cast<int>(Button::BUTTON_PLAY), INPUT_PULLUP);
  pinMode(static_cast<int>(Button::BUTTON_CLEAN_POOP), INPUT_PULLUP);
  pinMode(static_cast<int>(Button::BUTTON_LIGHTS_TOGGLE), INPUT_PULLUP);
}

void loop() {

  bool current_play_state =

      is_clicking(Button::BUTTON_PLAY);

  bool current_feed_state =

      is_clicking(Button::BUTTON_FEED);

  bool current_clean_state =

      is_clicking(Button::BUTTON_CLEAN_POOP);

  bool current_lights_state =

      is_clicking(Button::BUTTON_LIGHTS_TOGGLE);

  if (current_play_state && !last_play_state) {
    pet.play();
    happy_eye();
  }

  if (current_feed_state && !last_feed_state) {
    pet.feed();
    blink(4);
  }

  if (current_clean_state && !last_clean_state) {

    wakeup();
  }

  if (current_lights_state && !last_lights_state) {

    sleep();
  }

  last_play_state = current_play_state;
  last_feed_state = current_feed_state;
  last_clean_state = current_clean_state;
  last_lights_state = current_lights_state;

  pet.update();
}
