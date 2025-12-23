#define led1 3

void setup() {
  pinMode(led1, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'u') {
      digitalWrite(led1, HIGH);
      Serial.print("u → LED_GOES_ON\n");
    }
    else if (cmd == 'd') {
      digitalWrite(led1, LOW);
      Serial.print("d → LED_GOES_OFF\n");
    }
  }
}
