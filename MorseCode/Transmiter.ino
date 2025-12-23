const int out_pin = 2;

const char* morseCodes[36] = {
    "01", "1000", "1010", "100", "0", "0010", "110", "0000", "00", "0111",
    "101", "0100", "11", "10", "111", "0110", "1101", "010", "000", "1",
    "001", "0001", "011", "1001", "1011", "1100",
    "11111", "01111", "00111", "00011", "00001", "00000", "10000", "11000", "11100", "11110"
};

void sendMorseBit(bool isDash) {
    digitalWrite(out_pin, HIGH);
    delay(isDash ? 300 : 100);
    digitalWrite(out_pin, LOW);
    delay(100);
}

void sendMorseChar(char c) {
    int index = -1;
    if (c >= 'A' && c <= 'Z') {
        index = c - 'A';
    } else if (c >= '0' && c <= '9') {
        index = 26 + (c - '0');
    }
    if (index == -1) {
        Serial.print("(skip:'");
        Serial.print(c);
        Serial.print("')");
        return;
    }

    const char* code = morseCodes[index];
    Serial.print("[");
    Serial.print(c);
    Serial.print(":");
    Serial.print(code);
    Serial.print("] ");
    
    for (int i = 0; code[i] != '\0'; i++) {
        sendMorseBit(code[i] == '1');
        if (code[i + 1] != '\0') delay(100);
    }
}

void sendMorseWord(const char* word) {
    Serial.print("Sending word: ");
    Serial.println(word);
    
    for (int i = 0; word[i] != '\0'; i++) {
        char c = word[i];
        if (c >= 'a' && c <= 'z') c -= 32;
        sendMorseChar(c);
        delay(300);
    }
    
    Serial.println();
    Serial.println("Word sent.");
}

void setup() {
    pinMode(out_pin, OUTPUT);
    digitalWrite(out_pin, LOW);
    Serial.begin(9600);
    Serial.println("Morse sender ready. Send a word via Serial.");
}

void loop() {
    if (Serial.available()) {
        String word = Serial.readStringUntil('\n');
        word.trim();
        
        if (word.length() > 0) {
            Serial.print("Received: '");
            Serial.print(word);
            Serial.println("'");
            sendMorseWord(word.c_str());
            delay(1000);
        } else {
            Serial.println("Empty input.");
        }
    }
}
