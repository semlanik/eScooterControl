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
#include "speedometer.h"

#include "pinconfig.h"
#include "display.h"

/*
 * Tire diameter = 0.2159 meters
 * Tile length = pi*d = 0.6783 meters
 * Tire length in hour = 3600.0 * Tile length = 2441.88
 */

const float TireLength = 0.6783;
const float TireLengthCoef = 2441.88;

namespace {
void callback() {
  Speedometer::instance()->incrementHallCounter();
}
}

Speedometer::Speedometer() : mMomentSpeed(0)
 ,mLastHallTime(0)
 ,mHallCounter(0)
{
  pinMode(HallSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HallSensorPin), callback, RISING);
}

void Speedometer::incrementHallCounter()
{
  mHallCounter++;
  byte currentTick = mHallCounter % PulsePerCircle;
  if (currentTick == 0) {
    mHallCounter = 0;
    unsigned long currentTime = millis();
    mMomentSpeed = TireLengthCoef / (currentTime - mLastHallTime);
    mLastHallTime = currentTime;
  
    Display::instance()->setSpeed(mMomentSpeed);
  }
}
