#include <SoftTimer.h>
#include <CurrentSensor.h>

#define CURRENT_SENSOR A2

#define ENABLE 5
#define DIRA 3
#define DIRB 4

#define NUM_ELEMS(x)  (sizeof(x) / sizeof((x)[0]))

void onX(float value) {
  Serial.print("value: ");
  Serial.println(value);
}

CurrentSensor cs(A2, &onX);

void cbWatchdog(Task* me) {
  Serial.println("watchdog");
}

Task tWatchdog(5000, cbWatchdog);

void setMotor(int speed)  {
  if (speed < 0) {
    digitalWrite(DIRA, HIGH);
    digitalWrite(DIRB, LOW);

  } else {
    digitalWrite(DIRA, LOW);
    digitalWrite(DIRB, HIGH);
  }
  analogWrite(ENABLE, abs(speed));
}

// Sequence of motor speeds in range from -255 to 255
int speeds[] = {
  0,
  128,
  255,
  0,
  -128,
  -255
};

int i = 0;

void cbSwitchMotor(Task* me) {
  if (i > NUM_ELEMS(speeds)) i = 0;
  setMotor(speeds[i]);
  Serial.print("Set speed: ");
  Serial.println(speeds[i]);
  i++;
}

Task tSetSpeed(3000, cbSwitchMotor);

void setup() {
  Serial.begin(9600);

  SoftTimer.add(&tWatchdog);
  SoftTimer.add(&tSetSpeed);
  Serial.println("Ready");

  cs.start();

}