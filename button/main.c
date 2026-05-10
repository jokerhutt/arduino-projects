#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void) {

    // LED (PB5 / D13) = OUTPUT
    DDRB |= (1 << PB5);

    // Button (PD2 / D2) INPUT
    DDRD &= ~(1 << PD2);

    // Pull up on D2
    PORTD |= (1 << PD2);

    uint8_t ledState = 0;
    uint8_t lastButton = 0;

    while (1) {

        uint8_t isPressed = !(PIND & (1 << PD2));

        // if pressed
        if (!lastButton && isPressed) {
            ledState = !ledState;

            // If turning on
            if (ledState == 1) {
                PORTB |= (1 << PB5); // LED ON
            } else {
                PORTB &= ~(1 << PB5); // LED OFF
            }

            _delay_ms(200);
        }

        lastButton = isPressed;
    }
}
