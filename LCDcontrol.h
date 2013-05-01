#include <avr/io.h>
#include <util/delay.h>

#define RS 2 //Pin B2
#define RW 1 //Pin B1
#define E 0  //Pin B0

/*
 * Initializes the LCD to the desired settings.
 * 8 bit data-length, 2 lines, 5x8 dots.
 * Display on, no cursor.
 * Right moving cursor, no shift
 */
void startLCD();

/*
 * Writes a single character to the display at the current cursor position
 */
void writeChar(char out);

/*
 * Takes in a string of chars, and prints them one by one to the display.
 */
void writeStr(char* str);

/*
 * Clears the display and waits 2 ms
 */
void clearDisp();

/*
 * Sends the return home function and waits 1 ms
 */
void returnHome();

/*
 * Shifts the entire display to the right or left, using extra memory spaces off of the display.
 * dir equal to 0 is a right shift, otherwise shift left.
 */
void displayShift(int dir);

 /*
  * Checks if the button input on Pin B3 is pressed or not.
  * Polls the button state every 1ms, 4 consecutive reads means that it's stable.
  * Returns 1 if the button is up, 0 if it is pressed.
  */
//int buttonPressed();
