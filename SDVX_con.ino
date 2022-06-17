#include <HID-Project.h>
#include <Encoder.h>
#define ENC_TIME 80

// 로티러 엔코더
Encoder leftEnc(0, 1);
Encoder rightEnc(2, 3);
long pos = 0;
long encCheck[2] = {0, 0};
long encDir[2] = {0, 0};
long encTime[2] = {0, 0};

// 버튼
int ledPin[7] = {4, 6, 8, 16, 15, 19, 21};
int btnPin[7] = {5, 7, 9, 10, 14, 18, 20};
int btnCheck[7] = {0, 0, 0, 0, 0, 0, 0};

// for 변수
int i = 0;

/* 입력키 : 본인의 컨트롤러에 맞게 변경 */
char btnChar[7] = {'\n', 'c', 'm', 'd', 'f', 'j', 'k'};
char encChar[4] = {';', 'l', 's', 'a'};

void setup() {
  for(i=0; i<7; i++) {
    pinMode(ledPin[i], OUTPUT);
    pinMode(btnPin[i], INPUT);
  }
}

void loop() {
  /* 버튼 인식 */
  for(i=0; i<7; i++) {
    if(btnCheck[i] != digitalRead(btnPin[i])) {
      btnCheck[i] = (btnCheck[i] + 1) % 2;
      if(btnCheck[i]) {
        NKROKeyboard.press(btnChar[i]);
      } else {
        NKROKeyboard.release(btnChar[i]);
      }
      digitalWrite(ledPin[i], btnCheck[i] ? HIGH : LOW);
    }
  }

  /* 노브 인식 */
  for(i=0; i<2; i++) {
    pos = (i ? leftEnc.read() : rightEnc.read()) / 4;
    if(encCheck[i] < pos) { // 오른쪽
      if(encDir[i] != 1) {
        NKROKeyboard.press(encChar[i * 2]);
        NKROKeyboard.release(encChar[i * 2 + 1]);
        encDir[i] = 1;
      }
    } else if(encCheck[i] > pos) { // 왼쪽
      if(encDir[i] != -1) {
        NKROKeyboard.press(encChar[i * 2 + 1]);
        NKROKeyboard.release(encChar[i * 2]);
        encDir[i] = -1;
      }
    } else {
      if(encDir[i]) {
        encTime[i] = millis() + ENC_TIME;
        encDir[i] = 0;
      } else if(encTime[i] < millis()) {
        NKROKeyboard.release(encChar[i * 2]);
        NKROKeyboard.release(encChar[i * 2 + 1]);
      }
    }
    encCheck[i] = pos;
  }
}
