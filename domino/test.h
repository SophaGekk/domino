#include <cassert>
#include "gamewindow.h"
#include "domino_game.h"
#include "player.h"
#include <iostream>

// Проверка раздачи костяшек
void runTests_DealTiles() {
    DominoGame game(2, 0, {"Player1", "Player2"});
    game.startNewGame({"Player1", "Player2"});
    assert(game.getPlayers()[0]->getHand().size() == 7);
    assert(game.getPlayers()[1]->getHand().size()  == 7);
}

void runTests_CheckBlockedGame() {
    // Создаем игру с 2 игроками
    DominoGame game(2, 0, {"Player1", "Player2"});

    // Создаем предопределенное состояние "рыба"
    const QVector<DominoTile> boardTiles = {
        DominoTile(0, 1),  // Левая концевая костяшка
        DominoTile(1, 2),
        DominoTile(2, 3),
        DominoTile(3, 6)   // Правая концевая костяшка
    };

    // Руки игроков без совпадений с концевыми значениями (0 и 6)
    const QVector<DominoTile> hands0 ={DominoTile(4, 4), DominoTile(5, 5)};
    const QVector<DominoTile> hands1 ={DominoTile(2, 2), DominoTile(3, 3)};

    // Устанавливаем предопределенное состояние
    game.setBoard(boardTiles);
    game.getPlayers()[0]->setHand(hands0);
    game.getPlayers()[1]->setHand(hands1);
    game.getBazaar()->clear();
    // Проверяем состояние блокировки
    bool isBlocked = game.isGameOver();

    assert(isBlocked);
}


//Проверка пустой доски (начало игры) и начало не с 0
void runTests_EmptyBoard() {
    DominoGame game(2, 0, {"Player1", "Player2"});

    QVector<DominoTile>  boardTiles;
    game.setBoard(boardTiles);

    const QVector<DominoTile> hands0 ={DominoTile(5, 6)};
    const QVector<DominoTile> hands1 ={DominoTile(0, 0)};
    game.getPlayers()[0]->setHand(hands0);
    game.getPlayers()[1]->setHand(hands1);
    game.determineFirstPlayer();
    int first = game.getCurrentPlayerIndex();
    assert(first == 0);

}

void runTests(){
    std::cout << "Valid_DealTiles..." << std::flush;
    runTests_DealTiles();
    std::cout << "OK" << std::endl;

    std::cout << "Valid_CheckBlockedGame..." << std::flush;
    runTests_CheckBlockedGame();
    std::cout << "OK" << std::endl;

    std::cout << "Valid_EmptyBoard..." << std::flush;
    runTests_EmptyBoard();
    std::cout << "OK" << std::endl;

}
