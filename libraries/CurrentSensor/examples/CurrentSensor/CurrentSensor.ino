#include <SoftTimer.h>
#include <CurrentSensor.h>

#define CURRENT_SENSOR A2

#define ENABLE 5
#define DIRA 3
#define DIRB 4

#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

int speeds[] = {
  0,
  170,
  255,
  0,
  -170,
  -255
};

int i = 0;

void onCs(float value) {
  Serial.print("speed: ");
  Serial.print(speeds[i]);
  Serial.print(" value: ");
  Serial.println(value);
}

CurrentSensor cs(A2, &onCs);


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


void cbSwitchMotor(Task* me) {
  i++;
  if (i > NELEMS(speeds)) i = 0;
  setMotor(speeds[i]);
  Serial.print("Set speed: ");
  Serial.println(speeds[i]);
}

Task tSetSpeed(3000, cbSwitchMotor);

void setup() {
  Serial.begin(9600);

  SoftTimer.add(&tSetSpeed);
  Serial.println("Ready");

  cs.setPollingInterval(20);
  cs.setTotalCount(20);
  cs.start();

}