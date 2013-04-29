#include <avr/io.h>
#include <util/delay.h>

#define RS 2 //Pin B2
#define RW 1 //Pin B1
#define E 0  //Pin B0

void startLCD();
void pulseEnable();
void writeChar(char out);
void writeStr(char* str);
void clearDisp();
void returnHome();
void displayShift(int dir);
//int buttonPressed();
