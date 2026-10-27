#include "ConnectFourDiag.h"
namespace ConnectFourGame {
bool ConnectFourDiag::checkWin(int r, int c) const {
    Cell::State t = board[r][c].getState();
    if (t == Cell::EMPTY) return false;

    int cnt = 1;
    for (int rr = r - 1, cc = c - 1; rr >= 0 && cc >= 0 && board[rr][cc].getState() == t; rr--, cc--) cnt++;
    for (int rr = r + 1, cc = c + 1; rr < height && cc < width && board[rr][cc].getState() == t; rr++, cc++) cnt++;
    if (cnt >= 4) return true;

    cnt = 1;
    for (int rr = r - 1, cc = c + 1; rr >= 0 && cc < width && board[rr][cc].getState() == t; rr--, cc++) cnt++;
    for (int rr = r + 1, cc = c - 1; rr < height && cc >= 0 && board[rr][cc].getState() == t; rr++, cc--) cnt++;
    return cnt >= 4;
}
bool ConnectFourDiag::playUser(char column) {
    return ConnectFourAbstract::playUser(column);
}

bool ConnectFourDiag::playComputer() {
    return ConnectFourAbstract::playComputer();
}

void ConnectFourDiag::playGame() {
    ConnectFourAbstract::playGame();
}
}
