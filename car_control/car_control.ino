
#define SPEED_RIGHT 5
#define DIR_RIGHT 4

#define SPEED_LEFT 6
#define DIR_LEFT 7


#define FORWARD  HIGH
#define BACK  LOW


void move(bool left_dir, int left_speed, bool right_dir, int right_speed){
  digitalWrite(DIR_RIGHT, right_dir);
  digitalWrite(DIR_LEFT, left_dir);

  analogWrite(SPEED_RIGHT, right_speed);
  analogWrite(SPEED_LEFT, left_speed);

}

void setup() {
  pinMode(SPEED_RIGHT,OUTPUT);
  pinMode(DIR_RIGHT,OUTPUT);
  pinMode(SPEED_LEFT,OUTPUT);  
  pinMode(DIR_LEFT,OUTPUT);  

}

void loop() {
move(BACK, 100, FORWARD, 100 );

  

}
