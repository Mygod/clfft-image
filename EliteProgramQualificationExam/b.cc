#include <iostream>
using namespace std;
int main() {
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    int m = n - 60;
    if (m < 1) m = 1;
    while (m <= n) {
      int i = m, s = m;
      do s += i % 10; while (i /= 10);
      if (s == n) {
        cout << m << endl;
        break;
      } else ++m;
    }
    if (m > n) cout << 0 << endl;
  }
  return 0;
}
