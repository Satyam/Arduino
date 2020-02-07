#include <Servo.h>
#include <pitches.h>

//Time to wait for each turn of the loop
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

// Tune

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

// Hold the values read for joystick in initial ie. idle position
int centerX;
int centerY;

void setup() {
  Serial.begin(9600);

  // Joystick
  pinMode(JOY_SWITCH, INPUT_PULLUP);

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

// Previous values of the joystick
int lastX;
int lastY;

void loop() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  // Check for significant change in Y direction
  if (abs(y - lastY) > 2) {
    lastY = y;
    Serial.print("Y:  ");
    Serial.println(y);

    setMotor(long(y - centerY) * MAX_SPEED / centerY);
  }

  // Check for significant change in X direction
  if (abs(x - lastX) > 2) {
    lastX = x;
    Serial.print("X:  ");
    Serial.print(x);
    Serial.print("  ");
    Serial.println(long(x) * MAX_SERVO / centerX / 2);

    servo.write(long(x) * MAX_SERVO / centerX / 2);

  }

  // Check for switch and play tune
  if (digitalRead(JOY_SWITCH) == LOW) {
    Serial.println("BEEP!");
    for (int i = 0; i < 5 ; i++) {
      tone(BUZZER, melody[i], i < 4 ? DURATION : 3 * DURATION);
      delay(GAP);
    }
  }

  delay(WAIT);
}
