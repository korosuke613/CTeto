#ifndef __SCORE_H__
#define __SCORE_H__

#include "settings.h"

class Score {
 public:
  int point = 0;

  void addScoreLapse(int n) { point += SCORE_LAPSE[n]; }
};

#endif