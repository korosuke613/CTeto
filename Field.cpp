#include "Field.h"

//配列に壁のデータを格納
void Field::set() {
  std::vector<int> tmp = {0};
  tmp.resize(height);

  for (int i = 0; i < width; i++) {
    f.push_back(tmp);
  }
  for (int i = 0; i < height; i++) {
    f[0][i] = 100;
    f[width - 1][i] = 100;
    if (i == (height - 1)) {
      for (i = 0; i < width; i++) {
        f[i][height - 1] = 100;
      }
      i = height - 1;
    }
  }
  p.x = max_x / 2 - (width / 2);
  p.y = max_y / 2 - (height / 2);
}

//壁を描画
void Field::draw() {
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      move(p.y + i, p.x + j);
      if (f[j][i] == 100) {
        addch('@');
        refresh();
      } else if (f[j][i] == 300) {
        addch('?');
        refresh();
      }
    }
  }
}
