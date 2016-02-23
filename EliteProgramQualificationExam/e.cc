#include <cctype>
#include <iostream>
#include <string>

using namespace std;

string board[5];

inline bool operate(int &i, int &j, int x, int y) {
  if (x < 0 || x > 4 || y < 0 || y > 4) return true;
  swap(board[i][j], board[x][y]);
  i = x;
  j = y;
  return false;
}

int main() {
  int t = 0;
  while (getline(cin, board[0]) && board[0] != "Z") {
    if (t) cout << endl;
    getline(cin, board[1]);
    getline(cin, board[2]);
    getline(cin, board[3]);
    getline(cin, board[4]);
    int i, j = 0;
    for (i = 0; i < 5; ++i) if (board[i].length() == 4) {
        board[i] += ' ';
        j = 4;
        break;
      } else if ((j = static_cast<int>(board[i].find(' '))) >= 0) break;
    char c;
    bool bad = false;
    while (cin >> c && c != '0' && !bad) switch (c) {
        case 'A':
          bad |= operate(i, j, i - 1, j);
          break;
        case 'B':
          bad |= operate(i, j, i + 1, j);
          break;
        case 'R':
          bad |= operate(i, j, i, j + 1);
          break;
        case 'L':
          bad |= operate(i, j, i, j - 1);
          break;
      }
    cout << "Puzzle #" << ++t << ':' << endl;
    if (bad) cout << "This puzzle has no final configuration." << endl;
    else for (int i = 0; i < 5; ++i) cout << board[i][0] << ' ' << board[i][1] << ' ' << board[i][2] << ' '
            << board[i][3] << ' ' << board[i][4] << endl;
    getline(cin, board[0]); // read remaining
  }
  return 0;
}
