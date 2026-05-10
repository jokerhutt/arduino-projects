#include <Arduino.h>

enum class LightMode { Waiting = 2, Fired = 3 };

constexpr int BUTTON_PIN = 7;

LightMode current_light_mode = LightMode::Waiting;

unsigned long start_time = 0;

unsigned long times_taken[10];
int current_take = 0;

void set_light(LightMode new_mode) {
  digitalWrite(static_cast<int>(LightMode::Waiting), LOW);
  digitalWrite(static_cast<int>(LightMode::Fired), LOW);

  digitalWrite(static_cast<int>(new_mode), HIGH);
}

void handle_input_cooldown() {
  while (digitalRead(BUTTON_PIN) == LOW) {
    delay(10);
  }
}

void setup() {

  pinMode(static_cast<int>(LightMode::Waiting), OUTPUT);
  pinMode(static_cast<int>(LightMode::Fired), OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  set_light(LightMode::Waiting);

  randomSeed(analogRead(A0));
}

void loop() {

  int state = digitalRead(BUTTON_PIN);

  if (current_light_mode == LightMode::Waiting) {
    long delay_time = random(1000, 5000);
    delay(delay_time);
    current_light_mode = LightMode::Fired;
    set_light(current_light_mode);
    start_time = millis();
  }

  if (state == LOW && current_light_mode == LightMode::Fired) {
    unsigned long time_taken = millis() - start_time;
    current_light_mode = LightMode::Waiting;
    set_light(current_light_mode);
    times_taken[current_take] = time_taken;
    current_take++;
    handle_input_cooldown();
  }

  if (current_take >= 10) {
    for (int i = 0; i < 10; i++) {
      times_taken[i] = 0;
    }
    current_take = 0;
  }
}
