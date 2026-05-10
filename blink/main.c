#include <avr/io.h>
#include <util/delay.h>

int main(void) {

  // Pin 5 in port B is output
  DDRB |= (1 << PB5);
  DDRD &= ~(1 << PD2);

  while (1) {
    // Turn on Port B Pin 5
    PORTB |= (1 << PB5);
    _delay_ms(500);

    // Turn off Port B Pin 5
    PORTB &= ~(1 << PB5);
    _delay_ms(500);
  }
}
