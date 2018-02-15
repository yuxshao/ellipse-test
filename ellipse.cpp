// Two ellipse drawing algorithms.
#include "alg-other.h"

/* Additional helper functions for the ellipse-drawing helper function
   below corresponding to routines in Bresenham's algorithm. */
namespace {
  int bresenham_ellipse_error(int rx, int ry, int x, int y) {
    return x*x*ry*ry + y*y*rx*rx - rx*rx*ry*ry;
  }

  // Initialize positions x and y for Bresenham's algorithm
  void bresenham_ellipse_init(int rx, int ry, int *px, int *py) {
    // Start at the fatter pole
    if (rx > ry) { *px = 0; *py = ry; }
    else { *px = rx; *py = 0; }
  }

  // Move to next pixel to draw, according to Bresenham's algorithm
  void bresenham_ellipse_step(int rx, int ry, int *px, int *py) {
    int &x = *px, &y = *py;
    // Move towards the skinnier pole. Having 2 cases isn't needed, but it ensures
    // swapping rx and ry is the same as rotating 90 degrees.
    if (rx > ry) {
      int ex = bresenham_ellipse_error(rx, ry, x, y-1);
      int ey = bresenham_ellipse_error(rx, ry, x+1, y);
      int exy = bresenham_ellipse_error(rx, ry, x+1, y-1);
      if (ex + exy < 0) ++x;
      if (ey + exy > 0) --y;
    }
    else {
      int ex = bresenham_ellipse_error(rx, ry, x, y+1);
      int ey = bresenham_ellipse_error(rx, ry, x-1, y);
      int exy = bresenham_ellipse_error(rx, ry, x-1, y+1);
      if (ex + exy > 0) --x;
      if (ey + exy < 0) ++y;
    }
  }
}

/* Helper function for the ellipse drawing routines. Calculates the
   points of an ellipse which fits onto the rectangle specified by x1,
   y1, x2 and y2, and calls the specified routine for each one. The
   output proc has the same format as for do_line, and if the width or
   height of the ellipse is only 1 or 2 pixels, do_line will be
   called.

   Copyright (C) 2002 by Elias Pschernig (eliaspschernig@aon.at)
   for Allegro 4.x.

   Adapted for ASEPRITE by David A. Capello. */
void algo_ellipse(int x1, int y1, int x2, int y2, void *data, AlgoPixel proc) {
  int mx, my, rx, ry;
  int x, y;
  int mx2, my2;

  mx = (x1 + x2) / 2;
  mx2 = (x1 + x2 + 1) / 2;
  my = (y1 + y2) / 2;
  my2 = (y1 + y2 + 1) / 2;
  rx = ABS(x1 - x2);
  ry = ABS(y1 - y2);

  if (rx == 1) { algo_line(x2, y1, x2, y2, data, proc); rx--; }
  if (rx == 0) { algo_line(x1, y1, x1, y2, data, proc); return; }

  if (ry == 1) { algo_line(x1, y2, x2, y2, data, proc); ry--; }
  if (ry == 0) { algo_line(x1, y1, x2, y1, data, proc); return; }

  rx /= 2;
  ry /= 2;

  /* Draw the 4 poles. */
  proc(mx, my2 + ry, data);
  proc(mx, my - ry, data);
  proc(mx2 + rx, my, data);
  proc(mx - rx, my, data);

  /* For even diameter axis, double the poles. */
  if (mx != mx2) {
    proc(mx2, my2 + ry, data);
    proc(mx2, my - ry, data);
  }

  if (my != my2) {
    proc(mx2 + rx, my2, data);
    proc(mx - rx, my2, data);
  }

  /* Initialize drawing position at a pole. */
  bresenham_ellipse_init(rx, ry, &x, &y);

  for (;;) {
    /* Step to the next pixel to draw. */
    bresenham_ellipse_step(rx, ry, &x, &y);

    /* Edge conditions */
    if (y == 0 && x < rx) ++y; // don't move to horizontal radius except at pole
    if (x == 0 && y < ry) ++x; // don't move to vertical radius except at pole
    if (y <= 0 || x <= 0) break; // stop before pole, since it's already drawn

    /* Process pixel */
    proc(mx2 + x, my - y, data);
    proc(mx - x, my - y, data);
    proc(mx2 + x, my2 + y, data);
    proc(mx - x, my2 + y, data);
  }
}


/* Helper function for the ellipse drawing routines. Calculates the
   points of an ellipse which fits onto the rectangle specified by x1,
   y1, x2 and y2, and calls the specified routine for each one. The
   output proc has the same format as for do_line, and if the width or
   height of the ellipse is only 1 or 2 pixels, do_line will be
   called.

   Copyright (C) 2002 by Elias Pschernig (eliaspschernig@aon.at)
   for Allegro 4.x.

   Adapted for ASEPRITE by David A. Capello. */
void algo_ellipse_old(int x1, int y1, int x2, int y2, void *data, AlgoPixel proc)
{
  int mx, my, rx, ry;

  int err;
  int xx, yy;
  int xa, ya;
  int x, y;

  /* Cheap hack to get elllipses with integer diameter, by just offsetting
   * some quadrants by one pixel. */
  int mx2, my2;

  mx = (x1 + x2) / 2;
  mx2 = (x1 + x2 + 1) / 2;
  my = (y1 + y2) / 2;
  my2 = (y1 + y2 + 1) / 2;
  rx = ABS(x1 - x2);
  ry = ABS(y1 - y2);

  if (rx == 1) { algo_line(x2, y1, x2, y2, data, proc); rx--; }
  if (rx == 0) { algo_line(x1, y1, x1, y2, data, proc); return; }

  if (ry == 1) { algo_line(x1, y2, x2, y2, data, proc); ry--; }
  if (ry == 0) { algo_line(x1, y1, x2, y1, data, proc); return; }

  rx /= 2;
  ry /= 2;

  /* Draw the 4 poles. */
  proc(mx, my2 + ry, data);
  proc(mx, my - ry, data);
  proc(mx2 + rx, my, data);
  proc(mx - rx, my, data);

  /* For even diameter axis, double the poles. */
  if (mx != mx2) {
    proc(mx2, my2 + ry, data);
    proc(mx2, my - ry, data);
  }

  if (my != my2) {
    proc(mx2 + rx, my2, data);
    proc(mx - rx, my2, data);
  }

  xx = rx * rx;
  yy = ry * ry;

  /* Do the 'x direction' part of the arc. */

  x = 0;
  y = ry;
  xa = 0;
  ya = xx * 2 * ry;
  err = xx / 4 - xx * ry;

  for (;;) {
    err += xa + yy;
    if (err >= 0) {
      ya -= xx * 2;
      err -= ya;
      y--;
    }
    xa += yy * 2;
    x++;
    if (xa >= ya)
      break;

    proc(mx2 + x, my - y, data);
    proc(mx - x, my - y, data);
    proc(mx2 + x, my2 + y, data);
    proc(mx - x, my2 + y, data);
  }

  /* Fill in missing pixels for very thin ellipses. (This is caused because
   * we always take 1-pixel steps above, and thus might jump past the actual
   * ellipse line.)
   */
  if (y == 0)
    while (x < rx) {
      proc(mx2 + x, my - 1, data);
      proc(mx2 + x, my2 + 1, data);
      proc(mx - x, my - 1, data);
      proc(mx - x, my2 + 1, data);
      x++;
    }

  /* Do the 'y direction' part of the arc. */

  x = rx;
  y = 0;
  xa = yy * 2 * rx;
  ya = 0;
  err = yy / 4 - yy * rx;

  for (;;) {
    err += ya + xx;
    if (err >= 0) {
      xa -= yy * 2;
      err -= xa;
      x--;
    }
    ya += xx * 2;
    y++;
    if (ya > xa)
      break;
    proc(mx2 + x, my - y, data);
    proc(mx - x, my - y, data);
    proc(mx2 + x, my2 + y, data);
    proc(mx - x, my2 + y, data);
  }

  /* See comment above. */
  if (x == 0)
    while (y < ry) {
      proc(mx - 1, my - y, data);
      proc(mx2 + 1, my - y, data);
      proc(mx - 1, my2 + y, data);
      proc(mx2 + 1, my2 + y, data);
      y++;
    }
}
