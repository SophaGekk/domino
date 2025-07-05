/********************************************************************************
** Form generated from reading UI file 'gamewindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEWINDOW_H
#define UI_GAMEWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameWindow
{
public:
    QFrame *gameField;
    QScrollArea *scrollDominoArea;
    QGraphicsView *dominoArea;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayoutTopHand;
    QScrollArea *scrollBottomHand;
    QWidget *horizontalLayoutWidget_2;
    QHBoxLayout *horizontalLayoutBottomHand;
    QLabel *avatarTop;
    QLabel *labelNameTop;
    QLabel *labelScoreTop;
    QLabel *labelNameBottom;
    QLabel *avatarBottom;
    QLabel *labelScoreBottom;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayoutRightHand;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayoutLeftHand;
    QLabel *labelNameLeft;
    QLabel *labelNameRight;
    QLabel *labelScoreRight;
    QLabel *labelScoreLeft;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *rightPanelLayout;
    QPushButton *pushButton_skip;
    QLabel *labelReserve;
    QPushButton *homeButton;
    QLabel *labelRound;
    QWidget *verticalLayoutWidget_4;
    QVBoxLayout *labelReserveLayout;

    void setupUi(QWidget *GameWindow)
    {
        if (GameWindow->objectName().isEmpty())
            GameWindow->setObjectName("GameWindow");
        GameWindow->resize(800, 600);
        GameWindow->setStyleSheet(QString::fromUtf8("\n"
"        QWidget { background: #86ab71; }\n"
"        QFrame#gameField { background: #b4c28a; border-radius: 12px; border: 2px solid #b2c28a; }\n"
"        QLabel { color: #222; background: transparent; }\n"
"        QLabel#labelRound, QLabel#labelReserve { font-size: 20px; font-weight: bold; }\n"
"        QLabel#labelNameTop, QLabel#labelNameBottom { font-size: 16px; }\n"
"        QLabel#labelScoreTop, QLabel#labelScoreBottom { font-size: 14px; }\n"
"      "));
        gameField = new QFrame(GameWindow);
        gameField->setObjectName("gameField");
        gameField->setGeometry(QRect(0, 50, 804, 561));
        gameField->setStyleSheet(QString::fromUtf8("background: #b4c28a;"));
        scrollDominoArea = new QScrollArea(gameField);
        scrollDominoArea->setObjectName("scrollDominoArea");
        scrollDominoArea->setGeometry(QRect(59, 70, 671, 351));
        scrollDominoArea->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        scrollDominoArea->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        scrollDominoArea->setWidgetResizable(true);
        dominoArea = new QGraphicsView();
        dominoArea->setObjectName("dominoArea");
        dominoArea->setGeometry(QRect(0, 0, 669, 349));
        dominoArea->setMinimumSize(QSize(200, 200));
        dominoArea->setStyleSheet(QString::fromUtf8("QWidget { background: #b2cf7c; border: 2px solid #666; border-radius: 12px;}"));
        scrollDominoArea->setWidget(dominoArea);
        horizontalLayoutWidget = new QWidget(gameField);
        horizontalLayoutWidget->setObjectName("horizontalLayoutWidget");
        horizontalLayoutWidget->setGeometry(QRect(270, 0, 291, 61));
        horizontalLayoutTopHand = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayoutTopHand->setObjectName("horizontalLayoutTopHand");
        horizontalLayoutTopHand->setContentsMargins(0, 0, 0, 0);
        scrollBottomHand = new QScrollArea(gameField);
        scrollBottomHand->setObjectName("scrollBottomHand");
        scrollBottomHand->setGeometry(QRect(230, 450, 341, 100));
        scrollBottomHand->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        scrollBottomHand->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
        scrollBottomHand->setWidgetResizable(true);
        horizontalLayoutWidget_2 = new QWidget();
        horizontalLayoutWidget_2->setObjectName("horizontalLayoutWidget_2");
        horizontalLayoutWidget_2->setGeometry(QRect(0, 0, 339, 81));
        horizontalLayoutBottomHand = new QHBoxLayout(horizontalLayoutWidget_2);
        horizontalLayoutBottomHand->setObjectName("horizontalLayoutBottomHand");
        scrollBottomHand->setWidget(horizontalLayoutWidget_2);
        avatarTop = new QLabel(gameField);
        avatarTop->setObjectName("avatarTop");
        avatarTop->setGeometry(QRect(140, 10, 48, 48));
        avatarTop->setPixmap(QPixmap(QString::fromUtf8(":/icon/man1.png")));
        avatarTop->setScaledContents(true);
        labelNameTop = new QLabel(gameField);
        labelNameTop->setObjectName("labelNameTop");
        labelNameTop->setGeometry(QRect(200, 10, 61, 21));
        labelScoreTop = new QLabel(gameField);
        labelScoreTop->setObjectName("labelScoreTop");
        labelScoreTop->setGeometry(QRect(200, 30, 21, 31));
        labelNameBottom = new QLabel(gameField);
        labelNameBottom->setObjectName("labelNameBottom");
        labelNameBottom->setGeometry(QRect(629, 426, 61, 31));
        avatarBottom = new QLabel(gameField);
        avatarBottom->setObjectName("avatarBottom");
        avatarBottom->setGeometry(QRect(710, 430, 48, 48));
        avatarBottom->setPixmap(QPixmap(QString::fromUtf8(":/icon/girl1.png")));
        avatarBottom->setScaledContents(true);
        labelScoreBottom = new QLabel(gameField);
        labelScoreBottom->setObjectName("labelScoreBottom");
        labelScoreBottom->setGeometry(QRect(670, 450, 20, 21));
        verticalLayoutWidget = new QWidget(gameField);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(750, 120, 51, 231));
        verticalLayoutRightHand = new QVBoxLayout(verticalLayoutWidget);
        verticalLayoutRightHand->setObjectName("verticalLayoutRightHand");
        verticalLayoutRightHand->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_2 = new QWidget(gameField);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(0, 120, 51, 221));
        verticalLayoutLeftHand = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayoutLeftHand->setObjectName("verticalLayoutLeftHand");
        verticalLayoutLeftHand->setContentsMargins(0, 0, 0, 0);
        labelNameLeft = new QLabel(gameField);
        labelNameLeft->setObjectName("labelNameLeft");
        labelNameLeft->setGeometry(QRect(0, 50, 61, 21));
        labelNameRight = new QLabel(gameField);
        labelNameRight->setObjectName("labelNameRight");
        labelNameRight->setGeometry(QRect(730, 50, 63, 20));
        labelScoreRight = new QLabel(gameField);
        labelScoreRight->setObjectName("labelScoreRight");
        labelScoreRight->setGeometry(QRect(770, 80, 21, 31));
        labelScoreLeft = new QLabel(gameField);
        labelScoreLeft->setObjectName("labelScoreLeft");
        labelScoreLeft->setGeometry(QRect(10, 80, 21, 31));
        verticalLayoutWidget_3 = new QWidget(gameField);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(0, 410, 171, 141));
        rightPanelLayout = new QVBoxLayout(verticalLayoutWidget_3);
        rightPanelLayout->setObjectName("rightPanelLayout");
        rightPanelLayout->setContentsMargins(0, 0, 0, 0);
        pushButton_skip = new QPushButton(gameField);
        pushButton_skip->setObjectName("pushButton_skip");
        pushButton_skip->setEnabled(true);
        pushButton_skip->setGeometry(QRect(640, 500, 111, 29));
        pushButton_skip->setCheckable(false);
        labelReserve = new QLabel(GameWindow);
        labelReserve->setObjectName("labelReserve");
        labelReserve->setGeometry(QRect(670, 10, 102, 27));
        homeButton = new QPushButton(GameWindow);
        homeButton->setObjectName("homeButton");
        homeButton->setGeometry(QRect(30, 8, 41, 37));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        homeButton->setIcon(icon);
        homeButton->setIconSize(QSize(50, 50));
        homeButton->setFlat(true);
        labelRound = new QLabel(GameWindow);
        labelRound->setObjectName("labelRound");
        labelRound->setGeometry(QRect(360, 10, 80, 27));
        verticalLayoutWidget_4 = new QWidget(GameWindow);
        verticalLayoutWidget_4->setObjectName("verticalLayoutWidget_4");
        verticalLayoutWidget_4->setGeometry(QRect(669, 10, 121, 31));
        labelReserveLayout = new QVBoxLayout(verticalLayoutWidget_4);
        labelReserveLayout->setObjectName("labelReserveLayout");
        labelReserveLayout->setContentsMargins(0, 0, 0, 0);
        labelReserve->raise();
        gameField->raise();
        homeButton->raise();
        labelRound->raise();
        verticalLayoutWidget_4->raise();

        retranslateUi(GameWindow);

        QMetaObject::connectSlotsByName(GameWindow);
    } // setupUi

    void retranslateUi(QWidget *GameWindow)
    {
        GameWindow->setWindowTitle(QCoreApplication::translate("GameWindow", "\320\224\320\276\320\274\320\270\320\275\320\276 - \320\230\320\263\321\200\320\260", nullptr));
        labelNameTop->setText(QCoreApplication::translate("GameWindow", "\320\236\320\273\320\265\320\263", nullptr));
        labelScoreTop->setText(QCoreApplication::translate("GameWindow", "21", nullptr));
        labelNameBottom->setText(QCoreApplication::translate("GameWindow", "\320\222\321\213", nullptr));
        labelScoreBottom->setText(QCoreApplication::translate("GameWindow", "17", nullptr));
        labelNameLeft->setText(QString());
        labelNameRight->setText(QString());
        labelScoreRight->setText(QString());
        labelScoreLeft->setText(QString());
        pushButton_skip->setText(QCoreApplication::translate("GameWindow", "\320\237\321\200\320\276\320\277\321\203\321\201\320\272 \321\205\320\276\320\264\320\260", nullptr));
        labelReserve->setText(QCoreApplication::translate("GameWindow", "\320\240\320\265\320\267\320\265\321\200\320\262: 10", nullptr));
        labelRound->setText(QCoreApplication::translate("GameWindow", "\320\240\320\260\321\203\320\275\320\264: 1", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameWindow: public Ui_GameWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEWINDOW_H
