#ifndef CONNECTFOURDIAG_H
#define CONNECTFOURDIAG_H

#include "ConnectFourAbstract.h"
namespace ConnectFourGame {
class ConnectFourDiag : public ConnectFourAbstract {
public:
    virtual bool playUser(char column) override;
    virtual bool playComputer() override;
    virtual void playGame() override;
    bool checkWin(int r, int c) const override;
};
}
#endif 