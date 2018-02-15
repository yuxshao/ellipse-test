#include <iostream>
#include "alg.h"

const int sx = 50;
const int sy = 50;
int mat[sx][sy] = {0};
int x2; int y2;

void fillmat(int x, int y, void *data) {
  mat[x][y] += *(int *)data;
}

void testmat(int x, int y, void *data) {
  if (mat[x][y] == *(int *)data)
    throw "FAILED";
  mat[x][y] = *(int *)data;
}

void clearmat(int mat[sx][sy], int x2, int y2) {
  for (int y = 0; y <= y2; ++y) {
    for (int x = 0; x <= x2; ++x) {
      mat[x][y] = 0;
    }
  }
}

void printmat(int mat[sx][sy], int x2, int y2) {
  for (int y = 0; y <= y2; ++y) {
    for (int x = 0; x <= x2; ++x) {
      switch (mat[x][y]) {
        case 0: std::cout << '.'; break;
        case 1: std::cout << 'o'; break;
        case 2: std::cout << 'n'; break;
        case 3: std::cout << 'X'; break;
        default: std::cout << '?'; break;
      }
    }
    std::cout << std::endl;
  }
}

int main(int argc, char **argv) {
  int a = 1;

  x2 = 5;
  y2 = 3;
  if (argc == 3) {
    x2 = atoi(argv[1]); y2 = atoi(argv[2]);
  }

  a = 1;
  for (int x = 0; x < sx; ++x)
    for (int y = 0; y < sy; ++y) {
      clearmat(mat, x, y);
      algo_ellipse(0, 0, x, y, &a, testmat);
    }

  int difference = 0;
  for (int x = 2; x < sx; ++x)
    for (int y = 7; y < sy; ++y) {
      clearmat(mat, x, y);
      int aa = 1;
      algo_ellipse_old(0, 0, x, y, &aa, fillmat);
      int ab = 2;
      algo_ellipse(0, 0, x, y, &ab, fillmat);
      bool ok = true;
      int diff_fine = 0;
      for (int xx = 0; xx <= x; ++xx)
        for (int yy = 0; yy <= y; ++yy) {
          int r = mat[xx][yy];
          if (r == 1 || r == 2) { ok = false; ++diff_fine; }
      }
      if (!ok) {
        ++difference;
        std::cout << x << " " << y << " diff:" << diff_fine/4 << std::endl;
        printmat(mat, (x+1)/2, (y+1)/2);
        std::cout << std::endl;
      }
    }
  std::cout << "TOTAL: " << difference << std::endl;

  /*clearmat(mat, x2, y2);
  a = 1;
  algo_ellipse_old(0, 0, x2, y2, &a, fillmat);
  a = 2;
  algo_ellipse(0, 0, x2, y2, &a, fillmat);
  printmat(mat, x2, y2);*/
}

