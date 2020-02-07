#include "Joystick.h"

#define JOY_SWITCH 2
#define JOY_X A0
#define JOY_Y A1


void onPressed() {
  Serial.println("Button pressed");
};

void onX(int x) {
  Serial.print("x: ");
  Serial.println(x);
}

void onY(int y) {
  Serial.print("y: ");
  Serial.println(y);
}

void cbWatchdog(Task* me) {
  Serial.println("watchdog");
}

Task tWatchdog(1000, cbWatchdog);


void setup() {
  Serial.begin(9600);
  SoftTimer.add(&tWatchdog);
  Serial.println("Ready");
  Joystick j(
    JOY_X, &onX,
    JOY_Y, &onY,
    JOY_SWITCH, &onPressed
  );
  // PciManager.registerListener(JOY_SWITCH, &j);
  j.setPeriodMs(1000);
  j.setXYPolling(1000);
}
