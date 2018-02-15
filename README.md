# ellipse-test
Tests two applications of Gresenham's algorithm for drawing ellipses.
* `ellipse.cpp` contains the two algorithms
* `main.cpp` contains the test procedure - making sure the new one doesn't try to write a pixel twice, and reports when the algorithms draw in different places.
* `alg-other.h` contains other functions required by the ellipse algorithms.

Sample output in output-50-by-50. Each block starts with the width/height of the ellipse, the difference count in output, and a visual. `X` indicates where both algorithms drew. `.` where neither. `o` where only the old one, and `n` where only the new one.
