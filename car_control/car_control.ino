#define SPEED_RIGHT 5
#define DIR_RIGHT 4

#define SPEED_LEFT 6
#define DIR_LEFT 7

#define FORWARD  HIGH
#define BACK  LOW

#define TRIG_FRONT 3
#define ECHO_FRONT 2
#define SENSOR_LEFT A0

#define WALLCLOSE 5
#define WALLFAR 15
#define DELTA 20

#define HISTORY_SIZE 256

int historyDF[HISTORY_SIZE];
int historyDL[HISTORY_SIZE];

#define FORWARD_STATE 1
#define ROTATE_RIGHT_STATE 2
#define TURN_LEFT_STATE 3
#define TURN_RIGHT_STATE 4
#define RETURN_LEFT_STATE 5

int globalState = FORWARD_STATE;


// Additional funcs
void addValue(int* arr, int size, int value) {
  memmove(&arr[1], &arr[0], (size - 1) * sizeof(int));
  arr[0] = value;
}

long getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000);
  return duration / 58;
}

int getIRDistance(int sensor_pin) {
    int sensor_val = analogRead(sensor_pin);
    if (sensor_val < 80) return 30;
    if (sensor_val > 530) return 4;
    return 2076.0 / (analogRead(sensor_pin) - 11);
}

void move(bool left_dir, int left_speed, bool right_dir, int right_speed){
  digitalWrite(DIR_RIGHT, right_dir);
  digitalWrite(DIR_LEFT, left_dir);
  analogWrite(SPEED_RIGHT, right_speed);
  analogWrite(SPEED_LEFT, left_speed);
}


// Short-funcs
void forward(int speed) {move(FORWARD, speed, FORWARD, speed );}
void backword(int speed) {move(BACK, speed, BACK, speed );}
void turn_left(int steepness) {move(FORWARD, steepness, FORWARD, 255);}
void turn_right(int steepness) {move(FORWARD,255 , FORWARD, steepness);}
void rotate_righ(int speed) {move(FORWARD, speed, BACK, speed );}
void rotate_left(int speed) {move(BACK, speed, FORWARD, speed );}
void addDFHistory(int value) { addValue(historyDF, HISTORY_SIZE, value); }
void addDLHistory(int value) { addValue(historyDL, HISTORY_SIZE, value); }
void updateDFHistory() { addDFHistory(getDistance(TRIG_FRONT, ECHO_FRONT)); }
void updateDLHistory() { addDLHistory(getIRDistance(SENSOR_LEFT)); }



void setup() {
  pinMode(SPEED_RIGHT,OUTPUT);
  pinMode(DIR_RIGHT,OUTPUT);
  pinMode(SPEED_LEFT,OUTPUT);  
  pinMode(DIR_LEFT,OUTPUT);
  
  // Инициализация ультразвуковых датчиков
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  
  // Инициализация инфракрасного датчика
  pinMode(SENSOR_LEFT, INPUT);
}


void loop() {
    switch (globalState) {
        case FORWARD_STATE: 
            forward(100);
            if (historyDF[0] < WALLCLOSE){
              globalState = ROTATE_RIGHT_STATE;
              break;
            }
            if (historyDL[0] < WALLCLOSE){
              globalState = TURN_RIGHT_STATE;
              break;
            }
            if (historyDL[0] > WALLFAR){
              globalState = TURN_LEFT_STATE;
              break;
            }
        case ROTATE_RIGHT_STATE:
            rotate_righ(100);
            if (historyDL[0] > historyDL[1] 
                && historyDL[1] <= historyDF[2]) {
                globalState = RETURN_LEFT_STATE;
                }
            break;
        case RETURN_LEFT_STATE:
            rotate_left(50);
            if (historyDL[0] < historyDL[1]) {
                globalState = FORWARD_STATE;
            }
            break;
        case TURN_RIGHT_STATE: 
            turn_right(100);
            if (historyDL[0] > WALLCLOSE) {
                globalState = FORWARD_STATE;
            }
            break;
        case TURN_LEFT_STATE:
            turn_left(100);
            if (historyDL[0] <= WALLCLOSE) {
                globalState = FORWARD_STATE;
            }
            break;
    }
    updateDFHistory();
    updateDLHistory();
}
