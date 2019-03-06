/*
 TSC MoodBoard
*/

// constants won't change. Used here to set a pin number:
const int ledGreen =  4; 
const int ledYellow =  3;
const int ledRed =  15;

const int buttonGreen = 34;
const int buttonYellow = 32;
const int buttonRed = 35;

void setup() {
  Serial.begin(112500);
  initMood(buttonGreen, ledGreen);
  initMood(buttonYellow, ledYellow);
  initMood(buttonRed, ledRed);  
}

void loop() {
  mach(buttonGreen, ledGreen,1);
  mach(buttonYellow, ledYellow,0);
  mach(buttonRed, ledRed,-1);  
}

void initMood(int button, int led){
  pinMode(led, OUTPUT);
  pinMode(button,INPUT);
}

void mach(int button, int led, int value){
  int pressed = digitalRead(button);
  if (pressed) {
    digitalWrite(led, HIGH);
    Serial.print("{");
    Serial.print("'button': ");
    Serial.print(button);
    Serial.print(", 'led': ");
    Serial.print(led);
    Serial.print(", 'value': ");
    Serial.print(value);
    Serial.println("}");
    delay(500);
  } else {
    digitalWrite(led, LOW);
  }
}
