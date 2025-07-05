#include "player.h"
#include "bazaar.h"
#include "bot_player.h"
#include "domino_tile.h"

BotPlayer::BotPlayer(const QString& name) : Player(name) {}

std::pair<DominoTile, bool> BotPlayer::playTurn(int leftEnd, int rightEnd) {
    if(leftEnd == -1)
    {
        // Поиск дублей с максимальным значением
        DominoTile maxDouble(-1, -1);
        for (const DominoTile& tile : hand) {
            if (tile.isDouble() && tile.getLeftValue() > maxDouble.getLeftValue()) {
                maxDouble = tile;
            }
        }

        // Если дубль найден
        if (maxDouble.isValidNumer()) {
            return {maxDouble, true}; // Направление не важно для дубля
        }

        // Если дублей нет - берем максимальную костяшку
        DominoTile maxTile(-1, -1);
        for (const DominoTile& tile : hand) {
            if (tile.getTotalValue() > maxTile.getTotalValue()) {
                maxTile = tile;
            }
        }
        return {maxTile, true}; // Направление можно выбрать любое
    }
    else
    {
        QVector<std::tuple<DominoTile, bool, int>> possibleMoves;

        // Проверяем все возможные варианты размещения
        for (const DominoTile& tile : hand) {
            // Вариант 1: размещение слева (левое значение tile совпадает с leftEnd)
            if (tile.getLeftValue() == leftEnd || tile.getRightValue() == leftEnd) {
                bool needFlip = (tile.getRightValue() == leftEnd);
                possibleMoves.append({tile, true, tile.getTotalValue()});
            }

            // Вариант 2: размещение справа (правое значение tile совпадает с rightEnd)
            if (tile.getLeftValue() == rightEnd || tile.getRightValue() == rightEnd) {
                bool needFlip = (tile.getLeftValue() == rightEnd);
                possibleMoves.append({tile, false, tile.getTotalValue()});
            }
        }

        if (possibleMoves.isEmpty()) {
            return {DominoTile(-1, -1), false}; // Нет допустимых ходов
        }

        // Выбираем ход с максимальной суммой очков
        auto bestMove = *std::max_element(possibleMoves.begin(), possibleMoves.end(),
                                          [](const auto& a, const auto& b) {
                                              return std::get<2>(a) < std::get<2>(b);
                                          }
                                          );

        // Возвращаем костяшку и направление
        return {std::get<0>(bestMove), std::get<1>(bestMove)};
    }
}
