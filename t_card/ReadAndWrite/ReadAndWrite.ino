#include <SPI.h>

#include <MFRC522.h>

#include "OperatorCommad.h"

#define RST_PIN 9
#define SS_PIN 10

OperatorCommad operatorCommad;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(5);
  while (!Serial) {
  }
  SPI.begin();
  operatorCommad.init();
  // Serial.println(F("T-Card Run."));
}

void loop() { operatorCommad.executeCommand(); }
