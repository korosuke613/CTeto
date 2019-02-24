#ifndef __PIECE_H__
#define __PIECE_H__

#include <cstring>
#include <string>
#include "Point.h"

class Piece {
 public:
  Piece() {}
  explicit Piece(const char* m_, char c_) : c(c_) { std::strcpy(m, m_); }
  Point p;
  char m[9];
  char c;
};

#endif