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
#include "thread.h"
#include "display.h"
#include "speedometer.h"
#include "accelerationcontrol.h"
#include "buttoncontrol.h"

const unsigned long AcceleratorPedalUpdateTime = 200;
const unsigned long AcceleratorUpdateTime = 10;
const unsigned long DisplayUpdateTime = 250;
const unsigned long BatteryUpdateTime = 1000;
const unsigned long ButtonHandlingTime = 50;

Thread gDisplayThread = Thread(DisplayUpdateTime);
Thread gBatteryThread = Thread(BatteryUpdateTime);
Thread gAcceleratorPedalThread = Thread(AcceleratorPedalUpdateTime);
Thread gAcceleratorThread = Thread(AcceleratorUpdateTime);
Thread gButtonThread = Thread(ButtonHandlingTime);

unsigned char fakeBatteryLevel = 0;

void setup() {
//  Serial.begin(115200);
//  Serial.println("Init");

  delay(1000);
  AccelerationControl::instance();
  gAcceleratorPedalThread.assignCallback([](){
    AccelerationControl::instance()->dispatch();
  });

  gAcceleratorThread.assignCallback([](){
    AccelerationControl::instance()->dispatchAcceleration();
  });

  delay(500);
  Speedometer::instance()->attachToDisplay(Display::instance());

  delay(500);
  Display::instance();
  gDisplayThread.assignCallback([](){
    Display::instance()->updateDisplayBuffer();
  });

  gBatteryThread.assignCallback([](){
    Display::instance()->setBatteryLevel(fakeBatteryLevel);
    fakeBatteryLevel++;
    if (fakeBatteryLevel > 6) {
      fakeBatteryLevel = 0;
    }
  });

  ButtonControl::instance();
  gButtonThread.assignCallback([](){
    ButtonControl::instance()->dispatch();
  });
//  Serial.println("Init complete");
}

void loop() {
  Thread::dispatch();
}
