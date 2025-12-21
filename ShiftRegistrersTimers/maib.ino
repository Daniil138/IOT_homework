#define CS_595 6
#define DAT_595 3
#define CLK_595 5
volatile unsigned long int seconds=0;

const uint8_t segmentRight[10] = {
  0b00011000, // 0 → a
  0b11011110, // 1 → b
  0b00110100, // 2 → c
  0b10010100, // 3 → d
  0b11010010, // 4 → e
  0b10010001, // 5 → f
  0b00010001, // 6 → g
  0b11011100,
  0b00010000,
  0b10010000
};
const uint8_t segmentLeft[10] = {
  0b00000011, // 0 → a
  0b11011011, // 1 → b
  0b10010010, // 2 → c
  0b10000110, // 3 → d
  0b01011010, // 4 → e
  0b00110010, // 5 → f
  0b00100010, // 6 → g
  0b10011011,
  0b00000010,
  0b00010010
};
void setup() {
  pinMode(CS_595, OUTPUT);
  pinMode(DAT_595, OUTPUT);
  pinMode(CLK_595, OUTPUT);
  pinMode(12, INPUT);
  Serial.begin(9600);
  Serial.println("Отправьте секунду с которой продолжить до 100:");
 
  cli(); 
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B = (1 << CS12) | (1 << CS10) | (1 << WGM12); 
  OCR1A = 15624; 
  TIMSK1 = (1 << OCIE1A); 
  
  sei();
}
ISR(TIMER1_COMPA_vect) {
  seconds++;
  if (seconds >=100){
    seconds=0;
    };

}
void loop() {
  int value = digitalRead(12);
  if (value==0){
    seconds=0;
    delay(200);
  };

  digitalWrite(CS_595, LOW);
  shiftOut(DAT_595, CLK_595, MSBFIRST, segmentRight[seconds%10]);
  shiftOut(DAT_595, CLK_595, MSBFIRST, segmentLeft[seconds/10]);
  digitalWrite(CS_595, HIGH);

 if (Serial.available() > 0) {
  String input = Serial.readStringUntil('\n');
  input.trim();  

  if (input.length() > 0) {
    long number = input.toInt();

    if (number >= 0 && number <= 99) {
      seconds = number;
      Serial.print("Установлено: ");
      Serial.println(seconds);
    } else {
      Serial.println("Ошибка: введите число от 0 до 99");
    }
  }
}}
