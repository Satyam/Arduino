#include "autito-remote.h";

Potenciometro jX(JOY_X, jXHandler);
Potenciometro jY(JOY_Y, jYHandler);
Debouncer jSwDebouncer(JOY_SWITCH, MODE_CLOSE_ON_PUSH, startTune, NULL, true);

CurrentSensor currentSensor(CURRENT_SENSOR, csHandler);

HBridge motor(DIRA, DIRB, ENABLE);

Servo servo;

CmdOneByte remoteCmd = CmdOneByte(cmdHandler);

void setup() {
  Serial.begin(9600);

  // Joystick
  jX.start();
  jY.start();
  jSwDebouncer.init();
  PciManager.registerListener(JOY_SWITCH, &jSwDebouncer);


  // DC Motor
  motor.init();

  // Servo
  servo.attach(SERVO);
  setTurn(0);

  // Built in LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Remote command reader
  remoteCmd.setNoArgCmds("S|!"); // Stop, Straight, beep
  remoteCmd.start();
  
  // Tune player

  TunePlayer.attach(BUZZER);
  TunePlayer.onEnd(tuneEndHandler);

  // Play it slow
  TunePlayer.setNoteDuration(DURATION);
  TunePlayer.setGap(GAP);

  // Current sensor

  currentSensor.setDiscardExtremes(true);

  currentSensor.start();

  Serial.println("s0\nt0\n!0\n#0");

}

void cmdHandler(char cmd, byte value) {
  REPORT(REMOTE_CMD, cmd);
  switch (cmd) {

    case GO_FORWARD: 
      motor.setSpeed(value);
      REPORT(SPEED, value);
      break;

    case GO_BACK:
      motor.setSpeed(-value);
      REPORT(SPEED, -value);
      break;

    case TURN_LEFT: 
      setTurn(-value);
      REPORT(TURN, -value);
      break;

    case TURN_RIGHT: 
      setTurn(value);
      REPORT(TURN, value);
      break;

    case LED: 
      digitalWrite(LED_BUILTIN, value ? HIGH : LOW );
      REPORT(LED, value);
      break;

    case CURRENT:
      if (value) {
        currentSensor.start();
      } else {
        currentSensor.stop();
      }
      break;

    case REMOTE:
      REPORT(REMOTE, value);
      if (value) {
        jX.stop();
        jY.stop();
      } else {
        jX.start();
        jY.start();
      }
      break;

    case STOP:
      motor.setSpeed(0);
      REPORT(SPEED, 0);
      break;

    case GO_STRAIGHT:
      setTurn(0);
      REPORT(TURN, 0);
      break;

    case BEEP:
      startTune();
      break;

    default:
      REPORT(PARAM, cmd);
      break;
  }
}

void setTurn(int turn) {
  // Scale range of -255 to 255 into 0 to 180 for servo
  servo.write(long(turn) * MAX_SERVO / MAX_BYTE + MAX_SERVO);
}

int lastSpeed = 0;
unsigned long peakEndTime = 0;
float csSum = 0.0;
float csCnt = 0;
int overCurrentCount = 0;

// These values might need to be tweeked:
float OVERCURRENT_FACTOR = 1.1;
int OVERCURRENT_COUNT = 8;
unsigned long PEAK_DURATION = 500;
int SIZEOF_SAMPLE = 100;
// ---

void csHandler(float current) {
  REPORT(CURRENT, current);
  int speed = motor.getSpeed();
  if (lastSpeed != speed) {
    lastSpeed = speed;
    if (speed) {
      peakEndTime = millis() + PEAK_DURATION;
    } else {
      peakEndTime = 0;
      csSum = 0.0;
      csCnt = 0;
      overCurrentCount = 0;
    }
  }
  if (speed && millis() > peakEndTime && csCnt < SIZEOF_SAMPLE) {
    csSum += current;
    csCnt += 1;
  }
  if (csCnt > 10) {
    float limit = csSum / float(csCnt) * OVERCURRENT_FACTOR;
    if (speed > 0) {
      if (current > limit) {
        overCurrentCount += 1;
      } else {
        overCurrentCount = 0;
      }
    } else {
      if (current < limit) {
        overCurrentCount += 1;
      } else {
        overCurrentCount = 0;
      }
    }
  }
  if (overCurrentCount > OVERCURRENT_COUNT) {
    REPORT(HALTED, "halted");
    motor.setSpeed(0);
    REPORT(SPEED, 0);

  }
}


void startTune() {
  REPORT(BEEP, true);
  TunePlayer.play(tune);
}

void tuneEndHandler() {
  REPORT(BEEP, false);
};

void jXHandler(int x) {
  int a = min(x, MAX_BYTE);

  setTurn(a);

  REPORT(TURN, a);
  REPORT(X_POS, a);

};
void jYHandler(int y) {
  int s = - min(y, MAX_BYTE);
  motor.setSpeed(s);
  REPORT(SPEED, s);
  REPORT(Y_POS, s);
  
};

