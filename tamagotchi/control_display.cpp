#include "control_display.h"
/*
 * Main unified file for eye animations.
 * Library selection is done in "config.h".
 */
#include "display_wrapper.h" // Notre couche d'abstraction

// --- Constants ---
const long BAUD_RATE = 115200;

// Use an enum for animation indexes to avoid "magic numbers"
enum Animation {
  WAKEUP,
  RESET,
  MOVE_RIGHT_BIG,
  MOVE_LEFT_BIG,
  BLINK_LONG,
  BLINK_SHORT,
  HAPPY,
  SLEEP,
  SACCADE_RANDOM,
  MAX_ANIMATIONS // Helper to get the count of animations
};

// --- Structs for better state management ---

// Structure to hold the reference state of the eyes
struct EyeState {
  int height;
  int width;
  int x;
  int y;
};

const int REF_EYE_HEIGHT = 16;
const int REF_EYE_WIDTH = 16;
const int REF_SPACE_BETWEEN_EYE = 6;
const int REF_CORNER_RADIUS = 4;

// Current state of the eyes
EyeState left_eye, right_eye;
int corner_radius = REF_CORNER_RADIUS;

int calculate_safe_radius(int r, int w, int h) {
  // Adhere to U8g2's drawRBox constraints: r must be smaller than half of the
  // width/height. For integer arithmetic, this means r <= (dimension / 2) - 1.
  if (w < 2 * (r + 1)) {
    r = (w / 2) - 1;
  }
  if (h < 2 * (r + 1)) {
    r = (h / 2) - 1;
  }
  return (r < 0) ? 0 : r; // Ensure radius is not negative
}

void draw_eyes() {
  int r_left =
      calculate_safe_radius(corner_radius, left_eye.width, left_eye.height);
  int x_left = int(left_eye.x - left_eye.width / 2);
  int y_left = int(left_eye.y - left_eye.height / 2);
  g_draw_filled_round_rect(x_left, y_left, left_eye.width, left_eye.height,
                           r_left, G_COLOR_WHITE);

  int r_right =
      calculate_safe_radius(corner_radius, right_eye.width, right_eye.height);
  int x_right = int(right_eye.x - right_eye.width / 2);
  int y_right = int(right_eye.y - right_eye.height / 2);
  g_draw_filled_round_rect(x_right, y_right, right_eye.width, right_eye.height,
                           r_right, G_COLOR_WHITE);
}

void draw_frame() {
  g_clear_display();
  draw_eyes();
  g_update_display();
}

void reset_eyes(bool update) {
  // move eyes to the center of the display, defined by SCREEN_WIDTH,
  // SCREEN_HEIGHT
  left_eye.height = REF_EYE_HEIGHT;
  left_eye.width = REF_EYE_WIDTH;
  right_eye.height = REF_EYE_HEIGHT;
  right_eye.width = REF_EYE_WIDTH;

  left_eye.x = SCREEN_WIDTH / 2 - REF_EYE_WIDTH / 2 - REF_SPACE_BETWEEN_EYE / 2;
  left_eye.y = SCREEN_HEIGHT / 2;
  right_eye.x =
      SCREEN_WIDTH / 2 + REF_EYE_WIDTH / 2 + REF_SPACE_BETWEEN_EYE / 2;
  right_eye.y = SCREEN_HEIGHT / 2;

  corner_radius = REF_CORNER_RADIUS;

  if (update) {
    draw_frame();
  }
}

void blink(int speed) {
  reset_eyes(false); // Ensure starting from a clean state
  for (int i = 0; i < 3; i++) {
    left_eye.height -= speed;
    right_eye.height -= speed;

    // Dynamically adjust corner_radius during blink
    int current_h = left_eye.height;
    // Map current height (from min_blink_height to REF_EYE_HEIGHT) to a radius
    // (from 1 to REF_CORNER_RADIUS)
    int mapped_radius = map(current_h, 4, REF_EYE_HEIGHT, 1,
                            REF_CORNER_RADIUS); // Assuming min height is 4
    corner_radius = min(mapped_radius, current_h / 2);

    left_eye.width += 3;
    right_eye.width += 3;
    draw_frame();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye.height += speed;
    right_eye.height += speed;

    // Dynamically adjust corner_radius during blink
    int current_h = left_eye.height;
    int mapped_radius = map(current_h, 4, REF_EYE_HEIGHT, 1, REF_CORNER_RADIUS);
    corner_radius = min(mapped_radius, current_h / 2);

    left_eye.width -= 3;
    right_eye.width -= 3;
    draw_frame();
    delay(1);
  }
  reset_eyes();
}

void sleep() {
  reset_eyes(false);

  left_eye.height = 2;
  left_eye.width = REF_EYE_WIDTH;
  right_eye.height = 2;
  right_eye.width = REF_EYE_WIDTH;

  corner_radius = 0;
  draw_frame();
}
void wakeup() {
  reset_eyes(false);

  for (int h = 2; h <= REF_EYE_HEIGHT; h += 2) {
    left_eye.height = h;
    right_eye.height = h;
    int mapped_radius = map(h, 2, REF_EYE_HEIGHT, 1, REF_CORNER_RADIUS);
    // The corner radius can't be more than half the height, otherwise it
    // distorts.
    corner_radius = min(mapped_radius, h / 2);
    draw_frame();
  }
}

void happy_eye() {
  reset_eyes(true); // Draw the initial eyes to the screen
  int offset = REF_EYE_HEIGHT / 2;
  for (int i = 0; i < 10; i++) {
    // Draw the "happy" lower eyelid shape for both eyes
    g_draw_filled_triangle(
        left_eye.x - left_eye.width / 2 - 1, left_eye.y + offset,
        left_eye.x + left_eye.width / 2 + 1, left_eye.y + 5 + offset,
        left_eye.x - left_eye.width / 2 - 1,
        left_eye.y + left_eye.height + offset, G_COLOR_BLACK);
    g_draw_filled_triangle(
        right_eye.x + right_eye.width / 2 + 1, right_eye.y + offset,
        right_eye.x - right_eye.width / 2 - 2, right_eye.y + 5 + offset,
        right_eye.x + right_eye.width / 2 + 1,
        right_eye.y + right_eye.height + offset, G_COLOR_BLACK);
    offset -= 2;
    g_update_display();
    delay(1);
  }
  delay(1000);
  reset_eyes();
}

void saccade(int direction_x, int direction_y) {
  // quick movement of the eye, no size change. stay at position after movement,
  // will not move back,  call again with opposite direction direction == -1 :
  // move left direction == 1 :  move right

  const int MOVEMENT_AMPLITUDE_X = 8;
  const int MOVEMENT_AMPLITUDE_Y = 6;
  const int BLINK_AMPLITUDE = 8;

  // Animate the saccade in a single loop for simplicity
  for (int i = 1; i <= 2; i++) {
    left_eye.x += MOVEMENT_AMPLITUDE_X * direction_x;
    right_eye.x += MOVEMENT_AMPLITUDE_X * direction_x;
    left_eye.y += MOVEMENT_AMPLITUDE_Y * direction_y;
    right_eye.y += MOVEMENT_AMPLITUDE_Y * direction_y;

    // Eyes narrow during the first half of movement, then widen back
    int height_change = (i == 1) ? -BLINK_AMPLITUDE : BLINK_AMPLITUDE;
    right_eye.height += height_change;
    left_eye.height += height_change;

    draw_frame();
    delay(1);
  }
}

void move_big_eye(int direction);

void move_right_big_eye() { move_big_eye(1); }
void move_left_big_eye() { move_big_eye(-1); }

void move_big_eye(int direction) {
  // direction == -1 :  move left
  // direction == 1 :  move right
  reset_eyes(false);

  const int OVERSIZE_AMOUNT = 1;
  const int MOVEMENT_AMPLITUDE = 2;
  const int BLINK_AMPLITUDE = 5;

  // --- Move eyes out ---
  for (int i = 0; i < 3; i++) {
    left_eye.x += MOVEMENT_AMPLITUDE * direction;
    right_eye.x += MOVEMENT_AMPLITUDE * direction;
    right_eye.height -= BLINK_AMPLITUDE;
    left_eye.height -= BLINK_AMPLITUDE;

    EyeState *target_eye = (direction > 0) ? &right_eye : &left_eye;
    target_eye->height += OVERSIZE_AMOUNT;
    target_eye->width += OVERSIZE_AMOUNT;

    draw_frame();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye.x += MOVEMENT_AMPLITUDE * direction;
    right_eye.x += MOVEMENT_AMPLITUDE * direction;
    right_eye.height += BLINK_AMPLITUDE;
    left_eye.height += BLINK_AMPLITUDE;

    EyeState *target_eye = (direction > 0) ? &right_eye : &left_eye;
    target_eye->height += OVERSIZE_AMOUNT;
    target_eye->width += OVERSIZE_AMOUNT;

    draw_frame();
    delay(1);
  }

  delay(1000);

  // --- Move eyes back ---
  for (int i = 0; i < 3; i++) {
    left_eye.x -= MOVEMENT_AMPLITUDE * direction;
    right_eye.x -= MOVEMENT_AMPLITUDE * direction;
    right_eye.height -= BLINK_AMPLITUDE;
    left_eye.height -= BLINK_AMPLITUDE;

    EyeState *target_eye = (direction > 0) ? &right_eye : &left_eye;
    target_eye->height -= OVERSIZE_AMOUNT;
    target_eye->width -= OVERSIZE_AMOUNT;

    draw_frame();
    delay(1);
  }
  for (int i = 0; i < 3; i++) {
    left_eye.x -= MOVEMENT_AMPLITUDE * direction;
    right_eye.x -= MOVEMENT_AMPLITUDE * direction;
    right_eye.height += BLINK_AMPLITUDE;
    left_eye.height += BLINK_AMPLITUDE;

    EyeState *target_eye = (direction > 0) ? &right_eye : &left_eye;
    target_eye->height -= OVERSIZE_AMOUNT;
    target_eye->width -= OVERSIZE_AMOUNT;

    draw_frame();
    delay(1);
  }

  reset_eyes();
}

void init_eyes()

{

  g_init_display();

  sleep();
}

void launch_animation_with_index(int animation_index) {

  switch (animation_index) {
  case WAKEUP:
    wakeup();
    break;
  case RESET:
    reset_eyes(true);
    break;
  case MOVE_RIGHT_BIG:
    move_right_big_eye();
    break;
  case MOVE_LEFT_BIG:
    move_left_big_eye();
    break;
  case BLINK_LONG:
    blink(4);
    delay(1000);
    break;
  case BLINK_SHORT:
    blink(12);
    break; // This was missing, causing fall-through to HAPPY
  case HAPPY:
    happy_eye();
    break;
  case SLEEP:
    sleep();
    break;
  case SACCADE_RANDOM:
    reset_eyes(true);
    for (int i = 0; i < 20; i++) {
      int dir_x = random(-1, 2);
      int dir_y = random(-1, 2);
      saccade(dir_x, dir_y);
      delay(1);
      saccade(-dir_x, -dir_y);
      delay(1);
    }
    // BUG FIX: Moved the 'break' to the end of the case.
    break;
  default:
    // Do nothing for unknown animations
    break;
  }
}
