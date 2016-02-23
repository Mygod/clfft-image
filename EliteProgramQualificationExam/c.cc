#include <algorithm>
#include <iostream>
#include <string>
using namespace std;
int main() {
  int t;
  cin >> t;
  string str[100];
  while (t--) {
    cin >> str[0];
    auto len = str[0].length();
    for (int i = 1; i < len; ++i) str[i] = str[i - 1].substr(1) + str[i - 1][0];
    sort(&str[0], &str[len]);
    cout << str[0] << endl;
  }
  return 0;
}
