#include "autito-remote.h";

Potenciometro jX(JOY_X, jXHandler);
Potenciometro jY(JOY_Y, jYHandler);
Debouncer jSwDebouncer(JOY_SWITCH, MODE_CLOSE_ON_PUSH, startTune, NULL, true);

CurrentSensor currentSensor(CURRENT_SENSOR, csHandler);

Servo servo;

Task fetchCmd(1, cmdHandler);

void setup() {
  Serial.begin(9600);

  // Joystick
  jX.start();
  jY.start();
  jSwDebouncer.init();
  PciManager.registerListener(JOY_SWITCH, &jSwDebouncer);


  // DC Motor
  pinMode(ENABLE, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);


  // Servo
  servo.attach(SERVO);
  setTurn(0);

  // Built in LED
  pinMode(LED_BUILTIN, OUTPUT);

  // Remote command reader
  SoftTimer.add(&fetchCmd);

  // Tune player

  TunePlayer.attach(BUZZER);
  TunePlayer.onEnd(tuneEndHandler);

  // Play it slow
  TunePlayer.setNoteDuration(DURATION);
  TunePlayer.setGap(GAP);

  // Current sensor

  // Make it slow right now so it doesn't overwhelm the logger.
  currentSensor.setPollingInterval(50);
  currentSensor.setTotalCount(20);

  // *** currentSensor.start();

  Serial.println("s0t0!0#0");

}


void cmdHandler(Task* me) {
  if (Serial.available()) {
    byte remoteCommand = (byte)Serial.read();
    REPORT(REMOTE_CMD, char(remoteCommand));
    REPORT(PARAM, (byte) Serial.peek());
    switch (remoteCommand) {

      case GO_FORWARD: {
        byte speed = (byte)Serial.read();
        setMotor(speed);

        REPORT(SPEED, speed);
        }
        break;

      case GO_BACK: {
        byte speed = (byte)Serial.read();
        setMotor(-speed);

        REPORT(SPEED, -speed);
        }
        break;

      case STOP:
        setMotor(0);

        REPORT(SPEED, 0);
        break;

      case TURN_LEFT: {
        byte turn = (byte)Serial.read();
        setTurn(-turn);

        REPORT(TURN, -turn);
        }
        break;

      case TURN_RIGHT: {
        byte turn = (byte)Serial.read();
        setTurn(turn);

        REPORT(TURN, turn);
        }
        break;

      case GO_STRAIGHT:
        setTurn(0);

        REPORT(TURN, 0);
        break;

      case BEEP:
        startTune();
        break;

      case LED: {
        byte led  = (byte)Serial.read();
        digitalWrite(LED_BUILTIN, led ? HIGH : LOW );

        REPORT(LED, led);
      }
        break;
      case CURRENT:
        if ((byte) Serial.read()) {
          currentSensor.start();
        } else {
          currentSensor.stop();
        }
        break;
      case REMOTE:
        bool remote = (bool)Serial.read();
        REPORT(REMOTE, remote);

        if (remote) {
          jX.stop();
          jY.stop();
        } else {
          jX.start();
          jY.start();
        }
        break;

      default:
        break;
    }
  }
}


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

void setTurn(int turn) {
  // Scale range of -255 to 255 into 0 to 180 for servo
  servo.write(long(turn) * MAX_SERVO / MAX_BYTE + MAX_SERVO);
}

void csHandler(float value) {
  REPORT(CURRENT, value);
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
  setMotor(s);
  REPORT(SPEED, s);
  REPORT(Y_POS, s);
  
};

