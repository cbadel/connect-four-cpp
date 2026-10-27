#ifndef CONNECTFOURABSTRACT_H
#define CONNECTFOURABSTRACT_H

#include <string>
#include "Cell.h"
namespace ConnectFourGame {
// Oyun modu enum'u
enum GameMode { MODE_USER_VS_COMPUTER, MODE_USER1_VS_USER2 };

class ConnectFourAbstract {
public:
    ConnectFourAbstract();
    virtual ~ConnectFourAbstract();

    void readFromFile(const std::string& filename);
    void writeToFile(const std::string& filename) const;

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void displayBoard() const;

    virtual bool playUser(char column);
    virtual bool playComputer();
    
    bool isGameFinished() const;
    bool isDraw() const { return draw; }

    virtual void playGame();
    void setGameType(int t);
    
    std::string getCurrentPlayerName() const;
    void switchPlayer();

    virtual bool checkWin(int r, int c) const = 0;

protected:
    int width, height;
    GameMode gameMode;
    int currentPlayer;
    bool gameOver;
    bool draw;
    Cell** board;
    
    bool isValidColumn(char column) const;
    int getColumnIndex(char column) const;
    int findEmptyRow(int col) const;
};
}
#endif 