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

#include "battery.h"

#include "pinconfig.h"
#include "display.h"

namespace {
constexpr int VoltageValue(double Voltage, double R1, double R2) {
  return static_cast<int>(Voltage * ( R2 / (R1 + R2)) / (5.0 / 1023.0));
}

constexpr double R1 = 1000000; //1 MOhm
constexpr double R2 = 100000; //100 KOhm
constexpr int VoltageMin = VoltageValue(36.0, R1, R2); //669
constexpr int VoltageMax = VoltageValue(42.0, R1, R2) - VoltageMin; //778 - 669
constexpr int Step = (VoltageMax - VoltageMin) / 8;
}

BatterySensor::BatterySensor()
{
}

void BatterySensor::dispatch()
{
  int sensorValue = analogRead(BatteryPin) - VoltageMin;
  int batteryLevel = sensorValue/Step;
  Display::instance()->setBatteryLevel(batteryLevel);
}
