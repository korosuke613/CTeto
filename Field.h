#include <curses.h>
#include <vector>
#include "Point.h"

class Field {
 public:
  Field() { getmaxyx(stdscr, max_y, max_x); }
  explicit Field(int max_x_, int max_y_) : max_x(max_x_), max_y(max_y_) {}
  void set();   // 壁の作成
  void draw();  // 壁の描画

  int max_x = 0;                    // コンソールの横の長さ
  int max_y = 0;                    // コンソールの縦の長さ
  int width = 20;                   // 壁の横の長さ
  int height = 24;                  // 壁の縦の長さ
  Point p;                          // 壁の左上の座標
  std::vector<std::vector<int>> f;  // 壁
};