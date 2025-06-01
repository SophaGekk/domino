// human_player.h
#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "player.h"

class HumanPlayer : public Player {
public:
    explicit HumanPlayer(const QString& name);

    std::pair<DominoTile, bool> playTurn(int leftEnd, int rightEnd) override;

private:
    DominoTile selectedTile;
    bool isLeftEnd = false;

};

#endif // HUMAN_PLAYER_H
