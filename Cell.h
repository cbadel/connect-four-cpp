#ifndef CELL_H
#define CELL_H
namespace ConnectFourGame {
class Cell {
public:
    enum State { EMPTY, USER1, USER2, COMPUTER };
    
    Cell();
    Cell(char c, int r);
    
    void setPosition(char c, int r);
    void setState(State s);
    
    char getCol() const;
    int getRow() const;
    State getState() const;
    
    char toChar() const;

private:
    char col;
    int row;
    State state;
};
}
#endif 