#include "Piece.h"

//オブジェクトの回転
void Piece::rotate(bool isClockwise) {
  constexpr int isBlock = '1';
  char temp[9] = "00000000";
  int i = 0;

  if (isClockwise) {
    while (m[i] != '\0') {
      if (m[i] == isBlock) {
        if (i < 6) {
          temp[i + 2] = '1';
        } else {
          temp[i - 6] = '1';
        }
      }
      i++;
    }
  } else {
    while (m[i] != '\0') {
      if (m[i] == isBlock) {
        if (i > 1) {
          temp[i - 2] = '1';
        } else {
          temp[i + 6] = '1';
        }
      }
      i++;
    }
  }

  for (i = 0; i < 9; i++) {
    m[i] = temp[i];
  }
  setLRU();
}

void Piece::setLRU() {
  int i = 0;
  lmax = rmax = umax = 0;

  while (m[i] != '\0') {
    if (m[i] == '1') {
      /*
        012
        7 3
        654
      */
      switch (i) {
        case 0:
        case 7:
          lmax = 1;
          break;
        case 2:
        case 3:
          rmax = 1;
          break;
        case 4:
          rmax = umax = 1;
          break;
        case 5:
          umax = 1;
          break;
        case 6:
          lmax = umax = 1;
          break;
        default:
          break;
      }
    }
    i++;
  }
}