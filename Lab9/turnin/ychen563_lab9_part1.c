/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
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

enum states{Start, Init, Note1, Note2, Note3} state;

unsigned char input;
unsigned short frequency;

void Tick(){
    input = ~PINA & 0x07;
    switch(state){//Transition
        case Start:
            state = Init;
            frequency = 0;
            break;
        case Init:
            if (input == 0x01){state = Note1;}
            else if (input == 0x02) {state = Note2;}
            else if (input == 0x04) {state = Note3;}
            else {state = Init;}
            break;
        case Note1:
            if (input == 0x01){state = Note1;}
            else{state = Init;}
            break;
        case Note2:
            if (input == 0x02){state = Note2;}
            else{state = Init;}
            break;
        case Note3:
            if (input == 0x04){state = Note3;}
            else{state = Init;}
            break;
    }//Transition
    switch(state){//State Action
       case Start:
           break;
       case Init:
           frequency = 0;
           break;
       case Note1:
           frequency = 261.63;
           break;
       case Note2:
           frequency = 293.66;
           break;
       case Note3:
           frequency = 329.63;
           break;
    }//State Action
    set_PWM(frequency);     
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
