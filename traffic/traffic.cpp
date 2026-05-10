#include <Arduino.h>

enum class Light { Red = 2, Yellow = 3, Green = 4 };

constexpr int BUTTON_PIN = 7;
Light current_light = Light::Red;
bool is_changing = false;

void set_light(Light new_mode) {
  digitalWrite(static_cast<int>(Light::Red), LOW);
  digitalWrite(static_cast<int>(Light::Yellow), LOW);
  digitalWrite(static_cast<int>(Light::Green), LOW);

  digitalWrite(static_cast<int>(new_mode), HIGH);
}

void setup() {

  pinMode(static_cast<int>(Light::Red), OUTPUT);
  pinMode(static_cast<int>(Light::Yellow), OUTPUT);
  pinMode(static_cast<int>(Light::Green), OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  set_light(Light::Red);
}

void loop() {

  int state = digitalRead(BUTTON_PIN);

  // Button Pressed
  if (state == LOW && current_light == Light::Red && !is_changing) {

    // First press, wait bit
    is_changing = true;
    delay(500);

    // Set light to green so pedestrians can cross
    current_light = Light::Green;
    set_light(current_light);
    delay(5000);

    // Set light to yellow for around 2 sec
    current_light = Light::Yellow;
    set_light(current_light);
    delay(2000);

    // Back to red so u can click again
    current_light = Light::Red;
    set_light(current_light);
    is_changing = false;
  }
}
