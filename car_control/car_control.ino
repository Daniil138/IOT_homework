
#define SPEED_RIGHT 5
#define DIR_RIGHT 4

#define SPEED_LEFT 6
#define DIR_LEFT 7

void setup() {
  pinMode(SPEED_1,OUTPUT);
  pinMode(DIR_1,OUTPUT);
  pinMode(SPEED_2,OUTPUT);  
  pinMode(DIR_2,OUTPUT);  
  digitalWrite(DIR_1, LOW);
  digitalWrite(DIR_2, HIGH);
  digitalWrite(SPEED_1, LOW);
  digitalWrite(SPEED_2, HIGH);

}

void loop() {

}
