#include <iostream>

typedef void (*AlgoPixel)(int x, int y, void *data);
void algo_ellipse(int x1, int y1, int x2, int y2, void *data, AlgoPixel proc);
void algo_ellipse_old(int x1, int y1, int x2, int y2, void *data, AlgoPixel proc);

const int sx = 50;
const int sy = 50;
int mat[sx][sy] = {0};
int x2; int y2;

// Increase mat[x][y] by *data. Later we add 2 for every point the new algorithm
// fills and 1 for every point the old algorithm fills to compare differences
// between the two behaviours.
void fillmat(int x, int y, void *data) {
  mat[x][y] += *(int *)data;
}

// Set position x, y in the matrix to *data. If already set, fail.
// Tests to ensure that no pixel is set more than once.
void testmat(int x, int y, void *data) {
  if (mat[x][y] == *(int *)data)
    throw "FAILED";
  mat[x][y] = *(int *)data;
}

// Clear all elements in matrix from [0...x2] by [0...y2]
void clearmat(int mat[sx][sy], int x2, int y2) {
  for (int y = 0; y <= y2; ++y) {
    for (int x = 0; x <= x2; ++x) {
      mat[x][y] = 0;
    }
  }
}

// Print out the matrix: o (old), n (new), X (both), . (none)
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
  // Try all possible ellipse dimensions up to sx * sy. Ensure no pixel is
  // written to twice.
  for (int x = 0; x < sx; ++x)
    for (int y = 0; y < sy; ++y) {
      clearmat(mat, x, y);
      algo_ellipse(0, 0, x, y, &a, testmat);
    }

  // Try all possible ellipse dimensions up to sx * sy. Report any sizes that
  // cause a different output.
  int difference = 0;
  for (int x = 0; x < sx; ++x)
    for (int y = 0; y < sy; ++y) {
      clearmat(mat, x, y);
      // add 1 to every pixel the old matrix would fill
      a = 1;
      algo_ellipse_old(0, 0, x, y, &a, fillmat);
      // add 2 to every pixel the new matrix would fill
      a = 2;
      algo_ellipse(0, 0, x, y, &a, fillmat);

      // find points that were filled by exactly one of the two algs
      bool ok = true;
      int diff_fine = 0;
      for (int xx = 0; xx <= x; ++xx)
        for (int yy = 0; yy <= y; ++yy) {
          int r = mat[xx][yy];
          if (r == 1 || r == 2) { ok = false; ++diff_fine; }
      }
      // if one was found, report it and display the relevant quarter circle.
      if (!ok) {
        ++difference;
        std::cout << x << " " << y << " diff:" << diff_fine/4 << std::endl;
        printmat(mat, (x+1)/2, (y+1)/2);
        std::cout << std::endl;
      }
    }
  std::cout << "total mismatches: " << difference << std::endl;
}

