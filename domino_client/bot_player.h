#ifndef BOT_PLAYER_H
#define BOT_PLAYER_H

#include "player.h"

class BotPlayer : public Player {
public:
    explicit BotPlayer(const QString& name);

    std::pair<DominoTile, bool> playTurn(int leftEnd, int rightEnd) override;
    bool isBot() const override { return true; }
};


#endif // BOT_PLAYER_H
