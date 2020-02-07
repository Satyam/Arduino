#include <SoftTimer.h>
#include <Potenciometro.h>

#define JOY_X A0


void onX(int value) {
  Serial.print("x: ");
  Serial.println(value);
}

Potenciometro p(JOY_X, &onX);

void cbWatchdog(Task* me) {
  Serial.println("watchdog");
}

Task tWatchdog(5000, cbWatchdog);


void setup() {
  Serial.begin(9600);

  SoftTimer.add(&tWatchdog);

  Serial.println("Ready");

  p.start();
}
