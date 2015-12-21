#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

 #define SW ((~PINC>>4)&3)

 ISR(PCINT1_vect)
 {
 // �u�U�[��~���ɃX�C�b�`�������ꂽ��
 if ((TCCR2B == 0) && ( SW == 1)) {
 TCCR2B = 0x02; // ���̍Đ��J�n
 TCCR0B = 3; // �^�C�}���荞�݊J�n
 } 
 if ((TCCR2B == 0) && ( SW == 2)) {
 TCCR2B = 0x05; // ���̍Đ��J�n
 TCCR0B = 3; // �^�C�}���荞�݊J�n
 }
 }

 ISR(TIMER0_COMPA_vect)
 {
 static int n = 0;
 if (n == 500) {
 TCCR2B = 0; // �����~
 TCCR0B = 0; // �^�C�}���荞�ݒ�~
 TCNT0 = 0; // �v���p�̃^�C�}�N���A
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

 // �u�U�[�iCTC MODE, PRESCALE 64�j
 TCCR2A = 0x12; // COM OC2B(TOGGLE),

 // 2ms ���Ƃ̊��荞��
 OCR0A = 249;
 TCCR0A = 2;
 TCCR0B = 0;
 TIMSK0 |= (1 << OCIE0A);

 // �s���ω����荞��
 PCICR = _BV(PCIE1);
 PCMSK1 = _BV(PCINT12) | _BV(PCINT13);

 // ���荞�݊J�n
 sei();

 for (;;) {
 wdt_reset();
 }

 return 0;
 }
