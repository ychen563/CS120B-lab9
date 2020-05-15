/*	Author: ychen563
 *	Lab Section: 024
 *	Assignment: Lab #9  Exercise #2

 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
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

enum states{Start, Wait, Begin, Sound, End, Up, Down} state;

unsigned char input;
unsigned short frequency[9] = {0, 261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};
unsigned char i;

void Tick(){
    input = ~PINA & 0x07;
    switch(state){//Transition
        case Start:
            state = Wait;
            i = 0;
            break;
        case Wait:
            if (input == 0x04){state = Begin;}
            else {state = Wait;}
            break;
        case Begin:
            if (input & 0x04){state = Begin;}
            else if (!(input & 0x04)){state = Sound;i = 1;}
            break;
        case End:
            if (input & 0x04){state = End;}
            else if (!(input & 0x04)){state = Wait;i = 0;}
            break;
        case Sound:
            if (input == 0x01){state = Up; if(i < 8){++i;};}
            else if (input == 0x02) {state = Down;if (i > 1){--i;};}
            else if (input & 0x04) {state = End;}
            else {state = Sound;}
            break;
        case Up:
            if (input == 0x01){state = Up;}
            else{state = Sound;}
            break;
        case Down:
            if (input == 0x02){state = Down;}
            else{state = Sound;}
            break;
    }//Transition
    switch(state){//State Action
       case Start:
           break;
       case Wait:
           break;
       case Begin:
           break;
       case End:
           break;
       case Sound:
           break;
       case Up:
           break;
       case Down:
           break;
    }//State Action
    set_PWM(frequency[i]);     
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    state = Start;
    PWM_on();
    /* Insert your solution below */
    while (1) {
        Tick();
    }
    return 1;
}
