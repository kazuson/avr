#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

 #define SW ((~PINC>>4)&3)

 ISR(PCINT1_vect)
 {
 // ブザー停止中にスイッチが押されたら
 if ((TCCR2B == 0) && ( SW == 1)) {
 TCCR2B = 0x02; // 音の再生開始
 TCCR0B = 3; // タイマ割り込み開始
 } 
 if ((TCCR2B == 0) && ( SW == 2)) {
 TCCR2B = 0x05; // 音の再生開始
 TCCR0B = 3; // タイマ割り込み開始
 }
 }

 ISR(TIMER0_COMPA_vect)
 {
 static int n = 0;
 if (n == 500) {
 TCCR2B = 0; // 音を停止
 TCCR0B = 0; // タイマ割り込み停止
 TCNT0 = 0; // 計時用のタイマクリア
 OCR2A = 238;
 n = 0;
 return;
}
 OCR2A--;
 n++;
 }

 int main()
 {
 DDRD = 0x08;
 DDRC = 0x0;
 PORTC = 0x30;

 // ブザー（CTC MODE, PRESCALE 64）
 TCCR2A = 0x12; // COM OC2B(TOGGLE),

 // 2ms ごとの割り込み
 OCR0A = 249;
 TCCR0A = 2;
 TCCR0B = 0;
 TIMSK0 |= (1 << OCIE0A);

 // ピン変化割り込み
 PCICR = _BV(PCIE1);
 PCMSK1 = _BV(PCINT12) | _BV(PCINT13);

 // 割り込み開始
 sei();

 for (;;) {
 wdt_reset();
 }

 return 0;
 }
