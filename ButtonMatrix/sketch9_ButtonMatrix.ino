#define p_pin2 2
#define p_pin3 3
#define led_pin8 8
#define led_pin9 9

void setup() {
  pinMode(p_pin2, INPUT_PULLUP);
  pinMode(p_pin3, INPUT_PULLUP);
  pinMode(led_pin9, OUTPUT);
  pinMode(led_pin8, OUTPUT);
  Serial.begin(9600);

  digitalWrite(led_pin8, HIGH);
	digitalWrite(led_pin9, LOW);
}

void loop() {
  int number = 1;
  for (int i=0; i<2; i++){
    for (int j=2; j<4; j++){
      if (digitalRead(j) == LOW){
        Serial.println((String)"Button number "+number+" is pressed");
      }
      number += 1;
      delay(100);
    }
    change_state();
  }
}

void change_state(){
  digitalWrite(led_pin8, !digitalRead(led_pin8));
	digitalWrite(led_pin9, !digitalRead(led_pin9));
}