/**************************************************************
  Name      ShiftRegister_8leds_1                                
  Author    Bob Powell 
            texanfromiowa@gmail.com
            Copyright (C) 2012-2013, Parallelus Automation, Inc.
          
  Date      Oct 20, 2012    
  Modified  Apr 2, 2013                                
  Version   1.0.5      
  Arduino   1.0.4
  
  Notes     Example for using one 74HC595 Shift Register and LEDs.          
            Various test functions included for fun.  

            This program is heavily commented to explain a number
            of concepts.  PLEASE READ.
            
            This example uses standard "shiftOut" function to send data
            to the shift register.  This library include functions
            highByte and lowByte for sending two bytes of data to two
            8 bit shift registers.  This is the limitation of this 
            library.
            
            At various points in the code, you will see variations
            of this:   1<<i   This is bit shifting.  If you want a full
            explaination, go here: http://en.wikipedia.org/wiki/Logical_shift
            The bit shifting is used because of the inherent problems 
            with floating point numbers.  The prefered method for 
            calculating an exponent would be, for example, to use:
            
            pow(2,3) which would equal 4 (the number 2, cubed (2^3))

            Unfortuntely, the function returns a float which may not
            be exactly 8 (ie. 7.998761234).  Using C casting to force an
            integer would set this to a 7, which is unacceptable.  
            Rather write code to fix this, bit shifting was used, which 
            ALWAYS give you an integer, which is what we want anyway.
            
            
Legal Stuff:
============
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
at your option, any later version.
	 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
	 
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
          
   
   
     
Personal note:

If you do something interesting with this code, expand it, or just
have a question, please email it to me at the address above.  

I hope you find this example helpful.  Enjoy.

Bob


****************************************************************/

// Set some variables - Global variables, except for the Arduino
// pins, are CAPITALIZED.

// These variables are too make things a little easier for humans to read.
// The "const" sets the variable so it cannot be changed
// later in the program (ie. make them constants).  The standard
// constants true and false function just as well.
const int OFF = 0;
const int ON = 1;

// In this example, this chip is used: http://www.ti.com/lit/ds/symlink/sn74hc595.pdf
// The names below match those in the datasheet.
//Pin connected to SRCLK, pin 11, of 74HC595
const int latchPin = 10;
//Pin connected to RCLK pin 12, of 74HC595
const int clockPin = 11;
//Pin connected to SER, pin 14, of 74HC595
const int dataPin = 12;

/* Other 74HC595 pins:

  Pin 9, data output, is not used

  Connect SRCLR (master reset), pin 10, to 5V.  In this example,
  the chip will not be reset and must be set HIGH for the chip
  to function.  Setting this pin LOW resets the register.

  Connect OE (output enable), pin 13, to GRD (LOW).
  In this example, we are not using the 74HC595 for any
  data input, so we never switch this pin from HIGH
  to LOW, to switch if from inputing data to outputing 
  data repestively.
  
*/

// In this example, the shift register is 8 bits, so we use 8 leds.
const int NUM_LED = 8;

/*  Those of you who program will recognize the LED array is
    used essentially as a hash table. Rather that use a hash table, 
    using an array was a better programming example right now, and
    consistant with what other Arduino examples do.
    I did not want to introduce a hash table, which is a
    two-dementional array abstration.
*/ 
int LED[NUM_LED];        

const int ALLOFF = 0;
const int ALLON = (1<<NUM_LED) - 1;
// Create variable that will be set in setup()
// The variables are used later in various flashing and marque functions.
int ODDLEDS = 0;
int EVENLEDS = 0;




/**************************************************************
Function: setup
Purpose:  set up Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino
***************************************************************/
void setup() {
  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT); 
  
  // turn off all the LEDS, just in case
  allOff();

  // initialize the array OFF
  clearArray();
  
  // Create the values for the variables "ODDLEDS" 
  // and "EVENLEDS" for the future use.
  // See the comment above about bit shifting.
  int ti = 0;
  for(int i = 0; i < NUM_LED; i += 2){    
    ti = ODDLEDS;
    ODDLEDS = (1<<i) + ti;
  } 
  ti = 0;
  for(int i = 1; i < NUM_LED; i += 2){
    ti = EVENLEDS;
    EVENLEDS = (1<<i) + ti;
  } 
  
  
  // From http://arduino.cc/en/Reference/random
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generate
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

}  // end of setup 



/**************************************************************
Function: loop
Purpose:  loop funtion for Arduino
Args:     none
Returns:  nothing
Notes:    This function is required by the Arduino, and the 
          Arduino will loop through this function indefinately.
***************************************************************/
void loop() {
  
  // Flash all leds to let us know it on
  flashAll(1000, 2);
  delay(1000);
  
  // Meter effect
  for(int i = 0; i < 6; i++){
    int randomnum = random(NUM_LED);
    meter(50, randomnum, false);
  }
  delay(1000);
  // Meter effect in reverse
  for(int i = 0; i < 6; i++){
    int randomnum = random(NUM_LED);
    meter(50, randomnum, true);
  }
  delay(2000);

  // Flash odd only, then even only leds
  flash(ODDLEDS, 250, 3);
  flash(EVENLEDS, 250, 3);
  delay(2000);
  
  // Marque effect
  marque(200, 10);
  delay(2000);
  
  // criss-cross effect
  crissCross(150, 8);
  delay(2000);
  
  
  // Randomly turn on and off leds
  randomLeds(250,25);
  delay(2000);
  
  
  // Chase effect
  chase(250, 3, 3, false);
  delay(2000);
  // Chase effect in reverse
  // Note that the only differnce is the "true"
  chase(250, 3, 3, true);
  delay(2000);

  
  // The Knight Rider effect
  knightRider(75, 10, 3);
  delay(2000);
  
  //  Flash again
  flashAll(500, 2);
  delay(2000);  
  
  // Count up from a number to another -- in binary!!
  countUp(ALLOFF, ALLON, 50, false);
  delay(2000);
  
  // Flash odd then even leds
  flash(ODDLEDS, 250, 3);
  flash(EVENLEDS, 250, 3);
  delay(2000);
  
  // count up again, but in reverse
  countUp(ALLOFF, ALLON, 100, true);
  allOff();
  delay(2000);
  
  // count down from a number to another
  countDown(ALLON, ALLOFF, 100, false);
  delay(2000);
   
  allOn();
  delay(1000);
  allOff(); 

}  // End of Arduino Loop



/**************************************************************
Function: flashAll
Purpose:  Flashes all leds
Args:     int duration - how long an led is on 
          int count - how many times to flash
Returns:  nothing
Notes:    This function is a convenience function for flash()
***************************************************************/
void flashAll(int duration, int count)
{
  flash(ALLON, duration, count);
}  


/**************************************************************
Function: flash
Purpose:  Flashes leds
Args:     int num - number to be flashed
          int duration - how long an led is on 
          int count - how many times to flash
Returns:  nothing
Notes:    
***************************************************************/
void flash(int num, int duration, int count)
{
  allOff();
  for(int i = 0; i < count; i++)
  {
    writeBits(num);
    delay(duration);
    writeBits(ALLOFF);
    delay(duration);
  }
}  

/**************************************************************
Function: crissCross
Purpose:  Criss-Cross effect
Args:     int duration - how long an led is on 
          int count - how many times to cycle through effect
Returns:  nothing
Notes:    
***************************************************************/
void crissCross(int duration, int count)
{
  // Clear the array and turns off any leds still on 
  clearArray();
  allOff();
  // Create variables
  int firstled;
  int lastled;
  
  for(int i = 0; i < count; i++)
  {
    for(int j = 0; j < NUM_LED; j++)
    {
      // if its the start of a loop through the leds,
      // reset the variables to their starting position.
      if(j == 0){
        firstled = 0;
        lastled = NUM_LED -1;
      }
      LED[firstled] = ON;
      LED[lastled] = ON;
      writeBits(sumArray(LED));
      // Skip the delay if the center leds are crossing
      // or if the leds have reached the ends.
      // This code was changed to be human readable and 
      // is not the most effecient.
      if(lastled == (firstled +1)){}
        // do nothing - skip delay
      else if (lastled == 0){}
        // do nothing - skip delay
      else
        delay(duration);
      // Turn off the last leds we just turned on
      LED[firstled] = OFF;
      LED[lastled] = OFF;
      // increment variables
      firstled++;
      lastled--;
    }
  }
  
  // turn the last leds off
  delay(duration);
  allOff();
}


/**************************************************************
Function: randomLeds
Purpose:  Randomly turn on and off leds
Args:     int duration - time between random number generations 
          int count - number of cycles through the loop
Returns:  nothing
Notes:    
***************************************************************/
void randomLeds(int duration, int count)
{
  // Clear the array and turns off any leds still on 
  clearArray();
  allOff();
  // Create variables
  int randomnum;
  
  for(int i = 0; i < count; i++)
  {
    randomnum = random(NUM_LED);
    if(LED[randomnum] == ON)
    {
      LED[randomnum] = OFF;
      writeBits(sumArray(LED));
    }
    else
    {
      LED[randomnum] = ON;
      writeBits(sumArray(LED));
    }    
    delay(duration);
  }
  
  // Randomly turn off any remaining leds
  while(sumArray(LED) > 0)
  {
    randomnum = random(NUM_LED);
    LED[randomnum] = OFF;
    writeBits(sumArray(LED));
    delay(duration);
  } 

}

/**************************************************************
Function: marque
Purpose:  Marque effect
Args:     int duration - how long an led is on 
          int count - how many times to cycle through effect
Returns:  nothing
Notes:    This funtion is flashing the odd leds, then 
          flashing even leds to create the marque effect.         
**************************************************************/
void marque(int duration, int count)
{
  // Clear the array and turns off any leds still on 
  clearArray();
  allOff();  

  for(int i = 0; i < count; i++)
  {
    writeBits(ODDLEDS);
    delay(duration);
    writeBits(EVENLEDS);
    delay(duration);  
  }
  
  // Turn off when finished
  allOff();
}

/**************************************************************
Function: chase
Purpose:  Chase effect
Args:     int duration - how long an led is on 
          int count - how many times to cycle through effect
          int numleds - how many leds are on at a time
          bool reverse - true or false - a value of true
                       - reverses the normal direction
Returns:  nothing
Notes:    
**************************************************************/
void chase(int duration, int count, int numleds, bool reverse)
{
  // Clear the array and turns off any leds still on 
  allOff();
  clearArray();

  duration = duration / numleds;
  int trailingleds = numleds - 1;
  int ledcounter = NUM_LED + trailingleds;
  
  for(int i = 0; i < count; i++)
  {
    for(int j = 0; j < ledcounter; j++)
    {
      LED[j] = ON;
      if (reverse == true)
        writeBitsRev(sumArray(LED));
      else
        writeBits(sumArray(LED));
      delay(duration);
      LED[j + 1] = ON;
      LED[j - trailingleds] = OFF;
    }    
  } 
  
  // turn off the last led
  allOff(); 
  clearArray();
}

/**************************************************************
Function: meter
Purpose:  Meter effect
Args:     int duration - how long an led is on 
          int num - how many times to cycle through effect
          bool reverse - true or false - a value of true
                       - reverses the normal direction
Returns:  nothing
Notes:    
**************************************************************/
void meter(int duration, int num, bool reverse)
{
  // Clear the array and turns off any leds still on 
  allOff();
  clearArray();
  
  // if the number is greater than the number of leds, then return
  if(num > NUM_LED)
    return;
    
    
  // turn on the leds  
  for(int i = 0; i < num; i++)
  {
    LED[i] = ON;
    if (reverse == true)
      writeBitsRev(sumArray(LED));
    else
      writeBits(sumArray(LED));
    delay(duration);     
  } 

  // turn off the leds  
  for(int i = num; i > 0; i--)
  {
    LED[i] = OFF;
    if (reverse == true)
      writeBitsRev(sumArray(LED));
    else
      writeBits(sumArray(LED));
    delay(duration);     
  }  
  // turn off the last led
  allOff(); 
}


/**************************************************************
Function: knightRider
Purpose:  "Knight Rider" effect from the TV show
Args:     int duration - how long an led is on 
          int count - how many times to cycle through effect
          int numleds - how many leds are on at a time
Returns:  nothing
Notes:    This is not a perfect implementaion of the effect.
          A perfect effect would include a dimming of the 
          trainling leds (using PWM).  
**************************************************************/
void knightRider(int duration, int count, int numleds)
{
  // Clear the array and turns off any leds still on 
  allOff();
  clearArray();

  duration = duration / numleds;
  int trailingleds = numleds - 1;
  int ledcounter = NUM_LED + trailingleds;
  
  for(int i = 0; i < count; i++)
  {
    // cycle through one direction
    for(int j = 0; j < ledcounter -1 ; j++)
    {
      LED[j] = ON;
      writeBits(sumArray(LED));
      delay(duration);
      LED[j + 1] = ON;
      LED[j - trailingleds] = OFF;
      writeBits(sumArray(LED));
      // Skip the extra delay() commands as the 
      // trailing leds catch up.
      if(j < NUM_LED)
        delay(duration);    
    }
    // cycle back in the oppisite direction
    for(int j = ledcounter; j > -trailingleds; j--)
    {
      LED[j] = ON;
      writeBits(sumArray(LED));
      // Skip the initial delay() commands as the 
      // trailing leds start back.
      if(j < NUM_LED -1)
        delay(duration);
      LED[j - 1] = ON;
      LED[j + trailingleds] = OFF;
      writeBits(sumArray(LED));
      delay(duration);    
    }
  }
  
 // Turn the last led off 
 allOff(); 
 clearArray();
}

/**************************************************************
Function: countUp
Purpose:  Count up, in binary, from one number to another
Args:     int duration - how long an led is on 
          int startNum - starting number
          int endNum - ending number
          bool reverse - true or false - a value of true
                       - reverses the normal direction
Returns:  nothing
Notes:    
**************************************************************/
void countUp(int startNum, int endNum, int duration, bool reverse)
{
 // Clear the array and turns off any leds still on 
 allOff(); 
 clearArray();

 if(endNum <= startNum)
    return;
    
  for (int numberToDisplay = startNum; numberToDisplay <= endNum; numberToDisplay++) {
    if(reverse == true)
      writeBitsRev(numberToDisplay);
    else
      writeBits(numberToDisplay);
    
    delay(duration);
  }
} 

/**************************************************************
Function: countDown
Purpose:  Count down, in binary, from one number to another
Args:     int duration - how long an led is on 
          int startNum - starting number
          int endNum - ending number
          bool reverse - true or false - a value of true
                       - reverses the normal direction
Returns:  nothing
Notes:    
**************************************************************/
void countDown(int startNum, int endNum, int duration, bool reverse)
{
 // Clear the array and turns off any leds still on 
 allOff(); 
 clearArray();

 if(endNum >= startNum)
    return;
    
  for (int numberToDisplay = startNum; numberToDisplay > endNum; numberToDisplay--) {
    if(reverse == true)
      writeBitsRev(numberToDisplay);
    else
      writeBits(numberToDisplay);
    
    delay(duration);
  }
}  
 
 
 
/**************************************************************
Function: sumArray
Purpose:  Take a binary representation of the array of ON and OFF
          positions, and generate an integer.
Args:     int array[]
Returns:  integer of the total
Notes:    See comments on bit shifting at the beginning
          of this file.
**************************************************************/  
int sumArray(int array[])
{
   int total = 0;
   
   for (int i = 0; i < NUM_LED; i++)
   {
     if (array[i] == ON)
	 total = total + (1<<i);
   }
   
   return(total);
}

/**************************************************************
Function: allOn
Purpose:  Turns ON all leds
Args:     none
Returns:  none
Notes:    
**************************************************************/  
void allOn()
{
  writeBits(ALLON);
}

/**************************************************************
Function: allOff
Purpose:  Turns OFF all leds
Args:     none
Returns:  none
Notes:    
**************************************************************/  
void allOff()
{
   writeBits(ALLOFF);
}


/**************************************************************
Function: clearArray
Purpose:  Resets all positions of LED[] to OFF
Args:     none
Returns:  none
Notes:    
**************************************************************/ 
void clearArray()
{
  for (int i = 0; i < NUM_LED; i++)
   {
     LED[i] = OFF;
   }
}


/**************************************************************
Function: writeBits
Purpose:  Write an integer to the shift register with
          Most Significant Bit first (MSFBFIRST)
Args:     int num - number to sent to shift register
Returns:  nothing
Notes:    This function is a convenience function for outBits()
***************************************************************/
void writeBits(int num)
{
  outBits(MSBFIRST, num);
}

/**************************************************************
Function: writeBitsRev
Purpose:  Write an integer to the shift register with
          Least Significant Bit first (LSFBFIRST)
Args:     int num - number to sent to shift register
Returns:  nothing
Notes:    This function is a convenience function for outBits().
          It has the effect of reversing normal output.
***************************************************************/
void writeBitsRev(int num)
{
  outBits(LSBFIRST, num);
}


/**************************************************************
Function: outBits
Purpose:  Send integer to shift register
Args:     int num - number to sent to shift register
          int dir - First or Least significant bit
Returns:  nothing
Notes:    This is where data is actually sent out.
***************************************************************/
void outBits(int dir, int num)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, dir, num);
  digitalWrite(latchPin, HIGH);
  
}
