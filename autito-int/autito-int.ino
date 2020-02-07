#include <Servo.h>
#include <pitches.h>

#define WAIT 100

// Joystick
#define JOY_SWITCH 2
#define JOY_X A0
#define JOY_Y A1

// DC Motor
#define ENABLE 5
#define DIRA 3
#define DIRB 4
#define MAX_SPEED 255

// Servo
#define SERVO 9
#define MAX_SERVO 180

Servo servo;

#define BUZZER 8
#define DURATION (300)
#define GAP (DURATION + 50)

int melody[] = {
  NOTE_AS4, NOTE_C5, NOTE_GS4, NOTE_GS3, NOTE_DS4
};


void setMotor(int speed)  {
  int s = min(abs(speed), MAX_SPEED);
  Serial.print("Motor set at: ");
  Serial.println(s);
  Serial.println(speed);

  if (speed < 0) {
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);

  } else {
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
  }
  analogWrite(ENABLE, s);
}

// Play tune
#define QUIET 0
#define START_PLAYING 1
#define PLAYING 2

int note = 0;
int playing = QUIET;
unsigned long timeForNextNote;

// ISR for switch: plays tune
void handleSwitch() {
  if (playing == QUIET) playing = START_PLAYING;
}


// Variables to hold values for centered joystick
int centerX;
int centerY;

void setup() {
  Serial.begin(9600);

  // Joystick
  pinMode(JOY_SWITCH, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(JOY_SWITCH), handleSwitch, LOW);

  // DC Motor
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  // Read the values for the joystick at idle (center) position
  centerX = analogRead(JOY_X);
  centerY = analogRead(JOY_Y);


  Serial.print("Joystick centered at: ");
  Serial.print(centerX);
  Serial.print(", ");
  Serial.println(centerY);

  // Servo
  servo.attach(SERVO);
  servo.write(MAX_SERVO / 2);
}

int lastX;
int lastY;

void loop() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  // Check for significant movement on Y axis
  if (abs(y - lastY) > 2) {
    lastY = y;
    Serial.print("Y:  ");
    Serial.println(y);

    setMotor(long(y - centerY) * MAX_SPEED / centerY);
  }

  // Check for significant movement on X axis
  if (abs(x - lastX) > 2) {
    lastX = x;
    Serial.print("X:  ");
    Serial.print(x);
    Serial.print("  ");
    Serial.println(long(x) * MAX_SERVO / centerX / 2);

    servo.write(long(x) * MAX_SERVO / centerX / 2);

  }

  // Check on tune playing
  switch (playing) {
    case START_PLAYING:
      Serial.println("Start Tune");
      playing = PLAYING;
      note = 0;
      timeForNextNote = millis() + GAP;
      tone(BUZZER, melody[note], DURATION);
      break;
    case PLAYING:
      if (millis() > timeForNextNote) {
        note++;
        Serial.print("next note ");
        Serial.println(note);
        if (note == 4) {
          Serial.println("Stop tune");

          playing = QUIET;
          tone(BUZZER, melody[note], 3 * DURATION);
        } else {
          tone(BUZZER, melody[note], DURATION);
          timeForNextNote = millis() + GAP;
        }
      }
      break;
      // No need to do anything for playing == QUIET

  }

  // wait for a while
  delay(WAIT);
}
