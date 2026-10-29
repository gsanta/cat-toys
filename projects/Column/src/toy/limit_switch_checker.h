#ifndef LIMIT_SWITCH_CHECKER_H
#define LIMIT_SWITCH_CHECKER_H

#include <Arduino.h>
#include "../../lib/system/timer/timer_handler.h"
#include "../../lib/system/timer/timer_control.h"
#include "../../lib/logic/limit_switch/limit_switch.h"

class LimitSwitchChecker : public TimerHandler {
public:
  LimitSwitchChecker(LimitSwitch& limitSwitch1, LimitSwitch& limitSwitch2, TimerControl& timerControl, unsigned long intervalMs = 50)
    : TimerHandler(intervalMs), timerControl(timerControl), 
      limitSwitch1(limitSwitch1), limitSwitch2(limitSwitch2) {}

  void setup() {
    timerControl.add_millisecond_handler(this);
  }

  void handleTimerEvent() override {
    limitSwitch1.checkTriggered();
    limitSwitch2.checkTriggered();
  }

private:
  TimerControl& timerControl;
  LimitSwitch& limitSwitch1;
  LimitSwitch& limitSwitch2;
};

#endif
