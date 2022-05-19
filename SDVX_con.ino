#include <Keyboard.h>
#include <Encoder.h>
#define ENC_TIME 70

int btnPin[7] = {10, 9, 8, 7, 6, 5, 4};
char btnChar[7] = {KEY_RETURN, 'm', 'c', 'k', 'j', 'f', 'd'};
int btnHold[7] = {0, 0, 0, 0, 0, 0, 0};
Encoder leftEnc(0, 1);
Encoder rightEnc(2, 3);
long oldPos[2] = {0, 0};
long newPos[2] = {0, 0};
char encChar[4] = {'a', 's', 'l', ';'};
int encHold[2] = {0, 0};
long encTime[2] = {0, 0};
int i = 0;

void setup() {
  for (i = 0; i < 7; i++) {
    pinMode(i, INPUT);
  }
}

void loop() {
  for (i = 0; i < 7; i++) {
    if (btnHold[i] != digitalRead(btnPin[i])) {
      btnHold[i] = (btnHold[i] + 1) % 2;
      if (btnHold[i]) { // 버튼을 누른 경우
        Keyboard.press(btnChar[i]);
      } else { // 버튼을 뗀 경우
        Keyboard.release(btnChar[i]);
      }
    }
  }

  for (i = 0; i < 2; i++) {
    newPos[i] = i ? rightEnc.read() / 4 : leftEnc.read() / 4;
    if (newPos[i] != oldPos[i]) {
      if(oldPos[i] < newPos[i]) { // 오른쪽
        if(encHold[i] != 1) {
          Keyboard.press(encChar[i * 2 + 1]);
          Keyboard.release(encChar[i * 2]);
          encHold[i] = 1;
        }
      } else if(oldPos[i] > newPos[i]) { // 왼쪽
        if(encHold[i] != -1) {
          Keyboard.press(encChar[i * 2]);
          Keyboard.release(encChar[i * 2 + 1]);
          encHold[i] = -1;
        }
      }
      encTime[i] = millis();
      oldPos[i] = newPos[i];
    } else if(encHold[i] != 0 && encTime[i] + ENC_TIME < millis()) {
      Keyboard.release(encChar[i * 2]);
      Keyboard.release(encChar[i * 2 + 1]);
      encHold[i] = 0;
    }
  }
}
