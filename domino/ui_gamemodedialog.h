/********************************************************************************
** Form generated from reading UI file 'gamemodedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GAMEMODEDIALOG_H
#define UI_GAMEMODEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_GameModeDialog
{
public:
    QVBoxLayout *verticalLayout;
    QRadioButton *radioLocal;
    QRadioButton *radioNetwork;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnStart;

    void setupUi(QDialog *GameModeDialog)
    {
        if (GameModeDialog->objectName().isEmpty())
            GameModeDialog->setObjectName("GameModeDialog");
        GameModeDialog->resize(300, 150);
        verticalLayout = new QVBoxLayout(GameModeDialog);
        verticalLayout->setObjectName("verticalLayout");
        radioLocal = new QRadioButton(GameModeDialog);
        radioLocal->setObjectName("radioLocal");
        radioLocal->setChecked(true);

        verticalLayout->addWidget(radioLocal);

        radioNetwork = new QRadioButton(GameModeDialog);
        radioNetwork->setObjectName("radioNetwork");

        verticalLayout->addWidget(radioNetwork);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnCancel = new QPushButton(GameModeDialog);
        btnCancel->setObjectName("btnCancel");

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnStart = new QPushButton(GameModeDialog);
        btnStart->setObjectName("btnStart");

        horizontalLayout->addWidget(btnStart);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(GameModeDialog);
        QObject::connect(btnCancel, &QPushButton::clicked, GameModeDialog, qOverload<>(&QDialog::reject));
        QObject::connect(btnStart, &QPushButton::clicked, GameModeDialog, qOverload<>(&QDialog::accept));

        QMetaObject::connectSlotsByName(GameModeDialog);
    } // setupUi

    void retranslateUi(QDialog *GameModeDialog)
    {
        GameModeDialog->setWindowTitle(QCoreApplication::translate("GameModeDialog", "Dialog", nullptr));
        radioLocal->setText(QCoreApplication::translate("GameModeDialog", "\320\233\320\276\320\272\320\260\320\273\321\214\320\275\320\260\321\217 \320\270\320\263\321\200\320\260", nullptr));
        radioNetwork->setText(QCoreApplication::translate("GameModeDialog", "\320\241\320\265\321\202\320\265\320\262\320\260\321\217 \320\270\320\263\321\200\320\260", nullptr));
        btnCancel->setText(QCoreApplication::translate("GameModeDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        btnStart->setText(QCoreApplication::translate("GameModeDialog", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GameModeDialog: public Ui_GameModeDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GAMEMODEDIALOG_H
