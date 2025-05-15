#include "humanplayer.h"
#include "player.h"
#include "domino_tile.h"


HumanPlayer::HumanPlayer(const QString& name) : Player(name) {}

std::pair<DominoTile, bool> HumanPlayer::playTurn(int leftEnd, int rightEnd) {
    // Предположим, selectedTile уже выбрана игроком через UI
    DominoTile tile = selectedTile;
    bool isLeftEnd = false;

    // Проверка, можно ли разместить домино слева (без переворота)
    if (tile.getRightValue() == leftEnd) {
        isLeftEnd = true;
    }
    // Проверка, можно ли разместить домино справа (без переворота)
    else if (tile.getLeftValue() == rightEnd) {
        isLeftEnd = false;
    }
    // Проверка с переворотом домино
    else if (tile.getLeftValue() == leftEnd) {
        isLeftEnd = true;
    }
    else if (tile.getRightValue() == rightEnd) {
        isLeftEnd = false;
    }
    else {
        // Недопустимый ход (обработка ошибки)
        return {DominoTile(-1, -1), false};
    }

    return {tile, isLeftEnd};
}

