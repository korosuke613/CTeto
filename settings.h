#ifndef __SETTINGS_H__
#define __SETTINGS_H__

constexpr int WAIT_INTERVAL = 10000;

constexpr int FX = 20;  // 壁の横の長さ
constexpr int FY = 24;  // 壁の縦の長さ

constexpr int N_OBJ_X = 4;
constexpr int N_OBJ_Y = 9;

constexpr int EASY_INTERVAL = 14000;
constexpr int NORMAL_INTERVAL = 10000;
constexpr int HARD_INTERVAL = 4000;

constexpr int SCORE_LAPSE[3] = {10000, 50000, 120000};

constexpr char RIGHT_KEY = 'f';
constexpr char LEFT_KEY = 's';
constexpr char UP_KEY = 'e';
constexpr char DOWN_KEY = 'd';
constexpr char END_KEY = 'q';
constexpr char CLOCKWISE_KEY = 'l';
constexpr char C_CLOCKWISE_KEY = 'k';

#endif