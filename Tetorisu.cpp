#include "Tetorisu.h"

#include <time.h>
#include <unistd.h>
#include <cstdlib>

void Tetorisu::game() {
  init();
  while (1) {
    title();
    //終了
    if (is_end) {
      endwin();
      return;
    }
    start();
    result();
    //終了
    if (is_end) {
      endwin();
      return;
    }
    clear();
  }
}

void Tetorisu::init() {
  std::srand((unsigned)time(NULL));  //乱数の種をセット（最初に１回行う）
  initscr();
  noecho();
  timeout(0);
  curs_set(0);
  cbreak();
  getmaxyx(stdscr, max_y, max_x);
}

//オブジェクトをランダムで決定
void Tetorisu::choice_obj(Piece *obj) {
  int n;

  n = rand() % pieces.size();
  *obj = pieces[n];
}
void Tetorisu::title() {
  score.point = 0;
  Field tmp_field(max_x, max_y);
  field = tmp_field;
  field.set();
  field.draw();

  // スタート画面
  bool is_next = false;
  while (1) {
    key = getch();
    move(max_y / 2 - 5, max_x / 2 - 4);
    printw("TETORISU");

    move(max_y / 2 - 3, max_x / 2 - 6);
    printw("Futa Hirakoba");

    move(max_y / 2, max_x / 2 - 4);
    printw("easy[E]");

    move(max_y / 2 + 1, max_x / 2 - 5);
    printw("normal[N]");

    move(max_y / 2 + 2, max_x / 2 - 4);
    printw("hard[H]");

    move(max_y / 2 + 4, max_x / 2 - 4);
    printw("quit[Q]");

    refresh();

    switch (key) {
      case 'e':
        wait_interval = EASY_INTERVAL;
        mode = Mode::EASY;
        is_next = true;
        break;
      case 'n':
        wait_interval = NORMAL_INTERVAL;
        mode = Mode::NORMAL;
        is_next = true;
        break;

      case 'h':
        wait_interval = HARD_INTERVAL;
        mode = Mode::NORMAL;
        is_next = true;
        break;

      case 'q':
        is_end = true;
        is_next = true;
        break;

      default:
        break;
    }
    if (is_next) {
      break;
    }
  }
}

void Tetorisu::start() {
  bool result_flag = false;
  bool up_flag = false;
  landing_flag = false;
  choice_obj(&n_obj);

  c_obj = n_obj;

  n_obj.p.x = N_OBJ_X;
  n_obj.p.y = N_OBJ_Y;

  c_obj.p.x = max_x / 2;
  c_obj.p.y = field.p.y;

  move(max_y / 2 - 5, max_x / 2 - 4);
  printw("        ");
  move(max_y / 2 - 3, max_x / 2 - 6);
  printw("             ");
  move(max_y / 2, max_x / 2 - 4);
  printw("       ");
  move(max_y / 2 + 1, max_x / 2 - 5);
  printw("         ");
  move(max_y / 2 + 2, max_x / 2 - 4);
  printw("       ");
  move(max_y / 2 + 4, max_x / 2 - 4);
  printw("       ");
  key = 'p';

  //ゲーム画面
  int timer0 = 0;
  unsigned long timer1 = 0;
  drawPiece(n_obj, 0);
  while (1) {
    if (timer0 == 40) {
      key = DOWN_KEY;
      timer0 = 0;
    } else if (timer0 % 2 == 0) {
      key = getch();
      if (key == DOWN_KEY) score.point += 10;
      score.point++;

    } else {
      key = getch();
      if (key == DOWN_KEY) score.point += 10;
    }
    if (timer1 % 1000 == 0) {
      if (wait_interval > 1000) {
        wait_interval -= 50;
      }
    }
    if ((key == UP_KEY)) {
      up_flag = true;
      key = DOWN_KEY;
      score.point += 100;
    }

    if ((key == END_KEY)) {
      break;
    }

    if (landing_flag != true) {
      drawPiece(c_obj, 0);
    } else {
      up_flag = false;
      drawPiece(c_obj, 2);
      int n = line_lapse(c_obj);
      score.addScoreLapse(n);
      c_obj = n_obj;
      drawPiece(n_obj, 1);
      choice_obj(&n_obj);
      n_obj.p.x = N_OBJ_X;
      n_obj.p.y = N_OBJ_Y;
      drawPiece(n_obj, 0);
      c_obj.p.x = max_x / 2;
      c_obj.p.y = field.p.y;
      move(c_obj.p.y, c_obj.p.x);
      if (inch() != ' ') {
        result_flag = true;
      }
      landing_flag = false;
    }
    if (result_flag == true) {
      break;
    }

    printData();
    //		o_data(c_obj);

    checkMove(c_obj, &key);
    movePiece(&c_obj, key);

    refresh();
    if (up_flag == true) {
      usleep(10);
    } else {
      usleep(wait_interval);
      timer1++;
    }
    timer0++;
  }
}

void Tetorisu::result() {
  //リザルト画面
  move(max_y / 2 - 6, max_x / 2 - 7);
  printw("             ");
  move(max_y / 2 - 5, max_x / 2 - 7);
  printw("  GAME OVER  ");
  move(max_y / 2 - 4, max_x / 2 - 7);
  printw("             ");
  move(max_y / 2 - 3, max_x / 2 - 7);
  printw("  YOUR POINT ");
  move(max_y / 2 - 2, max_x / 2 - 4);
  printw("%8ld", score.point);
  move(max_y / 2 - 1, max_x / 2 - 7);
  printw("             ");
  move(max_y / 2, max_x / 2 - 6);
  printw("  restart[R]  ");
  move(max_y / 2 + 1, max_x / 2 - 5);
  printw("  quit[Q]  ");
  move(max_y / 2 + 2, max_x / 2 - 5);
  printw("             ");

  refresh();

  bool is_next = false;
  while (1) {
    key = getch();
    switch (key) {
      case 'r':
        is_next = true;
        break;
      case 'q':
        is_end = true;
        is_next = true;
        break;
      default:
        continue;
    }
    if (is_next) {
      break;
    }
  }
}

//ライン消しとライン移動
int Tetorisu::line_lapse(Piece obj) {
  Point a = obj.p;
  int i, j, k, n = 0, flag = 0;
  char c;

  for (i = 2; i >= 0; i--) {
    for (j = 0; j < (FX - 2); j++) {
      move(a.y - i + 1, j + field.p.x + 1);
      if (inch() != '?') break;
      flag++;
    }
    if (flag >= FX - 2) {
      for (j = 0; j < (FX - 2); j++) {
        move(a.y - i + 1, j + field.p.x + 1);
        addch(' ');
        usleep(10000);
        refresh();
      }
      for (j = 0; j < FY - 2; j++) {
        for (k = 0; k < (FX - 2); k++) {
          move(a.y - i - j, k + field.p.x + 1);
          c = inch();
          move(a.y - i - j + 1, k + field.p.x + 1);
          addch(c);
        }
      }
      n++;
    }
    flag = 0;
  }
  return n;
}

//オブジェクトが移動できるかのチェック
void Tetorisu::checkMove(Piece obj, char *key) {
  int i, bx, by, rr;
  Point a;

  a = obj.p;

  i = 0;
  bx = by = rr = 0;

  switch (*key) {
    case RIGHT_KEY:
      bx = 1;
      break;

    case LEFT_KEY:
      bx = -1;
      break;

    case DOWN_KEY:
    case UP_KEY:
      by = 1;
      break;

    case CLOCKWISE_KEY:
      rr = 1;
      break;

    case C_CLOCKWISE_KEY:
      rr = -1;
      break;

    default:
      bx = by = 0;
      break;
  }

  if (rr == 0) {
    while (obj.m[i] != '\0') {
      if (obj.m[i] == '1') {
        switch (i) {
          case 0:
            move(a.y - 1 + by, a.x - 1 + bx);
            break;
          case 1:
            move(a.y - 1 + by, a.x + bx);
            break;
          case 2:
            move(a.y - 1 + by, a.x + 1 + bx);
            break;
          case 3:
            move(a.y + by, a.x + 1 + bx);
            break;
          case 4:
            move(a.y + 1 + by, a.x + 1 + bx);
            break;
          case 5:
            move(a.y + 1 + by, a.x + bx);
            break;
          case 6:
            move(a.y + 1 + by, a.x - 1 + bx);
            break;
          case 7:
            move(a.y + by, a.x - 1 + bx);
            break;
          default:
            break;
        }
        if ((inch() != 'A') && (inch() != ' ') && (inch() != '@')) {
          if (*key == DOWN_KEY) {
            landing_flag = true;
          }
          *key = 'p';
        } else {
        }
      }
      i++;
    }
  } else {
    while (obj.m[i] != '\0') {
      if (obj.m[i] == '1') {
        switch (i) {
          case 0:
            move(a.y - rr, a.x + rr);
            break;

          case 1:
            move(a.y, a.x + rr);
            break;

          case 2:
            move(a.y + rr, a.x + rr);
            break;

          case 3:
            move(a.y + rr, a.x);
            break;

          case 4:
            move(a.y + rr, a.x - rr);
            break;

          case 5:
            move(a.y, a.x - rr);
            break;

          case 6:
            move(a.y - rr, a.x - rr);
            break;

          case 7:
            move(a.y - rr, a.x);
            break;

          default:
            break;
        }
        if ((inch() != 'A') && (inch() != ' ') && (inch() != '@')) {
          *key = 'p';
        } else {
        }
      }
      i++;
      drawPiece(n_obj, 0);
    }
  }

  move(a.y + by, a.x + bx);
  if ((inch() != 'A') && (inch() != ' ') && (inch() != '@')) {
    if (*key == DOWN_KEY) {
      landing_flag = true;
    }
    *key = UP_KEY;
  }
}

//オブジェクトの移動
void Tetorisu::movePiece(Piece *obj, char key) {
  int flag = 0;
  switch (key) {
    case RIGHT_KEY:
      if ((obj->p.x) < (field.p.x + FX - obj->rmax - 2)) {
        drawPiece(*obj, 1);
        obj->p.x++;
      }
      break;

    case LEFT_KEY:
      if (((obj->p.x) > (field.p.x + obj->lmax + 1))) {
        drawPiece(*obj, 1);
        obj->p.x--;
      }
      break;

    case DOWN_KEY:
      if (((obj->p.y) < (field.p.y + FY - 2 - obj->umax)) &&
          (field.f[obj->p.x - field.p.x]
                  [obj->p.y + field.p.y + 1 + obj->umax] != 300)) {
        drawPiece(*obj, 1);
        obj->p.y++;
      } else {
        landing_flag = true;
      }
      break;
      c_obj.p.y = field.p.y;

    case CLOCKWISE_KEY:
      if (((obj->p.x) < (field.p.x + FX - 2)) &&
          ((obj->p.x) > (field.p.x + 1)) &&
          ((obj->p.y) < (field.p.y + FY - 2))) {
        drawPiece(*obj, 1);
        obj->rotate(true);
      }
      break;

    case C_CLOCKWISE_KEY:
      if (((obj->p.x) < (field.p.x + FX - 2)) &&
          ((obj->p.x) > (field.p.x + 1)) &&
          ((obj->p.y) < (field.p.y + FY - 2))) {
        drawPiece(*obj, 1);
        obj->rotate(false);
      }
      break;

    default:
      flag = 1;
      break;
  }
}

//オブジェクトのデータ把握
void o_data(Piece obj) {
  int i = 0;
  while (i < 9) {
    move(0, 0 + i);
    if (obj.m[i] == '1') {
      addch('1');
    } else {
      addch('0');
    }
    i++;
  }
  /*
     move(1,0);
     printw("lmax=%2d, rmax=%2d, umax=%2d", lmax, rmax, umax);
     move(2,0);
     printw("field.p.x=%2d, field.p.y=%2d", field.p.x, field.p.y);
     move(3,0);
     printw("p.x=%2d, p.y=%2d", c_obj.p.x, c_obj.p.y);
     move(4,0);
     printw("timer0=%3d, g_point=%ld", timer0, g_point);
   */
}

//ゲーム中、左上に描画する
void Tetorisu::printData() {
  move(1, 0);
  printw("Right[S], Left[F], Down[D], Fall[E]");
  move(2, 0);
  printw("Right-Handed[K], Left-Handed[L]");
  move(4, 0);
  printw("YOUR-POINT");
  move(5, 0);
  printw("%ld", score.point);
  //	move(6,0);
  //	printw("%ld", timer1);
  move(7, 0);
  printw("NEXT-PIECE");
}

//オブジェクトの描画
void Tetorisu::drawPiece(Piece obj, int n) {
  Point a;
  int i;

  a = obj.p;

  move(a.y, a.x);
  addch(obj.c);

  i = 0;
  while (obj.m[i] != '\0') {
    if (obj.m[i] == '1') {
      switch (i) {
        case 0:
          move(a.y - 1, a.x - 1);
          break;
        case 1:
          move(a.y - 1, a.x);
          break;
        case 2:
          move(a.y - 1, a.x + 1);
          break;
        case 3:
          move(a.y, a.x + 1);
          break;
        case 4:
          move(a.y + 1, a.x + 1);
          break;
        case 5:
          move(a.y + 1, a.x);
          break;
        case 6:
          move(a.y + 1, a.x - 1);
          break;
        case 7:
          move(a.y, a.x - 1);
          break;
        default:
          break;
      }
      if (n == 1) {
        addch(' ');
        move(a.y, a.x);
        addch(' ');
      } else if (n == 2) {
        addch('?');
        move(a.y, a.x);
        addch('?');
      } else {
        addch(obj.c);
      }
    }
    i++;
  }
}
