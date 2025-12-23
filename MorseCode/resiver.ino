#include <avr/io.h>
#include <string.h>

const int dataPin  = 8;
const int clockPin = 10;
const int latchPin = 9;
const int serv = 11;
const int in_pin = 3;
const int out_pin = 2;

const int dot = 1;
const int space_dd = 1;
const int space_char = 3;
const int space_word = 7;

const char* morseCodes[36] = {
    "01", "1000", "1010", "100", "0", "0010", "110", "0000", "00", "0111",
    "101", "0100", "11", "10", "111", "0110", "1101", "010", "000", "1",
    "001", "0001", "011", "1001", "1011", "1100",
    "11111", "01111", "00111", "00011", "00001", "00000", "10000", "11000", "11100", "11110"
};

const uint8_t alphabet[36] = {
    0b01100000, 0b01001100, 0b11000101, 0b01001010, 0b11000100, 0b11100100,
    0b01010000, 0b01101000, 0b00111011, 0b00011011, 0b00100101, 0b11001101,
    0b10000100, 0b11101010, 0b01000001, 0b11100000, 0b01110000, 0b00100000,
    0b01010100, 0b11101100, 0b01001001, 0b01011111, 0b01011010, 0b00101000,
    0b01011000, 0b11000010,
    0b01000001, 0b01111011, 0b11000010, 0b01010010, 0b01111000, 0b01010100,
    0b01000100, 0b01110011, 0b01000000, 0b01010000
};

#define IN_PIN_PORT PINB
#define IN_PIN_BIT  3

const int MAX_PULSES = 100;
const int MAX_LOW_COUNT = 50;

volatile int pulse_buffer[MAX_PULSES];
volatile int pulse_index = 0;
volatile int current_count = 0;
volatile int state = 0;
volatile bool capture_done = false;
volatile bool message_ready = false;

void show_letters(char c) {
  if (c >= 'A' && c <= 'Z') {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, alphabet[c - 'A']);
    digitalWrite(latchPin, HIGH);
  }
  if (c >= '0' && c <= '9') {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, alphabet[26 + (c - '0')]);
    digitalWrite(latchPin, HIGH);
  }
}

int norm(int x) {
  return (x + 5) / 10;
}

void setup() {
  pinMode(in_pin, INPUT);
  pinMode(out_pin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  cli();
  TCCR1A = 0;
  TCCR1B = (1 << CS12);
  OCR1A = 624;
  TIMSK1 = (1 << OCIE1A);
  sei();
}

ISR(TIMER1_COMPA_vect) {
  bool pin_high = (IN_PIN_PORT & (1 << IN_PIN_BIT)) != 0;

  if (state == 0) {
    if (pin_high) {
      state = 1;
      current_count = 1;
    }
    return;
  }

  if (state == 1) {
    if (pin_high) {
      current_count++;
    } else {
      if (pulse_index < MAX_PULSES) {
        pulse_buffer[pulse_index++] = current_count;
      }
      state = 2;
      current_count = 1;
    }
  } else if (state == 2) {
    if (!pin_high) {
      current_count++;
      if (current_count >= MAX_LOW_COUNT) {
        capture_done = true;
        state = 0;
        message_ready = true;
        return;
      }
    } else {
      if (pulse_index < MAX_PULSES) {
        pulse_buffer[pulse_index++] = current_count;
      }
      state = 1;
      current_count = 1;
    }
  }
}

void loop() {
  if (message_ready) {
    int norm_pulses[MAX_PULSES];
    int n = pulse_index;
    for (int i = 0; i < n; i++) {
      norm_pulses[i] = norm(pulse_buffer[i]);
    }

    int start = 0;
    for (int i = 0; i <= n; i++) {
      if (i == n || (i % 2 == 1 && norm_pulses[i] >= 3)) {
        if (start < i) {
          char morse[50];
          int m_len = 0;
          for (int j = start; j < i; j += 2) {
            int len = norm_pulses[j];
            char sym = (len >= 2) ? '1' : '0';
            morse[m_len++] = sym;
          }
          morse[m_len] = '\0';

          char decoded = 0;
          for (int k = 0; k < 36; k++) {
            if (strcmp(morse, morseCodes[k]) == 0) {
              decoded = (k < 26) ? ('A' + k) : ('0' + (k - 26));
              break;
            }
          }

          if (decoded) {
            show_letters(decoded);
            delay(500);
          }
        }
        start = i + 1;
      }
    }

    pulse_index = 0;
    message_ready = false;
    capture_done = false;
  }
}
