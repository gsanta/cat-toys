#include "stepper_motor_radio_controller.h"

StepperMotorRadioController::StepperMotorRadioController(MotorDriverA4988& motor, uint8_t controlDirection,
                                                             LimitSwitch* minSwitch, LimitSwitch* maxSwitch,
                                                             bool isDebug)
    : motor(motor), controlDirection(controlDirection), minSwitch(minSwitch), maxSwitch(maxSwitch), isDebug(isDebug) {
}

int StepperMotorRadioController::parseMessage(const char* message) {
    if (strncmp(message, "Move", 4) != 0) {
      return 0;
    }

    int xStage, yStage;
    if (sscanf(message, "Move X Stage: %d, Y Stage: %d", &xStage, &yStage) != 2) {
      return 0;
    }

    // Only validate the stage this handler controls
    int stage = (controlDirection == CONTROL_X) ? xStage : yStage;
    const int MIN_STAGE = -10;
    const int MAX_STAGE = 10;
    if (stage < MIN_STAGE || stage > MAX_STAGE) {
      return 0;
    }

    return stage;
  }

void StepperMotorRadioController::handleMessageReceived(const char* message) {
    int stage = parseMessage(message);
    
    bool minLimitHit = minSwitch && minSwitch->isTriggered() && stage < 0;
    bool maxLimitHit = maxSwitch && maxSwitch->isTriggered() && stage > 0;
    bool limitStopped = false;

    if (minLimitHit) {
      motor.setIsRunning(false);
      limitStopped = true;
    } else if (maxLimitHit) {
      motor.setIsRunning(false);
      limitStopped = true;
    }

    if (!limitStopped) {
      if (stage == 0) {
        motor.setIsRunning(false);
      } else {
        motor.setIsRunning(true);

        int speed = abs(stage);

        if (stage > 0) {
          motor.setDirection(UP);
        } else if (stage < 0) {
          motor.setDirection(DOWN);
        }

        motor.setSpeed(speed);
      }
    }

    // All debug messages at the bottom
    if (isDebug) {
      if (limitStopped) {
        Serial.print(minLimitHit ? "MIN" : "MAX");
        Serial.print(" limit hit - cannot move ");
        Serial.print(minLimitHit ? "backward" : "forward");
        Serial.print(" (");
        Serial.print(controlDirection == CONTROL_X ? "X" : "Y");
        Serial.println(")");
      } else if (stage != 0) {
        Serial.print("Moving ");
        Serial.println(stage);
      }
    }
  }
