#ifndef CONNECTFOURPLUS_H
#define CONNECTFOURPLUS_H

#include "ConnectFourAbstract.h"

namespace ConnectFourGame {
class ConnectFourPlus : public ConnectFourAbstract {
public:
    virtual bool playUser(char column) override;
    virtual bool playComputer() override;
    virtual void playGame() override;
    bool checkWin(int r, int c) const override;
};
}
#endif // CONNECTFOURPLUS_H