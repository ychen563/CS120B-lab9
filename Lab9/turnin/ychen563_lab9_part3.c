/*	Author: ychen563
 *	Lab Section: 024
 *	Assignment: Lab #9  Exercise #3

 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

void set_PWM(double frequency) {
    static double current_frequency;
    if (frequency != current_frequency){
        if (!frequency){ TCCR3B &= 0x08; }
        else {TCCR3B |= 0x03; }
  
        if (frequency < 0.954) { OCR3A = 0xFFFF; }
        
        else if (frequency > 31250) { OCR3A = 0x0000; }

        else {OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

        TCNT3 = 0;
        current_frequency = frequency;
    }
}

void PWM_on() {
    TCCR3A = (1 << COM3A0);
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    set_PWM(0);
}

void PWM_off(){
    TCCR3A = 0x00;
    TCCR3B = 0x00;
}

enum states{Start, Wait, End, Sound} state;

unsigned char input;
unsigned short note[17] = {0, 523.25, 493.88, 523.25, 523.25, 392.00, 392.00, 349.23, 349.23, 523.25, 493.88, 523.25, 523.25, 392.00, 392.00, 349.23, 349.23};
unsigned char i;
unsigned char j;


void Tick(){
    input = ~PINA & 0x04;
    switch(state){//Transition
        case Start:
            state = Wait;
            j = 0;
            break;
        case Wait:
            i = 0;
            j = 0;
            if (input == 0x04){state = Sound;j = 1;}
            else {state = Wait;}
            break;
        case End:
            if (input == 0x04){state = End;}
            else {state = Wait;}
            break;
        case Sound:
            if (i < 15){j++;i++;}
            else{
                j = 0;
                state = End;
            }
            break;
        default:
            state = Wait;
            break;
    }//Transition
    switch(state){//State Action
        case Start:
           break;
        case Wait:
           break;
        case Sound:
           break;
        default:
           break;
    }//State Action
    set_PWM(note[j]);     
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(313);
    TimerOn();

    state = Start;
    PWM_on();
    /* Insert your solution below */
    while (1) {
        while (!TimerFlag){}  // Wait for Tick's period
      	TimerFlag = 0;        // Lower flag
    	Tick();
    }
    return 1;
}
