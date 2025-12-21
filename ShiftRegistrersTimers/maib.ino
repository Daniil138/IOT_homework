// Пины для 74HC595
#define CS_595   6   
#define DAT_595  3   
#define CLK_595  5   

const uint8_t segmentRight[10] = {
  0b00011000, // 0
  0b11011110, // 1
  0b00110100, // 2
  0b10010100, // 3
  0b11010010, // 4
  0b10010001, // 5
  0b00010001, // 6
  0b11011100, // 7
  0b00010000, // 8
  0b10010000  // 9
};
const uint8_t segmentLeft[10] = {
  0b00000011, // 0
  0b11011011, // 1
  0b10010010, // 2
  0b10000110, // 3
  0b01011010, // 4
  0b00110010, // 5
  0b00100010, // 6
  0b10011011, // 7
  0b00000010, // 8
  0b00010010  // 9
};

volatile uint8_t seconds = 0;
volatile bool updateDisplay = true;


static inline void fastShiftOut(uint8_t data) {

  const uint8_t clk_mask = _BV(CLK_595);
  const uint8_t dat_mask = _BV(DAT_595);
  volatile uint8_t *clk_port = portOutputRegister(digitalPinToPort(CLK_595));
  volatile uint8_t *dat_port = portOutputRegister(digitalPinToPort(DAT_595));

  for (uint8_t i = 0; i < 8; i++) {
    if (data & 0x80) {
      *dat_port |= dat_mask;
    } else {
      *dat_port &= ~dat_mask;
    }
    data <<= 1;
    *clk_port |= clk_mask;   
    *clk_port &= ~clk_mask; 
  }
}

static inline void latchHigh(void) {
  *portOutputRegister(digitalPinToPort(CS_595)) |= _BV(CS_595);
}
static inline void latchLow(void) {
  *portOutputRegister(digitalPinToPort(CS_595)) &= ~_BV(CS_595);
}

void setup() {

  pinMode(CS_595, OUTPUT);
  pinMode(DAT_595, OUTPUT);
  pinMode(CLK_595, OUTPUT);

  Serial.begin(9600);
  Serial.println("Отправьте начальное значение (00–99):");

  cli();
  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC + 1024
  OCR1A = 15624; 
  TIMSK1 = (1 << OCIE1A);
  sei();
}

ISR(TIMER1_COMPA_vect) {
  seconds++;
  if (seconds > 99) seconds = 0;
  updateDisplay = true;
}

void loop() {

  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      long num = input.toInt();
      if (num >= 0 && num <= 99) {
        seconds = (uint8_t)num;
        updateDisplay = true;
        Serial.print("Установлено: ");
        Serial.println(seconds);
      } else {
        Serial.println("Ошибка: введите число от 0 до 99");
      }
    }
  }


  if (updateDisplay) {
    uint8_t sec_copy = seconds;
    updateDisplay = false;

    latchLow();
    fastShiftOut(segmentRight[sec_copy % 10]);
    fastShiftOut(segmentLeft[sec_copy / 10]);
    latchHigh();
  }
}
