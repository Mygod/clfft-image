#include <cctype>
#include <iostream>

using namespace std;
int main() {
  cin >> noskipws;
  char sequence[10000];
  while (cin) {
    while (cin >> sequence[0] && !isalnum(sequence[0])) ;
    if (!cin) break;
    int i = 1;
    while (cin >> sequence[i] && isalnum(sequence[i++])) ;
    --i;
    int j = 0;
    char c = '0';
    while (cin && isalnum(c) && i > j) {
      cin >> c;
      if (sequence[j] == c) ++j;
    }
    while (cin && isalnum(c)) cin >> c;
    cout << (i == j ? "Yes" : "No") << endl;
  }
  return 0;
}
