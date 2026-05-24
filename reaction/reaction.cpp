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

  // ASSIGN OUTPUT PINS. WAITING = PIN 2 (YELLOW), FIRED = PIN 3 (GREEN)
  pinMode(static_cast<int>(LightMode::Waiting), OUTPUT);
  pinMode(static_cast<int>(LightMode::Fired), OUTPUT);
  // ASSIGN OUTPUT PIN (BUTTON PRESSED)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // START IT AT YELLOW LIGHT
  set_light(LightMode::Waiting);

  randomSeed(analogRead(A0));

  render_display(0, current_light_mode == LightMode::Fired);
}

void loop() {

  int state = digitalRead(BUTTON_PIN);

  if (current_light_mode == LightMode::Waiting) {
    // IF WAITING, WAIT BETWEEN 1 and 5 SECONDS
    long delay_time = random(1000, 5000);
    delay(delay_time);
    // AFTER DELAY, SET THE MODE TO FIRED
    current_light_mode = LightMode::Fired;
    // SET LIGHT TO GREEN
    set_light(current_light_mode);
    // START TIMER
    start_time = millis();
    // CHANGE DISPLAY TO SHOW GO
    render_display(time_taken, current_light_mode == LightMode::Fired);
  }

  if (state == LOW && current_light_mode == LightMode::Fired) {
    // TIME TAKEN IS CURRENT TIME - START TIME
    unsigned long local_time_taken = millis() - start_time;
    current_light_mode = LightMode::Waiting;
    set_light(current_light_mode);
    time_taken = local_time_taken;
    handle_input_cooldown();
    render_display(time_taken, current_light_mode == LightMode::Fired);
  }
}
