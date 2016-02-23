#include <cmath>
#include <iostream>

using namespace std;

int main() {
  int n;
  cin >> n;
  while (n) {
    double min = 1.0 / 0.0;
    while (n--) {
      int v, t;
      cin >> v >> t;
      double time = 4500.0 / v + t;
      if (time < min) min = time;
    }
    cout << ceil(min) << endl;
  }
  return 0;
}
