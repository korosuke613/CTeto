#ifndef __TETORISU__H__
#define __TETORISU__H__

#include <curses.h>
#include <vector>
#include "Field.h"
#include "Piece.h"
#include "Score.h"
#include "settings.h"

enum Mode { EASY, NORMAL, HARD };

class Tetorisu {
 public:
  Tetorisu() { init(); }
  void game();
  void init();
  void title();
  void start();
  void choice_obj(Piece *obj);
  int line_lapse(Piece obj);
  void checkMove(Piece obj, char *key);
  void movePiece(Piece *obj, char key);
  void printData();
  void drawPiece(Piece obj, int n);
  void result();

  int max_x, max_y;  // 画面サイズ
  Score score;

  std::vector<Piece> pieces{Piece{"01001100", 'A'}, Piece{"01000110", 'A'},
                            Piece{"00010110", 'A'}, Piece{"00001101", 'A'},
                            Piece{"00011100", 'A'}, Piece{"01000100", 'A'},
                            Piece{"00010101", 'A'}};
  Piece c_obj, n_obj;
  Field field;
  char key;
  int wait_interval = EASY_INTERVAL;
  bool is_end = false;  // ゲーム終了フラグ
  bool landing_flag = false;
  Mode mode = Mode::EASY;
};

#endif