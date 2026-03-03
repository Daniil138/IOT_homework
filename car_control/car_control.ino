
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

void forward(int speed){
  move(FORWARD, speed, FORWARD, speed );

}

void backword(int speed){
  move(BACK, speed, BACK, speed );

}

void tern_left(int steepness){
  move(FORWARD, steepness, FORWARD, 255);


}

void tern_right(int steepness){
  move(FORWARD,255 , FORWARD, steepness);

}

void rotate_righ(int speed){
  move(FORWARD, speed, BACK, speed );
}

void rotate_left(int speed){
  move(BACK, speed, FORWARD, speed );
}

void setup() {
  pinMode(SPEED_RIGHT,OUTPUT);
  pinMode(DIR_RIGHT,OUTPUT);
  pinMode(SPEED_LEFT,OUTPUT);  
  pinMode(DIR_LEFT,OUTPUT);  



}

void loop() {
  forward(200);
  delay(1000);
  backword(200);
  delay(1000);
  tern_left(50);
  delay(1000);
  tern_right(50);
  delay(1000);
  rotate_left(150);
  delay(1000);
  rotate_righ(150);
  delay(1000);

  

}
