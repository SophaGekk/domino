#include "bazaar.h"
#include "domino_tile.h"
#include <algorithm>
#include <random>
#include <chrono>

Bazaar::Bazaar() {
    initialize();
}

void Bazaar::initialize() {
    tiles.clear();
    // Создаем все 28 костяшек домино
    for (int i = 0; i <= 6; ++i) {
        for (int j = i; j <= 6; ++j) {
            tiles.append(DominoTile(i, j));
        }
    }
    // Перемешиваем
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 rng(seed);
    std::shuffle(tiles.begin(), tiles.end(), rng);
}

bool Bazaar::isEmpty() const {
    return tiles.empty();
}

DominoTile Bazaar::drawTile() {
    if (isEmpty()) {
        // Можно выбросить исключение или вернуть специальную костяшку
        return DominoTile(-1, -1); // Недействительная костяшка
    }

    // Берем последнюю костяшку (для эффективности)
    DominoTile tile = tiles.last();
    tiles.removeLast();
    return tile;
}

int Bazaar::remainingTilesCount() const {
    return tiles.size();
}

const QVector<DominoTile>& Bazaar::getRemainingTiles() const {
    return tiles;
}

void Bazaar::setTiles(const QVector<DominoTile>& newTiles) {
    tiles = newTiles;
}

void Bazaar::removeTile(const DominoTile& tile) {
    auto it = std::find(tiles.begin(), tiles.end(), tile);
    if (it != tiles.end()) {
        tiles.erase(it);
    }
}

const QVector<DominoTile>& Bazaar::getTiles() const {
    return tiles;
}
