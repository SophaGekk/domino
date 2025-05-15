#ifndef DOMINOTILE_H
#define DOMINOTILE_H
#include <QObject>


class DominoTile {
public:
    DominoTile(int left=0, int right=0) : leftValue(left), rightValue(right) {};

    int getLeftValue() const { return leftValue; }
    int getRightValue() const { return rightValue; }
    bool isDouble() const { return leftValue == rightValue; }
    int getTotalValue() const { return leftValue + rightValue; }

    void rotate() { std::swap(leftValue, rightValue); }

    bool operator==(const DominoTile& other) const;

    friend QDataStream& operator<<(QDataStream& out, const DominoTile& tile);
    friend QDataStream& operator>>(QDataStream& in, DominoTile& tile);
    void flip(); // Метод для переворота домино

    bool isValidNumer() const {
        return (leftValue >= 0 && leftValue <= 6) && (rightValue >= 0 && rightValue <= 6);
    }



private:
    int leftValue;
    int rightValue;

};

#endif // DOMINO_TILE_H
