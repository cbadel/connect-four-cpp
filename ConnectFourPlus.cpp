#include "ConnectFourPlus.h"
namespace ConnectFourGame {
    // Bu fonksiyon ayni satir veya ayni sutunda 4 tane var mi kontrol ediyor
bool ConnectFourPlus::checkWin(int r, int c) const {
    Cell::State t = board[r][c].getState();
    if (t == Cell::EMPTY) return false;

    int cnt = 1;
    // saga-sola bakiyorum
    for (int cc = c - 1; cc >= 0 && board[r][cc].getState() == t; cc--) cnt++;
    for (int cc = c + 1; cc < width && board[r][cc].getState() == t; cc++) cnt++;
    if (cnt >= 4) return true;

    cnt = 1;
    // yukari-asagi bakiyorum
    for (int rr = r - 1; rr >= 0 && board[rr][c].getState() == t; rr--) cnt++;
    for (int rr = r + 1; rr < height && board[rr][c].getState() == t; rr++) cnt++;
    return cnt >= 4;
}
//kullanici hamlesi
bool ConnectFourPlus::playUser(char column) {
    return ConnectFourAbstract::playUser(column);
}
//bilgisayar hamlesi
bool ConnectFourPlus::playComputer() {
    return ConnectFourAbstract::playComputer();
}
//oyunu baslatan fonksiyon
void ConnectFourPlus::playGame() {
    ConnectFourAbstract::playGame();
}
}
