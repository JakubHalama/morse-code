#include <Arduino.h>

#define LED_PIN 2
#define BUZZER_PIN 3

#define DOT_TIME 150
#define DASH_TIME 300
#define SPACE_TIME 600

struct MorseChar {
  char character;
  const char* code;
};

MorseChar morseTable[] = {
  {'A', ".-"}, {'B', "-..."}, {'C', "-.-."}, {'D', "-.."},
  {'E', "."}, {'F', "..-."}, {'G', "--."}, {'H', "...."},
  {'I', ".."}, {'J', ".---"}, {'K', "-.-"}, {'L', ".-.."},
  {'M', "--"}, {'N', "-."}, {'O', "---"}, {'P', ".--."},
  {'Q', "--.-"}, {'R', ".-."}, {'S', "..."}, {'T', "-"},
  {'U', "..-"}, {'V', "...-"}, {'W', ".--"}, {'X', "-..-"},
  {'Y', "-.--"}, {'Z', "--.."}, {'1', ".----"}, {'2', "..---"},
  {'3', "...--"}, {'4', "....-"}, {'5', "....."}, {'6', "-...."},
  {'7', "--..."}, {'8', "---.."}, {'9', "----."}, {'0', "-----"}
};

String encodeMorse(String input) {
  input.toUpperCase();
  String morse = "";
  for (char& c : input) {
    if (c == ' ') {
      morse += " / ";
    } else {
      bool found = false;
      for (MorseChar mc : morseTable) {
        if (mc.character == c) {
          morse += mc.code;
          morse += " ";
          found = true;
          break;
        }
      }
      if (!found) morse += " // ";
    }
  }
  return morse;
}

String getInput() {
  String input = "";
  while (true) {
    if (Serial.available() > 0) {
      char c = Serial.read();
      if (c == '\n') {
        break;
      } else if (c == '\b' || c == 127) {
        if (input.length() > 0) {
          input.remove(input.length() - 1);
          Serial.print("\b \b");
        }
      } else {
        input += c;
        Serial.print(c);
      }
    }
  }
  return input.trim();
}

void playMorse(const String& morse) {
  for (char c : morse) {
    if (c == '.') {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(DOT_TIME);
    } else if (c == '-') {
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);
      delay(DASH_TIME);
    } else if (c == '/') {
      delay(SPACE_TIME);
    }
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    delay(DOT_TIME);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);  
  Serial.begin(9600);
}

void loop() {
  Serial.print("Enter a string to convert it into Morse code: ");
  String input = getInput();
  Serial.println("\nEntered string: " + input);
  String morse = encodeMorse(input);
  Serial.println("Morse code: " + morse);
  playMorse(morse);
}