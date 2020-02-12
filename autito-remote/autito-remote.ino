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

  currentSensor.setDiscardExtremes(true);

  currentSensor.start();

  Serial.println("s0t0!0#0");

}

char remoteCommand = 0;


void cmdHandler(Task* me) {
  if (Serial.available()) {
    if (remoteCommand) {
      byte value = (byte)Serial.read();
      switch (remoteCommand) {

        case GO_FORWARD: 
          setMotor(value);
          REPORT(SPEED, value);
          break;

        case GO_BACK:
          setMotor(-value);
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
        case GO_STRAIGHT:
        case BEEP:
        default:
          break;
      }


      remoteCommand = 0;
    } else {

      remoteCommand = (char)Serial.read();
      REPORT(REMOTE_CMD, char(remoteCommand));
      switch (remoteCommand) {

        // These need an extra argument
        case GO_FORWARD: 
        case GO_BACK: 
        case TURN_LEFT: 
        case TURN_RIGHT: 
        case LED:
        case CURRENT:
        case REMOTE:
          break;

        case STOP:
          setMotor(0);
          REPORT(SPEED, 0);
          remoteCommand = 0;
          break;

        case GO_STRAIGHT:
          setTurn(0);
          REPORT(TURN, 0);
          remoteCommand = 0;
          break;

        case BEEP:
          startTune();
          remoteCommand = 0;
          break;

        default:
          REPORT(PARAM, remoteCommand);
          remoteCommand = 0;
          break;
      }
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

