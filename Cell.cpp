#include "Cell.h"
namespace ConnectFourGame {
Cell::Cell(): col('A'), row(1), state(EMPTY) {}

Cell::Cell(char c, int r): col(c), row(r), state(EMPTY) {}

void Cell::setPosition(char c, int r) { 
    col = c; 
    row = r; 
}

char Cell::getCol() const { return col; }
int Cell::getRow() const { return row; }

void Cell::setState(State s) { state = s; }
Cell::State Cell::getState() const { return state; }

char Cell::toChar() const {
    switch(state) {
        case EMPTY: return '.';
        case USER1: return 'X';
        case USER2: return 'O';
        case COMPUTER: return 'C';
    }
    return '?';
}
}