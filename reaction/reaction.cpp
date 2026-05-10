#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

enum class LightMode { Waiting = 2, Fired = 3 };

constexpr int BUTTON_PIN = 7;

Adafruit_SSD1306 display(128, 32, &Wire);

LightMode current_light_mode = LightMode::Waiting;

unsigned long start_time = 0;

unsigned long time_taken = 0;

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

void render_display(unsigned long reaction_time, bool go_mode) {
  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(SSD1306_WHITE);

  // left
  display.setCursor(0, 0);

  display.print(reaction_time);

  display.print("ms");

  // right
  display.setCursor(80, 0);

  if (go_mode) {

    display.print("GO");

  } else {

    display.print("WAIT");
  }

  display.display();
}

void setup_display() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();

  display.setTextSize(1);

  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);

  display.println("hello");

  display.display();
}

void setup() {

  setup_display();

  pinMode(static_cast<int>(LightMode::Waiting), OUTPUT);
  pinMode(static_cast<int>(LightMode::Fired), OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  set_light(LightMode::Waiting);

  randomSeed(analogRead(A0));

  render_display(0, current_light_mode == LightMode::Fired);
}

void loop() {

  int state = digitalRead(BUTTON_PIN);

  if (current_light_mode == LightMode::Waiting) {
    long delay_time = random(1000, 5000);
    delay(delay_time);
    current_light_mode = LightMode::Fired;
    set_light(current_light_mode);
    start_time = millis();
    render_display(time_taken, current_light_mode == LightMode::Fired);
  }

  if (state == LOW && current_light_mode == LightMode::Fired) {
    unsigned long local_time_taken = millis() - start_time;
    current_light_mode = LightMode::Waiting;
    set_light(current_light_mode);
    time_taken = local_time_taken;
    handle_input_cooldown();
    render_display(time_taken, current_light_mode == LightMode::Fired);
  }
}
