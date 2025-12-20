volatile unsigned long int seconds=0;
void setup() {
  DDRB = DDRB | 0b00111110;
  PORTB |= 0b00111110;
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

  PINB = (1 << PINB1);
  if (seconds%2==0){
    PINB = (1 << PINB2);
  }
  if (seconds%3==0){
    PINB = (1 << PINB3);
  }
  if (seconds%4==0){
    PINB = (1 << PINB4);
  }
  if (seconds%5==0){
    PINB = (1 << PINB5);
  }
  
  

}

void loop() {

}
