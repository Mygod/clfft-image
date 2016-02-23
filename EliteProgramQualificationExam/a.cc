#include <cctype>
#include <iostream>

using namespace std;

int main() {
  int i, j;
  while (cin >> i >> j) {
    int down = i, up = j;
    if (i > j) swap(up, down);
    int max = 1;
    for (int k = down; k <= up; ++k) {
      int x = k, count = 1;
      while (x > 1) {
        if (x & 1) x = 3 * x + 1; else x >>= 1;
        ++count;
      }
      if (count > max) max = count;
    }
    cout << i << ' ' << j << ' ' << max << endl;
  }
  return 0;
}
