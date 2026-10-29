#include "rf24_radio_transmitter.h"
#include <string.h>

RF24RadioTransmitter::RF24RadioTransmitter(uint8_t ce, uint8_t csn, const byte* addr, bool isDebug)
  : radio(ce, csn), cePin(ce), csnPin(csn), isDebug(isDebug), isInitialized(false) {
  memcpy(address, addr, 5);
  address[5] = 0;
  receivedText[0] = '\0';
}

RF24RadioTransmitter::~RF24RadioTransmitter() {
  // Destructor implementation
}

void RF24RadioTransmitter::setup(bool read) {
  delay(500);

    Serial.println("[RF24] Attempting to initialize...");
  
  // Add extra delays for Arduino clones with timing issues
  delay(100);
  
  // Try with retries
  int retries = 0;
  while (retries < 3 && !isInitialized) {
    isInitialized = radio.begin();
    if (isInitialized) {
      break;
    }

    Serial.print("[RF24] Init failed (attempt ");
    Serial.print(retries + 1);
    Serial.println("), retrying...");
    delay(100);
    radio.powerDown();
    delay(100);
    retries++;
  }

  if (!isInitialized) {
    Serial.println("[RF24] Init failed after 3 attempts - possible hardware issue");
    Serial.println("[RF24] For new Arduino clones, check:");
    Serial.println("  - Oscillator frequency (should be 16MHz)");
    Serial.println("  - RF24 power supply (add 10uF capacitor near VCC)");
    return;
  }
  
  Serial.println("[RF24] Init success");
  
  // Set channel explicitly to match transmitter
  radio.setChannel(76);
  radio.setDataRate(RF24_1MBPS);
  radio.setPayloadSize(32);
  radio.setCRCLength(RF24_CRC_16);
  radio.setAutoAck(true);
  radio.setRetries(5, 15);

  if (read) {
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
    Serial.println("[RF24] Setup as RECEIVER");
  } else {
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
    Serial.println("[RF24] Setup as TRANSMITTER");
  }
  
  Serial.print("[RF24] Channel: ");
  Serial.println(radio.getChannel());
  Serial.print("[RF24] DataRate: ");
  Serial.println(radio.getDataRate());
}

bool RF24RadioTransmitter::read() {
  if (!isInitialized) {
    return false;
  }

  if (radio.available()) {
    radio.read(&receivedText, 32);
    receivedText[32] = '\0';

    if (isDebug) {
      Serial.print("[RF24] Received: ");
      Serial.println(receivedText);
    }

    return true;
  } else {
    if (isDebug) {
      Serial.println("[RF24] No data available");
    }
  }
  return false;
}

void RF24RadioTransmitter::write(const char* text, size_t length) {
  // Pad to exactly 32 bytes
  // char buffer[32] = {0};
  // memcpy(buffer, text, (length > 32) ? 32 : length);
  
  // Serial.print("[RF24] Sending: ");
  // Serial.println(text);
  // radio.write(buffer, 32);
  if (!isInitialized) {
    if (isDebug) {
      Serial.println("[RF24] Not sent: radio is not initialized");
    }
    return;
  }

  const bool sent = radio.write(text, length);
  if (isDebug) {
    Serial.print(sent ? "[RF24] Sent: " : "[RF24] Send failed: ");
    Serial.println(text);
  }
}

const char* RF24RadioTransmitter::getReceivedText() {
  return receivedText;
}

const void RF24RadioTransmitter::clearReceivedText() {
  receivedText[0] = '\0';
}
