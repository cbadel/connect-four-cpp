#ifndef CONNECTFOURPLUSUNDO_H
#define CONNECTFOURPLUSUNDO_H

#include "ConnectFourPlus.h"
#include <vector>
namespace ConnectFourGame {
class ConnectFourPlusUndo : public ConnectFourPlus {
public:
    struct Move { 
        int r, c; 
        Cell::State s; 
        int player;
    };
    
    virtual bool playUser(char column) override;
    virtual bool playComputer() override;
    void undo();
    void undoAll();
    virtual void playGame() override;

private:
    std::vector<Move> moves;
};
}
#endif 