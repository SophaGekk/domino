/********************************************************************************
** Form generated from reading UI file 'gameoverdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEOVERDIALOG_H
#define UI_GAMEOVERDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GameOverDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QScrollArea *scrollPlayers;
    QWidget *playersContainer;
    QVBoxLayout *playersLayout;
    QVBoxLayout *verticalLayout_3;

    void setupUi(QDialog *GameOverDialog)
    {
        if (GameOverDialog->objectName().isEmpty())
            GameOverDialog->setObjectName("GameOverDialog");
        GameOverDialog->resize(266, 375);
        GameOverDialog->setStyleSheet(QString::fromUtf8("\n"
"        QDialog {\n"
"          background: white;\n"
"          border: 2px solid #666;\n"
"          border-radius: 12px;\n"
"        }\n"
"        QLabel#titleLabel {\n"
"          font-size: 24px;\n"
"          font-weight: bold;\n"
"          color: #444;\n"
"        }\n"
"        QScrollArea { border: none; }\n"
"      "));
        verticalLayout = new QVBoxLayout(GameOverDialog);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(GameOverDialog);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(titleLabel);

        scrollPlayers = new QScrollArea(GameOverDialog);
        scrollPlayers->setObjectName("scrollPlayers");
        scrollPlayers->setWidgetResizable(true);
        playersContainer = new QWidget();
        playersContainer->setObjectName("playersContainer");
        playersContainer->setGeometry(QRect(0, 0, 244, 305));
        playersLayout = new QVBoxLayout(playersContainer);
        playersLayout->setObjectName("playersLayout");
        scrollPlayers->setWidget(playersContainer);

        verticalLayout->addWidget(scrollPlayers);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");

        verticalLayout->addLayout(verticalLayout_3);


        retranslateUi(GameOverDialog);

        QMetaObject::connectSlotsByName(GameOverDialog);
    } // setupUi

    void retranslateUi(QDialog *GameOverDialog)
    {
        GameOverDialog->setWindowTitle(QCoreApplication::translate("GameOverDialog", "\320\240\320\265\320\267\321\203\320\273\321\214\321\202\320\260\321\202\321\213 \320\270\320\263\321\200\321\213", nullptr));
        titleLabel->setText(QCoreApplication::translate("GameOverDialog", "\320\230\321\202\320\276\320\263\320\270 \320\270\320\263\321\200\321\213", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameOverDialog: public Ui_GameOverDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEOVERDIALOG_H
