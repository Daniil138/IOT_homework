#define SENSOR_PIN A5

bool isStreaming = false;

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
}

void loop() {
  if (Serial.available() > 0) {
    char incomingByte = Serial.read();
    
    switch (incomingByte) {
      case 'p':
        isStreaming = false;
        break;
      case 's':
        isStreaming = true;
        Serial.println("Stream started!");
        break;
    }
  }
  
  if (isStreaming) {
    Serial.println(analogRead(SENSOR_PIN));
    delay(200);
  }
}
