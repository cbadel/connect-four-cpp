#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#include "ConnectFourPlus.h"
#include "ConnectFourDiag.h"
#include "ConnectFourPlusUndo.h"
using namespace ConnectFourGame;
int main() {
    srand(time(0));
    
    cout << "========================================\n";
    cout << "    CONNECT FOUR GAME\n";
    cout << "========================================\n\n";
    
    cout << "Select player mode:\n";
    cout << "1) Player 1 vs Player 2\n";
    cout << "2) Player vs Computer\n";
    cout << "Choice: ";
    
    int type;
    cin >> type;//kullanicidan mod
    
    if (type < 1 || type > 2) {
        cout << "Invalid choice. Defaulting to Player vs Computer.\n";
        type = 2;//hataliysa pc ile oyanr
    }
    
    cout << "\nSelect game type:\n";
    cout << "1) ConnectFourPlus (Horizontal/Vertical wins)\n";
    cout << "2) ConnectFourDiag (Diagonal wins only)\n";
    cout << "3) ConnectFourPlusUndo (Plus with undo feature)\n";
    cout << "Choice: ";
    
    int gameType;
    cin >> gameType;
    
    if (gameType == 1) {
        ConnectFourPlus game;
        game.setGameType(type);//user vs comp ayari
        game.playGame();//oyunu baslat
    }
    else if (gameType == 2) {
        ConnectFourDiag game;
        game.setGameType(type);
        game.playGame();
    }
    else if (gameType == 3) {
        ConnectFourPlusUndo game;
        game.setGameType(type);
        game.playGame();
    }
    else {
        cout << "Invalid choice!\n";
    }
    
    return 0;
}