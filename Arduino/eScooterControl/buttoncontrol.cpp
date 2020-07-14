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

#include "buttoncontrol.h"
#include "pinconfig.h"
#include "display.h"

const unsigned long ReactionTime = 700;
const unsigned long LongReactionTime = 500;

/*
 * Do not use interruption here because film buttons have false positives in case of bad contact. 
 * It's better to dispatch them frequently and count positives manually by digitalRead
 */
ButtonControl::ButtonControl() : m_triggers(0)
   ,m_triggerTime(0)
   ,m_previousState(HIGH)
   ,m_ledState(LOW)
{
  pinMode(LedPin, OUTPUT);
  pinMode(PowerPin, OUTPUT);
  pinMode(ButtonPin, INPUT_PULLUP);
  digitalWrite(PowerPin, HIGH);
}

void ButtonControl::dispatch() {
  unsigned long currentTime = millis();
  bool state = digitalRead(ButtonPin);
  if (m_previousState == HIGH && state == LOW) {
    if (m_triggers == 0) {
      m_triggerTime = currentTime;
    }
    ++m_triggers;
  }

  unsigned long timeDiff = currentTime - m_triggerTime;
  m_previousState = state;
  if (state == LOW && timeDiff >= LongReactionTime && m_triggers == 1) {
//      Serial.println("Power off");
    //TODO: Prohibit switch off if speed > 0 || throttle is no at 0
    digitalWrite(PowerPin, LOW);    
  } else if (timeDiff >= ReactionTime && state == HIGH) {
    if (m_triggers == 1) {
//      Serial.println("Toggle led");
      toggleLedState();
    } else if (m_triggers == 2) {
//      Serial.println("Toggle eco");
      //TODO: Enable eco mode
    }
    //Action finished reset number of triggers
    m_triggers = 0;
  }
}

void ButtonControl::toggleLedState()
{
  if (m_ledState == HIGH) {
    m_ledState = LOW;
  } else {
    m_ledState = HIGH;
  }

  Display::instance()->setLightOn(m_ledState);

  digitalWrite(LedPin, m_ledState);
}
