#ifndef AUTITO_REMOTE_H
#define AUTITO_REMOTE_H

#include <SoftTimer.h>
#include <CurrentSensor.h>
#include <Potenciometro.h>
#include <Debouncer.h>
#include <PciManager.h>
#include <TunePlayer.h>
#include <Servo.h>
#include <pitches.h>
#include <HBridge.h>

#define DEBOUNCE_INTERVAL 25

#define MAX_BYTE 255

// Joystick
#define JOY_SWITCH 2
#define JOY_X A1
#define JOY_Y A0

void jXHandler(int x);
void jYHandler(int y);

// DC Motor
#define ENABLE 5
#define DIRA 3
#define DIRB 4
#define MAX_SPEED 255

// Servo
#define SERVO 9
#define MAX_SERVO 90

void setTurn(int turn);

// Music:
#define BUZZER 8
#define DURATION 300
#define GAP 30

int tune[] = {
  NOTE_AS4, 1,
  NOTE_C5,  1,
  NOTE_GS4, 1,
  NOTE_GS3, 1,
  NOTE_DS4, 3,
  0  
};
void startTune();
void tuneEndHandler();

// Current Sensor
#define CURRENT_SENSOR A2
void csHandler(float value);

// Remote commands

#define GO_FORWARD 'f'
#define GO_BACK 'b'
#define STOP 'S'

#define FULL_FORWARD 'F'
#define FULL_BACK 'B'

#define TURN_LEFT 'l'
#define TURN_RIGHT 'r'
#define GO_STRAIGHT '|'

#define BEEP '!'

#define LED '#'

#define REMOTE 'R'

void cmdHandler(Task* me);

// Report Status

#define REMOTE_CMD '>'
#define PARAM '='
#define SPEED 's'
#define TURN 't'
#define CURRENT 'c'
#define X_POS 'x'
#define Y_POS 'y'

#define REPORT(cmd, value)  Serial.print(cmd);Serial.println(value)

#endif