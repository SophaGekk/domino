#include "domino_tile.h"

bool DominoTile::operator==(const DominoTile& other) const {
    return (leftValue == other.leftValue && rightValue == other.rightValue) ||
           (leftValue == other.rightValue && rightValue == other.leftValue);
}

QDataStream& operator<<(QDataStream& out, const DominoTile& tile) {
    out << tile.getLeftValue() << tile.getRightValue();
    return out;
}

QDataStream& operator>>(QDataStream& in, DominoTile& tile) {
    int left, right;
    in >> left >> right;
    tile = DominoTile(left, right);
    return in;
}

void DominoTile::flip() {
    std::swap(leftValue, rightValue);
}
