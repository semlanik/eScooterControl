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

const PROGMEM byte Digit[10][5] = {
  {
    0b00000111,
    0b00000101,
    0b00000101,
    0b00000101,
    0b00000111
  },{
    0b00000110,
    0b00000101,
    0b00000100,
    0b00000100,
    0b00000100
  },{
    0b00000111,
    0b00000100,
    0b00000111,
    0b00000001,
    0b00000111
  },{
    0b00000111,
    0b00000100,
    0b00000111,
    0b00000100,
    0b00000111
  },{
    0b00000101,
    0b00000101,
    0b00000111,
    0b00000100,
    0b00000100
  },{
    0b00000111,
    0b00000001,
    0b00000111,
    0b00000100,
    0b00000111
  },{
    0b00000111,
    0b00000001,
    0b00000111,
    0b00000101,
    0b00000111
  },{
    0b00000111,
    0b00000100,
    0b00000110,
    0b00000001,
    0b00000001
  },{
    0b00000111,
    0b00000101,
    0b00000111,
    0b00000101,
    0b00000111
  },{
    0b00000111,
    0b00000101,
    0b00000111,
    0b00000100,
    0b00000111
  }
};

const PROGMEM byte AcceleratorLevel[10][6] = {
    {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000001
  },{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000001,
    0b00000001,
    0b00000001
  },{
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000011,
    0b00000011,
    0b00000001
  },{
    0b00000000,
    0b00000000,
    0b00000010,
    0b00000111,
    0b00000011,
    0b00000001
  },{
    0b00000000,
    0b00000100,
    0b00000110,
    0b00000111,
    0b00000011,
    0b00000001
  },{
    0b00000000,
    0b00001100,
    0b00001110,
    0b00000111,
    0b00000011,
    0b00000001
  },{
    0b00000000,
    0b00011100,
    0b00011110,
    0b00000111,
    0b00000011,
    0b00000001
  },{
    0b00000000,
    0b00111100,
    0b00111110,
    0b00000111,
    0b00000011,
    0b00000001
  },{
    0b00000000,
    0b01111100,
    0b01111110,
    0b00000111,
    0b00000011,
    0b00000001
  },{
    0b00000000,
    0b11111100,
    0b11111110,
    0b00000111,
    0b00000011,
    0b00000001
  }
};

const PROGMEM byte StopSign[8] = {
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00011000,
    0b00011000
};

const PROGMEM byte LampSign[5] = {
    0b00000000,
    0b00000000,
    0b01000000,
    0b11100000,
    0b01000000
};

const PROGMEM byte EcoSign[5] = {
    0b00000000,
    0b00000000,
    0b00001100,
    0b00001100,
    0b00000100
};

Display::Display() : mLedControl(DisplayDataPin, DisplayClkPin, DisplayCsPin,2)
 ,mDisplayBuffer{
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000}
  , m_updateRequested(true)
  , m_acceleration(0)
  , m_battery(0)
  , m_speed(0)
  , m_stop(false)
  , m_lightOn(false)
  , m_eco(false)
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
    mDisplayBuffer[5 + i] = 0;
    if (m_acceleration > 0) {
      mDisplayBuffer[5 + i] = pgm_read_byte(&(AcceleratorLevel[m_acceleration - 1][i]));
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
  if (m_battery > 6) {
    m_battery = 6;
  }

  mDisplayBuffer[15] = 0;
  for (byte i = 1; i < (m_battery + 1); ++i) {
    mDisplayBuffer[15] |= 1 << i;
  }
}

void Display::drawLampSign()
{
  for (byte i = 0; i < 5; ++i) {
    if (m_lightOn) {
      mDisplayBuffer[9 + i] |= pgm_read_byte(&(LampSign[i]));
    } else {
      mDisplayBuffer[9 + i] &= ~pgm_read_byte(&(LampSign[i]));      
    }
  }
}

void Display::drawEcoSign()
{
  for (byte i = 0; i < 5; ++i) {
    if (m_eco) {
      mDisplayBuffer[9 + i] |= pgm_read_byte(&(EcoSign[i]));
    } else {
      mDisplayBuffer[9 + i] &= ~pgm_read_byte(&(EcoSign[i]));      
    }
  }
}

void Display::drawStopSign()
{
  for (byte i = 0; i < 8; ++i) {
    mDisplayBuffer[6 + i] = pgm_read_byte(&(StopSign[i]));
  }  
}

void Display::updateDisplayBuffer()
{
  if (!m_updateRequested) {
    return;
  }
  m_updateRequested = false;

  drawSpeed();
  drawBatteryLevel();

  if (!m_stop) {
    drawAccelerationLevel();
    drawLampSign();
    drawEcoSign();
  } else {
    drawStopSign();
  }
  
  for(byte i = 0; i < 8; ++i) {
    mLedControl.setColumn(1, i, mDisplayBuffer[i]);    
    mLedControl.setColumn(0, i, mDisplayBuffer[i + 8]);    
  }
}
