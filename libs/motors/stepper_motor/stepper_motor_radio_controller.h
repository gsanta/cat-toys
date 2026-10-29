#ifndef STEPPER_MOTOR_RADIO_CONTROLLER_H
#define STEPPER_MOTOR_RADIO_CONTROLLER_H

#include "../../input/joystick/staged_joystick_reader.h"
#include "../../system/timer/timer_handler.h"
#include "../../communication/radio_transmitter/rf24_radio_transmitter.h"
#include "../../motors/stepper_motor/motor_driver_a4988.h"
#include "../../communication/radio_transmitter/radio_read_command_handler.h"
#include "../../logic/limit_switch/limit_switch.h"

#define CONTROL_X 1
#define CONTROL_Y 2

class StepperMotorRadioController : public RadioReadCommandHandler {
public:
  StepperMotorRadioController(MotorDriverA4988& motor, uint8_t controlDirection,
                               LimitSwitch* minSwitch = nullptr, LimitSwitch* maxSwitch = nullptr,
                               bool isDebug = false);

  void handleMessageReceived(const char* message) override;

private:
  int parseMessage(const char* message);
  
  MotorDriverA4988& motor;  
  LimitSwitch* minSwitch;
  LimitSwitch* maxSwitch;
  uint8_t controlDirection;
  int maxSpeed = 3;
  bool isDebug;
};

#endif
