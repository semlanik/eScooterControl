/*
 * MIT License
 *
 * Copyright (c) 2020 Alexey Edelev <semlanik@gmail.com>
 *
 * This file is part of eScooterControl project https://github.com/semlanik/eScooterControl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this
 * software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and
 * to permit persons to whom the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include "display.h"

#include "pinconfig.h"

#include <avr/pgmspace.h>

const PROGMEM byte BatteryLevel = B00000010;

const PROGMEM byte Digit[10][5] = {
  {
    B00000111,
    B00000101,
    B00000101,
    B00000101,
    B00000111
  },{
    B00000110,
    B00000101,
    B00000100,
    B00000100,
    B00000100
  },{
    B00000111,
    B00000100,
    B00000111,
    B00000001,
    B00000111
  },{
    B00000111,
    B00000100,
    B00000111,
    B00000100,
    B00000111
  },{
    B00000101,
    B00000101,
    B00000111,
    B00000100,
    B00000100
  },{
    B00000111,
    B00000001,
    B00000111,
    B00000100,
    B00000111
  },{
    B00000111,
    B00000001,
    B00000111,
    B00000101,
    B00000111
  },{
    B00000111,
    B00000100,
    B00000110,
    B00000001,
    B00000001
  },{
    B00000111,
    B00000101,
    B00000111,
    B00000101,
    B00000111
  },{
    B00000111,
    B00000101,
    B00000111,
    B00000100,
    B00000111
  }
};

const PROGMEM byte AcceleratorLevel[10][6] = {
    {
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000001
  },{
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B00000001,
    B00000001
  },{
    B00000000,
    B00000000,
    B00000000,
    B00000011,
    B00000011,
    B00000001
  },{
    B00000000,
    B00000000,
    B00000010,
    B00000111,
    B00000011,
    B00000001
  },{
    B00000000,
    B00000100,
    B00000110,
    B00000111,
    B00000011,
    B00000001
  },{
    B00000000,
    B00001100,
    B00001110,
    B00000111,
    B00000011,
    B00000001
  },{
    B00000000,
    B00011100,
    B00011110,
    B00000111,
    B00000011,
    B00000001
  },{
    B00000000,
    B00111100,
    B00111110,
    B00000111,
    B00000011,
    B00000001
  },{
    B00000000,
    B01111100,
    B01111110,
    B00000111,
    B00000011,
    B00000001
  },{
    B00000000,
    B11111100,
    B11111110,
    B00000111,
    B00000011,
    B00000001
  }
};

Display::Display() : mLedControl(DisplayDataPin, DisplayClkPin, DisplayCsPin,2)
 ,mDisplayBuffer{
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000}
  , m_acceleration(0)
  , m_battery(0)
  , m_speed(0)
{
  mLedControl.shutdown(0, false);
  mLedControl.setIntensity(0, 0);
  mLedControl.clearDisplay(0);  

  mLedControl.shutdown(1, false);
  mLedControl.setIntensity(1, 0);
  mLedControl.clearDisplay(1);  
}

void Display::drawAccelerationLevel()
{
  if (m_acceleration > 10) {
    m_acceleration = 10;
  }
  
  for (byte i = 0; i < 6; ++i) {
    mDisplayBuffer[8 + i] = 0;
    if (m_acceleration > 0) {
      mDisplayBuffer[8 + i] = pgm_read_byte(&(AcceleratorLevel[m_acceleration - 1][i]));
    }
  }
}


void Display::drawSpeed()
{
  if (m_speed > 99) {
    m_speed = 99;
  }
  byte tens = m_speed / 10;
  byte ones = m_speed % 10;

  for (byte i = 0; i < 5; ++i) {
    mDisplayBuffer[i] = 0;
    byte tensDigit = pgm_read_byte(&(Digit[tens][i]));
    byte onesDigit = pgm_read_byte(&(Digit[ones][i]));
    mDisplayBuffer[i] = (tensDigit | onesDigit << 4);
  }
}

void Display::drawBatteryLevel()
{ 
  if (m_battery > 5) {
    m_battery = 5;
  }

  mDisplayBuffer[6] = 0;
  mDisplayBuffer[7] = 0;

  byte batteryLevelChar = pgm_read_byte(&(BatteryLevel));
  for (byte i = 0; i < m_battery; ++i) {
    mDisplayBuffer[6] |= batteryLevelChar << i;
    mDisplayBuffer[7] |= batteryLevelChar << (i + 1);
  }
}

void Display::updateDisplayBuffer()
{
  drawAccelerationLevel();
  drawSpeed();
  drawBatteryLevel();
  for(byte i = 0; i < 8; ++i) {
    mLedControl.setColumn(1, i, mDisplayBuffer[i]);    
    mLedControl.setColumn(0, i, mDisplayBuffer[i + 8]);    
  }
}
