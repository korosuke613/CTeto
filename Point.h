#ifndef __POINT_H__
#define __POINT_H__

class Point {
 public:
  constexpr Point() {}
  constexpr explicit Point(int x_, int y_) : x(x_), y(y_) {}
  int x = 0;
  int y = 0;
};

#endif