#include "Piece.h"
#include "Point.h"
#include "Keys.h"

#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

constexpr int WAIT_INTERVAL = 10000;

constexpr int FX = 20;
constexpr int FY = 24;

constexpr int N_OBJ_X = 4;
constexpr int N_OBJ_Y = 9;

constexpr int OBJ_NUM = 7;

Piece o1 = Piece{"01001100", 'A'};
Piece o2 = Piece{"01000110", 'A'};
Piece o3 = Piece{"00010110", 'A'};
Piece o4 = Piece{"00001101", 'A'};
Piece o5 = Piece{"00011100", 'A'};
Piece o6 = Piece{"01000100", 'A'};
Piece o7 = Piece{"00010101", 'A'};

Piece c_obj, n_obj;

int max_x, max_y;
int fo_x, fo_y;
int field[FX][FY] = {{0}};
int rmax, lmax, umax;
int timer0 = 0;
unsigned long timer1;
int landing_flag = 0;
long g_point;

void initTET(void);
void draw_obj(Piece obj, int n);
void kaiten(Piece *obj, int n);
void o_move(Piece *obj, char key);
void o_data(Piece obj);
void draw_field(void);
void set_field(void);
void o_check(Piece obj, char *key);
void line_lapse(Piece obj);
void choice_obj(Piece *obj);
void data(void);

int main(void) {
  char key;
  int i, j;
  int wait_interval = 10000;
  bool result_flag = false;
  bool up_flag = false;
  bool end = false;

  initTET();
  srand((unsigned)time(NULL));  //乱数の種をセット（最初に１回行う）
  set_field();

TITLE:
  g_point = 0;
  result_flag = false;
  choice_obj(&n_obj);

  c_obj = n_obj;

  n_obj.p.x = N_OBJ_X;
  n_obj.p.y = N_OBJ_Y;

  c_obj.p.x = max_x / 2;
  c_obj.p.y = fo_y;

  draw_field();
  refresh();

  // スタート画面
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
        wait_interval = 14000;
        goto START;
        break;

      case 'n':
        wait_interval = 10000;
        goto START;
        break;

      case 'h':
        wait_interval = 4000;
        goto START;
        break;

      case 'q':
        end = true;
        goto START;
        break;

      default:
        break;
    }
  }

START:
  if (end == true) {
    endwin();
    return 0;
  }

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
  timer1 = 0;
  draw_obj(n_obj, 0);
  while (1) {
    if (timer0 == 40) {
      key = DOWN_KEY;
      timer0 = 0;
    } else if (timer0 % 2 == 0) {
      key = getch();
      if (key == DOWN_KEY) g_point += 10;
      g_point++;

    } else {
      key = getch();
      if (key == DOWN_KEY) g_point += 10;
    }
    if (timer1 % 1000 == 0) {
      if (wait_interval > 1000) {
        wait_interval -= 50;
      }
    }
    if ((key == UP_KEY)) {
      up_flag = true;
      key = DOWN_KEY;
      g_point += 100;
    }

    if ((key == END_KEY)) {
      break;
    }

    if (landing_flag != true) {
      draw_obj(c_obj, 0);
    } else {
      up_flag = false;
      draw_obj(c_obj, 2);
      line_lapse(c_obj);
      c_obj = n_obj;
      draw_obj(n_obj, 1);
      choice_obj(&n_obj);
      n_obj.p.x = N_OBJ_X;
      n_obj.p.y = N_OBJ_Y;
      draw_obj(n_obj, 0);
      c_obj.p.x = max_x / 2;
      c_obj.p.y = fo_y;
      move(c_obj.p.y, c_obj.p.x);
      if (inch() != ' ') {
        result_flag = true;
      }
      landing_flag = false;
    }
    if (result_flag == true) {
      break;
    }

    data();
    //		o_data(c_obj);

    o_check(c_obj, &key);
    o_move(&c_obj, key);

    refresh();
    if (up_flag == true) {
      usleep(10);
    } else {
      usleep(wait_interval);
      timer1++;
    }
    timer0++;
  }

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
  printw("%8ld", g_point);
  move(max_y / 2 - 1, max_x / 2 - 7);
  printw("             ");
  move(max_y / 2, max_x / 2 - 6);
  printw("  restart[R]  ");
  move(max_y / 2 + 1, max_x / 2 - 5);
  printw("  quit[Q]  ");
  move(max_y / 2 + 2, max_x / 2 - 5);
  printw("             ");

  refresh();

  while (1) {
    key = getch();
    switch (key) {
      case 'r':
        clear();
        goto TITLE;
        break;
      case 'q':
        endwin();
        return 0;
      default:
        break;
    }
  }

  //終了
  endwin();
  return 0;
}

//オブジェクトをランダムで決定
void choice_obj(Piece *obj) {
  int n;

  n = rand() % OBJ_NUM + 1;

  switch (n) {
    case 1:
      *obj = o1;
      break;
    case 2:
      *obj = o2;
      break;
    case 3:
      *obj = o3;
      break;
    case 4:
      *obj = o4;
      break;
    case 5:
      *obj = o5;
      break;
    case 6:
      *obj = o6;
      break;
    case 7:
      *obj = o7;
      break;
    default:
      *obj = o1;
      break;
  }
}

//配列に壁のデータを格納
void set_field(void) {
  int i;
  for (i = 0; i < FY; i++) {
    field[0][i] = 100;
    field[FX - 1][i] = 100;
    if (i == (FY - 1)) {
      for (i = 0; i < FX; i++) {
        field[i][FY - 1] = 100;
      }
      i = FY - 1;
    }
  }
  fo_x = max_x / 2 - (FX / 2);
  fo_y = max_y / 2 - (FY / 2);
}

//壁を描画
void draw_field(void) {
  int i, j;
  for (i = 0; i < FY; i++) {
    for (j = 0; j < FX; j++) {
      move(fo_y + i, fo_x + j);
      if (field[j][i] == 100) {
        addch('@');
        refresh();
      } else if (field[j][i] == 300) {
        addch('?');
        refresh();
      }
    }
  }
}

//テトリスの準備
void initTET(void) {
  initscr();
  noecho();
  timeout(0);
  curs_set(0);
  cbreak();
  getmaxyx(stdscr, max_y, max_x);
  g_point = 0;
}

//ライン消しとライン移動
void line_lapse(Piece obj) {
  Point a = obj.p;
  int i, j, k, n = 0, flag = 0;
  char c;

  for (i = 2; i >= 0; i--) {
    for (j = 0; j < (FX - 2); j++) {
      move(a.y - i + 1, j + fo_x + 1);
      if (inch() != '?') break;
      flag++;
    }
    if (flag >= FX - 2) {
      for (j = 0; j < (FX - 2); j++) {
        move(a.y - i + 1, j + fo_x + 1);
        addch(' ');
        usleep(10000);
        refresh();
      }
      for (j = 0; j < FY - 2; j++) {
        for (k = 0; k < (FX - 2); k++) {
          move(a.y - i - j, k + fo_x + 1);
          c = inch();
          move(a.y - i - j + 1, k + fo_x + 1);
          addch(c);
        }
      }
      n++;
    }
    flag = 0;
  }
  switch (n) {
    case 1:
      g_point += 10000;
      break;

    case 2:
      g_point += 50000;
      break;

    case 3:
      g_point += 120000;

    default:
      break;
  }
}

//オブジェクトが移動できるかのチェック
void o_check(Piece obj, char *key) {
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
            landing_flag = 1;
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
      draw_obj(n_obj, 0);
    }
  }

  move(a.y + by, a.x + bx);
  if ((inch() != 'A') && (inch() != ' ') && (inch() != '@')) {
    if (*key == DOWN_KEY) {
      landing_flag = 1;
    }
    *key = UP_KEY;
  }
}

//オブジェクトの移動
void o_move(Piece *obj, char key) {
  int flag = 0;
  switch (key) {
    case RIGHT_KEY:
      if ((obj->p.x) < (fo_x + FX - rmax - 2)) {
        draw_obj(*obj, 1);
        obj->p.x++;
      }
      break;

    case LEFT_KEY:
      if (((obj->p.x) > (fo_x + lmax + 1))) {
        draw_obj(*obj, 1);
        obj->p.x--;
      }
      break;

    case DOWN_KEY:
      if (((obj->p.y) < (fo_y + FY - 2 - umax)) &&
          (field[obj->p.x - fo_x][obj->p.y + fo_y + 1 + umax] != 300)) {
        draw_obj(*obj, 1);
        obj->p.y++;
      } else {
        landing_flag = 1;
      }
      break;
      c_obj.p.y = fo_y;

    case CLOCKWISE_KEY:
      if (((obj->p.x) < (fo_x + FX - 2)) && ((obj->p.x) > (fo_x + 1)) &&
          ((obj->p.y) < (fo_y + FY - 2))) {
        draw_obj(*obj, 1);
        kaiten(obj, 0);
      }
      break;

    case C_CLOCKWISE_KEY:
      if (((obj->p.x) < (fo_x + FX - 2)) && ((obj->p.x) > (fo_x + 1)) &&
          ((obj->p.y) < (fo_y + FY - 2))) {
        draw_obj(*obj, 1);
        kaiten(obj, 1);
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
     printw("fo_x=%2d, fo_y=%2d", fo_x, fo_y);
     move(3,0);
     printw("p.x=%2d, p.y=%2d", c_obj.p.x, c_obj.p.y);
     move(4,0);
     printw("timer0=%3d, g_point=%ld", timer0, g_point);
   */
}

//ゲーム中、左上に描画する
void data() {
  move(1, 0);
  printw("Right[S], Left[F], Down[D], Fall[E]");
  move(2, 0);
  printw("Right-Handed[K], Left-Handed[L]");
  move(4, 0);
  printw("YOUR-POINT");
  move(5, 0);
  printw("%ld", g_point);
  //	move(6,0);
  //	printw("%ld", timer1);
  move(7, 0);
  printw("NEXT-PIECE");
}

//オブジェクトの描画
void draw_obj(Piece obj, int n) {
  Point a;
  int i;

  a = obj.p;

  move(a.y, a.x);
  addch(obj.c);

  i = 0;
  lmax = rmax = umax = 0;
  while (obj.m[i] != '\0') {
    if (obj.m[i] == '1') {
      switch (i) {
        case 0:
          move(a.y - 1, a.x - 1);
          lmax = 1;
          break;
        case 1:
          move(a.y - 1, a.x);
          break;
        case 2:
          move(a.y - 1, a.x + 1);
          rmax = 1;
          break;
        case 3:
          move(a.y, a.x + 1);
          rmax = 1;
          break;
        case 4:
          move(a.y + 1, a.x + 1);
          rmax = umax = 1;
          break;
        case 5:
          move(a.y + 1, a.x);
          umax = 1;
          break;
        case 6:
          move(a.y + 1, a.x - 1);
          lmax = umax = 1;
          break;
        case 7:
          move(a.y, a.x - 1);
          lmax = 1;
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

//オブジェクトの回転
void kaiten(Piece *obj, int n) {
  char temp[9] = "00000000";
  int i = 0;

  if (n == 0) {
    while (obj->m[i] != '\0') {
      if (obj->m[i] == '1') {
        if (i < 6) {
          temp[i + 2] = '1';
        } else {
          temp[i - 6] = '1';
        }
      }
      i++;
    }
  } else {
    while (obj->m[i] != '\0') {
      if (obj->m[i] == '1') {
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
    obj->m[i] = temp[i];
  }
}
