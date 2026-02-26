#include "config.h"
#include "wifi_manager.h"

void setup() {
  Serial.begin(9600);
  Serial.print("Start");
  init_WIFI(WIFI_START_MODE_AP);
  delay(500);
  Serial.print("Our id is:");
  Serial.println(id());
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
