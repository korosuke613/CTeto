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
}