#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include "domino_game.h"
#include "dominolabel.h"
#include "bazaaroverlay.h"
#include "ClickableLabel.h"
#include <QGraphicsEffect>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include "chatmanager.h"

class Client;
namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget {
    Q_OBJECT

public:
    explicit GameWindow(int playersCount, int BotCount, bool showReserve, const QStringList &playerNames, DominoGame* existingGame, QWidget *parent = nullptr);
    void setHighlightEnabled(bool highlight);
    void setPlayersCount(int count);
    void setBotCount(int count);
    void updateHandDisplay();
    void updateReserveLabel(bool showReserve);
    void setupHands();
    void handleDominoClick(DominoLabel* clickedLabel);
    void showPossibleMoves(const DominoTile& tile);
    void processMove(DominoTile tile);
    DominoTile getTileFromLabel(DominoLabel* label);
    void updateBoard();
    void onFirstMoveClicked();
    DominoTile getSelectedTile() const {
        return selectedTile;
    }

    void showFinalGameOver();
    void handleHighlightClick(bool isLeftEnd);
    void updateHighlightedAreas();
    QPoint getTilePosition(const DominoTile& tile) const;
    void handleBazaarTileSelected(const DominoTile& tile);
    void showGameOver();
    bool isGameOver() const
    {
        return game->isGameOver();
    };
    void onPlayerChanged(int index);
    void showTurnOverlay(const QString& name);
    // Обработка нажатия пробела
    void keyPressEvent(QKeyEvent* event);
    void clearHighlights();
    void clearSelection();
    ~GameWindow();
    void setIsNewGame(bool state) { isNewGame = state; }

    void onLeftEndClicked();
    void onRightEndClicked();
    void writeGameStats();
    DominoGame *game;

    void sendChatMessage();

    void highlightValidTiles();
    void handleNoValidMoves();

    void setReserveVisible(bool visible) {
        showReserve = visible;
    }

    void updateFromJson(const QJsonObject& state);
    void setClient(Client* server);
    void setClientPlayerName(const QString& name) { clientPlayerName = name; }
    void loadGameState(const QJsonObject& state);

signals:
    void returnToMainMenu();
    void requestBazaar();
    void requestSkip();
    void sendNetworkChat(const QString& message);

public slots:
    void updateGameState(); // Обновление интерфейса
    void enableSpaceKey() { m_spaceKeyEnabled = true; }
    void disableSpaceKey() { m_spaceKeyEnabled = false; }

private slots:
    void updateChatHistory(const QString& formattedMessage);
    void onHomeClicked();   // Обработка кнопки "Домой"
    void onReserveClicked();
    void updateUI();
    void on_pushButton_skip_clicked();

private:
    bool isNewGame;

    void clearBoard();      // Очистка поля

    Ui::GameWindow *ui;
    QGraphicsScene *scene;  // Графическая сцена для dominoArea
           // Логика игры
    int playersCount;
    int BotCount;

    bool isReserveVisible;
    bool isHighlightEnabled;
    int currentPlayersCount;
    int currentStartTiles;

    QVector<DominoLabel*> topHandLabels;  // Элементы верхнего игрока (задняя сторона)
    QVector<DominoLabel*> bottomHandLabels; // Элементы нижнего игрока (фронтальная сторона)
    QVector<DominoLabel*> leftHandLabels;    // Левый игрок (вертикальный)
    QVector<DominoLabel*> rightHandLabels;   // Правый игрок (вертикальный)

    QLabel *reserveLabel;

    QPoint leftEndPosition = QPoint(350, 200); // Пример координат
    QPoint rightEndPosition = QPoint(390, 200);
    void drawArrow(const QPoint& pos, bool isLeft, const DominoTile& tile);
    bool isLeftEnd;

    DominoTile selectedTile;
    BazaarOverlay* m_bazaarOverlay;

    ClickableLabel* labelReserve ;
    QPushButton* reserveButton;

    QGraphicsOpacityEffect* m_parentEffect;
    QWidget* m_darkOverlay; // Виджет для затемнения

    QWidget* m_turnOverlay; // Виджет для смены хода
    QLabel* m_turnLabel;    // Надпись "Теперь ход игрока..."

    ChatManager* chatManager;
    QTextEdit* chatHistoryWidget;
    QLineEdit* messageInput;
    QPushButton* sendButton;

    bool showReserve = true;
    const QStringList &playerNames;
    QList<QGraphicsItem*> highlightItems; // Храним элементы подсветки

    bool isGameOverShown = false;
    bool gameOverShown = false;
    bool m_spaceKeyEnabled;
    bool isHandleNoValidMoves = false;
    bool isValidMessage = false;

    bool isNetworkGame = false;
    Client* сlient = nullptr;
    DominoGame* existingGame = nullptr;

    QString clientPlayerName;
    int clientPlayerIndex = -1; // Позиция клиента среди игроков
    QVector<int> playerPositions; // Распределение позиций на экране

    void determinePlayerPositions();
    void updateHandForPosition(int playerIndex, int screenPosition);
};

#endif // GAMEWINDOW_H
