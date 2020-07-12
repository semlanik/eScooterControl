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
#pragma once

#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


//Two pins used by Wire library for I2C bus
//Are not used in code directly. Keep here as reminder.
const int I2CSDAPin = A4;
const int I2CSCLPin = A5;

const int ButtonPin = 4;
const int PowerPin = 5;

const int LedPin = 6;

const int HallSensorPin = 2;
const byte PulsePerCircle = 45;

const int StopSensorPin = 3;

const int DisplayDataPin = 10;
const int DisplayCsPin = 11;
const int DisplayClkPin = 12;

const int AcceleratorSensorPin = A0;

//Align accelerator trigger values
const unsigned int AcceleratorSensorDiff = 548;

const unsigned long CruiseTime = 3000; //ms

const uint8_t AcceleratorAddress = 0x60;
