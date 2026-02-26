#include "config.h"
#include "wifi_manager.h"
#include "server.h"
#include "MQTT.h"

void setup() {
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  Serial.print("Start");
  init_WIFI(WIFI_START_MODE_CLIENT);
  delay(500);
  Serial.print("Our id is:");
  Serial.println(id());
  init_server();
  init_MQTT();
  mqtt_cli.subscribe("")
  // put your setup code here, to run once:

}

void loop() {
  server.hendleClient();
  mqtt_cli.loop()
  // put your main code here, to run repeatedly:

}
