#include "player.h"

Player::Player(const QString& name)
    : name(name), score(0) {  // Инициализация полей
}

int Player::getScore() const {
    return score;
}

void Player::setScore(int newScore) {
    score += newScore;
}

void Player::removeTile(const DominoTile& tile) {
    for (int i = 0; i < hand.size(); ++i) {
        if (hand[i] == tile) {
            hand.remove(i);
            return;
        }
    }
}


DominoTile Player::getSelectedTile() const {
    return m_selectedTile;
}

const QVector<DominoTile>& Player::getHand() const {
    return hand;
}


QString Player::getName() const{
    return name;
}

int Player::getHandSize() const {
    return hand.size();
}

bool Player::hasMatchingTile(int value) const {
    for (const DominoTile& tile : hand) {
        if (tile.getLeftValue() == value || tile.getRightValue() == value) {
            return true;
        }
    }
    return false;
}

void Player::drawTile(Bazaar* bazaar) {
    if (bazaar && !bazaar->isEmpty()) {
        DominoTile newTile = bazaar->drawTile();
        hand.append(newTile);
    }
}




bool Player::hasTile(const DominoTile& tile) const {
    return hand.contains(tile);
}
void Player::setHand(const QVector<DominoTile>& newHand) {
    hand = newHand;
}

bool Player::hasValidMove(int left, int right) const {
    for (const DominoTile& domino : hand) {
        if (domino.getLeftValue() == left || domino.getRightValue() == left ||
            domino.getLeftValue() == right || domino.getRightValue() == right) {
            return true;
        }
    }
    return false;
}
