#define F_CPU 16000000UL
#include <avr/io.h>

int main(void) {

  // LED (PB5 / D13) = output
  DDRB |= (1 << PB5);

  // Button (PD2 / D2) = input
  DDRD &= ~(1 << PD2);

  // Enable internal pull-up on D2
  PORTD |= (1 << PD2);

  while (1) {
    // If button pressed (reads LOW)
    if (!(PIND & (1 << PD2))) {
      PORTB |= (1 << PB5); // LED ON
    } else {
      PORTB &= ~(1 << PB5); // LED OFF
    }
  }
}
