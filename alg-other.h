#include <iostream>
typedef void (*AlgoPixel)(int x, int y, void *data);

const int ABS(int a) { return (a < 0 ? -a : a); }
const int SGN(int a) { return (a == 0 ? 0 : a/ABS(a)); }

void algo_line(int x1, int y1, int x2, int y2, void* data, AlgoPixel proc)
{
  bool yaxis;

  // If the height if the line is bigger than the width, we'll iterate
  // over the y-axis.
  if (ABS(y2-y1) > ABS(x2-x1)) {
    std::swap(x1, y1);
    std::swap(x2, y2);
    yaxis = true;
  }
  else
    yaxis = false;

  const int w = ABS(x2-x1)+1;
  const int h = ABS(y2-y1)+1;
  const int dx = SGN(x2-x1);
  const int dy = SGN(y2-y1);

  int e = 0;
  int y = y1;

  // Move x2 one extra pixel to the dx direction so we can use
  // operator!=() instead of operator<(). Here I prefer operator!=()
  // instead of swapping x1 with x2 so the error always start from 0
  // in the origin (x1,y1).
  x2 += dx;

  for (int x=x1; x!=x2; x+=dx) {
    if (yaxis)
      proc(y, x, data);
    else
      proc(x, y, data);

    // The error advances "h/w" per each "x" step. As we're using a
    // integer value for "e", we use "w" as the unit.
    e += h;
    if (e >= w) {
      y += dy;
      e -= w;
    }
  }
}
