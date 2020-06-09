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
#include "singleton.h"
#include "accelerationcontrol.h"
#include "pinconfig.h"

const int AcceleratorSensorDivider = (AcceleratorSensorMaxValue - AcceleratorSensorMinValue) * AcceleratorSensorStep;

AccelerationControl::AccelerationControl() : m_stopState(HIGH)
  , m_accelerationLevel(0)
  , m_accelerationVoltage(0)
  , m_cruiseTime(0)
  , m_cruiseLevel(0)
{
//  m_accelerator.begin(0x62);//TODO address to be setup from pinconfig;
  pinMode(AcceleratorSensorPin, INPUT);
  pinMode(StopSensorPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(StopSensorPin), [](){
    AccelerationControl::instance()->stop();
  }, CHANGE);

  m_stopState = digitalRead(StopSensorPin);
}


void AccelerationControl::setAcceleration(const int level)
{
  if (level == m_accelerationLevel) {
    if (m_cruiseLevel < m_accelerationLevel
          && m_cruiseTime < millis()) {
      m_cruiseLevel = m_accelerationLevel;
    }
    return;
  }

  if (level > m_accelerationLevel) {
    m_cruiseLevel = 0; //Reset cruise whenever accelerator level raised, when drop it means we go down
    m_cruiseTime = millis() + CruiseTime;  
  }

  m_accelerationLevel = level;
}

void AccelerationControl::dispatch(unsigned long)
{  
  if (m_stopState == HIGH) {
    setAcceleration(readAcceleratorData());
  }
  updateAccelerationVoltage();
}

void AccelerationControl::stop() {
  m_stopState = digitalRead(StopSensorPin);
  if (m_stopState == LOW) {
    m_cruiseLevel = 0; //reset cruise when stop
    setAcceleration(0);
  }
}

int AccelerationControl::readAcceleratorData()
{
  int value = analogRead(AcceleratorSensorPin);
  return round(100.0*(value - AcceleratorSensorMinValue)/AcceleratorSensorDivider);
}

void AccelerationControl::updateAccelerationVoltage() {
  int level = m_accelerationLevel;
  if (m_cruiseLevel > 0) {//At cruise control
    level = m_cruiseLevel;
  }

  int expectedVoltage = 0x0fff / AcceleratorSensorStep * level;
}
