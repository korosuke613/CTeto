#ifndef __PIECE_H__
#define __PIECE_H__

#include <curses.h>
#include <cstring>
#include <string>
#include "Point.h"

class Piece {
 public:
  Piece() {}
  explicit Piece(const char* m_, char c_) : c(c_) {
    std::strcpy(m, m_);
    setLRU();
  }
  void rotate(bool isClockwise);
  void setLRU();
  Point p;
  char m[9];
  char c;
  int lmax = 0;  // 左を占有しているサイズ
  int rmax = 0;  // 右を占有しているサイズ
  int umax = 0;  // 下を占有しているサイズ
};

#endif