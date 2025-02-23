#include <Arduino.h>

#include "Atp301x_Arduino_SPI.h"


ATP301x_ARDUINO_SPI atp301x;

void setup() {
  atp301x.begin();
}

void loop() {
  
  atp301x.chimeK(true);
  delay(1000);

  atp301x.talk("te'_suto.", true);
  delay(1000);

  atp301x.chimeJ(true);
  delay(1000);

  atp301x.talk("chu-dannte'_suto tomaranai+hazu.", true);
  delay(500);
  atp301x.stop();
  delay(1000);

  atp301x.talk("chu-dannte'_suto tomaru+hazu.", false);
  delay(500);
  atp301x.stop();
  delay(1000);

  atp301x.talk("nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse- nagasugi'ru/o'nnse-.", false);
  delay(500);
  atp301x.stop();
  delay(1000);

}

