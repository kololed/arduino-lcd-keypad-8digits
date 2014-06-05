//    A small demo program for LCD/Keypad shield
//    where the user can edit a decimal value (up to 8 digits)
//    
// 
//     Copyright (C) {2014}  {Ronan Barzic}
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


// This code is based on the following code :
// http://www.dfrobot.com/wiki/index.php?title=Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)#Example_use_of_LiquidCrystal_library
// Licence : unknown




#include <LiquidCrystal.h>


// DFRobot compatible shield
LiquidCrystal lcd(8,9,4,5,6,7);

#define pinIN 12
#define pinOUT 13

#define rMSG1 0 // The row used to print the message
#define cMSG1 8 // the column


// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int value =1000; 
char buffer[9]; // 8 chars + \0
char one_char_str[2];

unsigned char value_digits[8] = {0,0,0,0,1,0,0,0}; // 1000  by default
//                      0 1 2 3 4 5 6 7
unsigned pos = 4;

enum action_t {none,up,down,right,left,select} next_action;
enum state_t {stop,start} state;


void digits2string(void) {
    unsigned i;
    for(i=0;i<8;i++) {
        buffer[i] = value_digits[i] + '0'; 
    } 
    buffer[8] = 0;
}

// read the buttons
int read_LCD_buttons()
{
    adc_key_in = analogRead(0);      // read the value from the sensor 
    // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
    // we add approx 50 to those values and check to see if we are close
    if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
    // For V1.1 us this threshold
    if (adc_key_in < 50)   return btnRIGHT;  
    if (adc_key_in < 250)  return btnUP; 
    if (adc_key_in < 450)  return btnDOWN; 
    if (adc_key_in < 650)  return btnLEFT; 
    if (adc_key_in < 850)  return btnSELECT;  


    // For V1.0 comment the other threshold and use the one below:
    /*
      if (adc_key_in < 50)   return btnRIGHT;  
      if (adc_key_in < 195)  return btnUP; 
      if (adc_key_in < 380)  return btnDOWN; 
      if (adc_key_in < 555)  return btnLEFT; 
      if (adc_key_in < 790)  return btnSELECT;   
    */


    return btnNONE;  // when all others fail, return this...
}

void setup()
{
    lcd.begin(16, 2);              // start the library
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Value : "); // print a simple message
    lcd.setCursor(cMSG1,rMSG1);
    digits2string();
    lcd.print(buffer);
    pinMode(pinOUT, OUTPUT); 
    pinMode(pinIN, INPUT); 
 
}


void loop()
{
    lcd.setCursor(cMSG1+pos,rMSG1);
    digits2string();
    one_char_str[0] = buffer[pos];
    one_char_str[1] = 0;
    lcd.print(one_char_str);      
    lcd.setCursor(cMSG1+pos,rMSG1);

   
    // lcd.setCursor(0,1);            // move to the begining of the second line
    lcd_key = read_LCD_buttons();  // read the buttons

    switch (lcd_key)               // depending on which button was pushed, we perform an action
    {
    case btnRIGHT:
        {
            next_action = right;
            break;
        }
    case btnLEFT:
        {
            next_action = left;
            break;
        }
    case btnUP:
        {
            next_action = up;
      
            break;
        }
    case btnDOWN:
        {
            next_action = down;
            break;
        }
    case btnSELECT:
        {
            next_action = select;
            break;
        }
    case btnNONE:
        {
            switch(next_action) {
            case up: {  
                if(state==stop) {
                    value_digits[pos] = (value_digits[pos] == 9) ? 0 : value_digits[pos]+1 ; 
                }
                break;
            }
            case down: {
                if(state==stop) {
                    value_digits[pos] = (value_digits[pos] == 0) ? 9 : value_digits[pos]-1 ; 
                }
                break;
            }
            case left: {
                if(state==stop) {
                    pos = (pos==0) ? 0 : pos-1;
                }
                break;
            }
            case right: {
                if(state==stop) {
                    pos = (pos==7) ? 7 : pos+1; 
                }
                break;
            }
            case select: {
                if(state ==stop) {
                    lcd.setCursor(0,1);
                    lcd.noCursor();
                    lcd.print("START");
                    state = start;
                } else {
                    lcd.setCursor(0,1);
                    lcd.noCursor();
                    lcd.print("STOP ");
                    lcd.setCursor(cMSG1+pos,rMSG1);
                    lcd.cursor();
                    state = stop;
                }
            }
            default:
                break;
            }  
            delay(100);
            next_action = none;  
     
            break;
        }
    }

}

