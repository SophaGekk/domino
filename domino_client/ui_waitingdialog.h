/********************************************************************************
** Form generated from reading UI file 'waitingdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITINGDIALOG_H
#define UI_WAITINGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_WaitingDialog
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lblStatus;
    QLabel *lblCode;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listPlayers;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnCancel;

    void setupUi(QDialog *WaitingDialog)
    {
        if (WaitingDialog->objectName().isEmpty())
            WaitingDialog->setObjectName("WaitingDialog");
        WaitingDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(WaitingDialog);
        verticalLayout->setObjectName("verticalLayout");
        lblStatus = new QLabel(WaitingDialog);
        lblStatus->setObjectName("lblStatus");
        lblStatus->setAlignment(Qt::AlignCenter);
        lblStatus->setWordWrap(true);

        verticalLayout->addWidget(lblStatus);

        lblCode = new QLabel(WaitingDialog);
        lblCode->setObjectName("lblCode");
        lblCode->setAlignment(Qt::AlignCenter);
        lblCode->setVisible(false);

        verticalLayout->addWidget(lblCode);

        groupBox = new QGroupBox(WaitingDialog);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        listPlayers = new QListWidget(groupBox);
        listPlayers->setObjectName("listPlayers");

        verticalLayout_2->addWidget(listPlayers);


        verticalLayout->addWidget(groupBox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnCancel = new QPushButton(WaitingDialog);
        btnCancel->setObjectName("btnCancel");

        horizontalLayout->addWidget(btnCancel);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(WaitingDialog);
        QObject::connect(btnCancel, &QPushButton::clicked, WaitingDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(WaitingDialog);
    } // setupUi

    void retranslateUi(QDialog *WaitingDialog)
    {
        WaitingDialog->setWindowTitle(QCoreApplication::translate("WaitingDialog", "\320\236\320\266\320\270\320\264\320\260\320\275\320\270\320\265 \320\270\320\263\321\200\320\276\320\272\320\276\320\262", nullptr));
        lblStatus->setText(QCoreApplication::translate("WaitingDialog", "\320\236\320\266\320\270\320\264\320\260\320\275\320\270\320\265 \320\270\320\263\321\200\320\276\320\272\320\276\320\262...", nullptr));
        lblCode->setText(QCoreApplication::translate("WaitingDialog", "\320\232\320\276\320\264 \321\201\320\265\321\201\321\201\320\270\320\270: ", nullptr));
        groupBox->setTitle(QCoreApplication::translate("WaitingDialog", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\275\321\213\320\265 \320\270\320\263\321\200\320\276\320\272\320\270", nullptr));
        btnCancel->setText(QCoreApplication::translate("WaitingDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WaitingDialog: public Ui_WaitingDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINGDIALOG_H
