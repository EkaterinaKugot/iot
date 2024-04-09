// 4, 7 for direction
// 5, 6 for speed

#define DIR_RIGHT 4//false - forward, true - back
#define DIR_LEFT 7//false - forward, true - back
#define SPEED_RIGHT 5
#define SPEED_LEFT 6

#define F_TRIG 2
#define F_ECHO 3
#define L_TRIG 11
#define L_ECHO 12

#define FD 25
#define CD 10
#define JD 15

enum State {
  FORWARD,
  DETECT_TURN_OR_ROTATE,
  TURN_RIGHT,
  TURN_LEFT,
  ROTATE_RIGHT,
  ROTATE_LEFT,
};

State state = FORWARD;

int counter = 0;
int ld_prev=0;
int time_rotate = 600;
int time_jogging = 300;

void setup() {
  for(int i = 0; i<=7; i++)
  {
      pinMode(i, OUTPUT);
  }
  pinMode(F_TRIG, OUTPUT);
  pinMode(L_TRIG, OUTPUT);
  pinMode(F_ECHO, INPUT);
  pinMode(L_ECHO, INPUT);
  int counter = millis() + 1250;
  stop();
  Serial.begin(9600);
}

void move(bool lforward, bool rforward, int lvelocity, int rvelocity){
  digitalWrite(DIR_RIGHT, rforward);
  digitalWrite(DIR_LEFT, lforward);
  analogWrite(SPEED_RIGHT, rvelocity);
  analogWrite(SPEED_LEFT, lvelocity);
}

void move_forward(int velocity){
  move(true, true, velocity, velocity);
}

void move_back(int velocity){
  move(false, false, velocity, velocity);
}

void rotate_left(int velocity){
  move(false, true, velocity, velocity);
}

void rotate_right(int velocity){
  move(true, false, velocity, velocity);
}

void turn_right(int velocity){
  move(true, true, velocity, velocity/2);
}
void turn_left(int velocity){
  move(true, true, velocity/2, velocity);
}

void stop(){
  move(true, true, 0, 0);
}

long get_distance(int pin_trig, int pin_echo)
{
  long duration, cm;
  digitalWrite(pin_trig, LOW);
  delayMicroseconds(5);
  digitalWrite(pin_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin_trig, LOW);

  duration = pulseIn(pin_echo, HIGH);
  cm = (duration / 2) / 29.1;
  return cm;
}  

void loop() {
  int ld = get_distance(L_TRIG, L_ECHO);
  int fd = get_distance(F_TRIG, F_ECHO);
  int time = millis();

  switch (state) {
    case FORWARD:
      move_forward(255);
      state = DETECT_TURN_OR_ROTATE;
      break;
      
    case DETECT_TURN_OR_ROTATE:
      if (ld < CD) {
        state = TURN_RIGHT;
      } else if (ld > FD && fd > FD) {
        state = TURN_LEFT;
      } else if (fd < CD && CD < ld && ld < FD){
        state = ROTATE_RIGHT;
      } else if (fd <= CD && ld > ld_prev && ld - ld_prev > JD){
        state = ROTATE_LEFT;
      }
      break;
      
    case TURN_RIGHT:
      turn_right(150);
      if (ld > CD && ld < FD && fd > CD){
        state = FORWARD;
      }
      break;

    case TURN_LEFT:
      turn_left(150);
      if (ld > CD && ld < FD && fd > CD){
        state = FORWARD;
      }
      break;
      
    case ROTATE_RIGHT:
      time = millis();
      while (millis()-time < time_rotate){
        rotate_right(255);
      } 
      state = FORWARD;
      break;

    case ROTATE_LEFT:
      time = millis();
      while(millis()-time < time_jogging){
        move_forward(255);
      } 
      time=millis();
      while(millis()-time < time_rotate){
        rotate_left(255);
      } 
      state = FORWARD;
      break;

  }
  ld_prev = ld;
}
