#include <Arduino.h>
#include "rf24_radio_transmitter_manager.h"

RF24RadioTransmitterManager::RF24RadioTransmitterManager(uint8_t ce, uint8_t csn, const byte* addr, TimerControl& timerControl, bool isDebug) : radio(ce, csn, addr, isDebug), timerControl(timerControl), timerListener(*this) {}

void RF24RadioTransmitterManager::setup(bool read) {
  radio.setup(read);
  timerControl.add_millisecond_handler(&timerListener);
}

void RF24RadioTransmitterManager::addCommandHandler(RadioReadCommandHandler *handler) {
  if (handlerCount < MAX_COMMAND_HANDLERS) {
      handlers[handlerCount++] = handler;
  }
}

void RF24RadioTransmitterManager::update() {
  if (!radio.read()) {
    return;
  }

  for (int i = 0; i < handlerCount; ++i) {
    handlers[i]->handleMessageReceived(radio.getReceivedText());
  }

  radio.clearReceivedText();
}