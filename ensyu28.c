#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

unsigned char k=0;

volatile unsigned char led[3][8] = {
    {
        0b00011000,
        0b00111100,
        0b01100110,
        0b01100110,
        0b01100110,
        0b01100110,
        0b00111100,
        0b00011000,
    },
    {
        0b00011000,
        0b00111000,
        0b01111000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b00011000,
        0b01111110,
    },
    {
        0b00111100,
        0b01100110,
        0b01100110,
        0b00001110,
        0b00011100,
        0b00111000,
        0b01110000,
        0b01111110,
    }
};

/* タイマ割り込み*/
ISR(TIMER1_COMPA_vect)
{
    k++;
    if(k == 3){
        k = 0;
    }
}

int main(void)
{
    static unsigned char scan; // LED 走査カウンタ
    unsigned char sc;
    int i;
    DDRB = 0xff;
    DDRC = 0x0f;
    DDRD = 0xfa;

/* タイマ割り込み設定*/
    OCR1A = 32767; /* 2mS */
    TCCR1A = 0;
    TCCR1B = 12; /* 1/64 */
    TIMSK1 |= (1 << OCIE1A);

    sei();
    for (;;) {
        PORTB = 0;
        scan = (scan + 1) & 7;
        sc = ~(1 << scan);
        PORTC = sc & 0x0f;
        PORTD = sc & 0xf0;
        PORTB = led[k][7-scan];
        
        for(i=0;i<1000;i++){
            wdt_reset();
        }
    }

    return 0;
}
