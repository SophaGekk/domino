/********************************************************************************
** Form generated from reading UI file 'networkdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NETWORKDIALOG_H
#define UI_NETWORKDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_NetworkDialog
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QRadioButton *radioHost;
    QRadioButton *radioClient;
    QGroupBox *groupBox_2;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *editPlayerName;
    QLabel *label_2;
    QLineEdit *editHost;
    QLabel *label_3;
    QLineEdit *editPort;
    QLabel *labelPlayers;
    QSpinBox *spinPlayers;
    QLabel *labelCode;
    QLineEdit *editCode;
    QHBoxLayout *horizontalLayout;
    QPushButton *btnCancel;
    QSpacerItem *horizontalSpacer;
    QPushButton *btnConnect;

    void setupUi(QDialog *NetworkDialog)
    {
        if (NetworkDialog->objectName().isEmpty())
            NetworkDialog->setObjectName("NetworkDialog");
        NetworkDialog->resize(400, 368);
        verticalLayout = new QVBoxLayout(NetworkDialog);
        verticalLayout->setObjectName("verticalLayout");
        groupBox = new QGroupBox(NetworkDialog);
        groupBox->setObjectName("groupBox");
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName("verticalLayout_2");
        radioHost = new QRadioButton(groupBox);
        radioHost->setObjectName("radioHost");
        radioHost->setChecked(true);

        verticalLayout_2->addWidget(radioHost);

        radioClient = new QRadioButton(groupBox);
        radioClient->setObjectName("radioClient");

        verticalLayout_2->addWidget(radioClient);


        verticalLayout->addWidget(groupBox);

        groupBox_2 = new QGroupBox(NetworkDialog);
        groupBox_2->setObjectName("groupBox_2");
        formLayout = new QFormLayout(groupBox_2);
        formLayout->setObjectName("formLayout");
        label = new QLabel(groupBox_2);
        label->setObjectName("label");

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        editPlayerName = new QLineEdit(groupBox_2);
        editPlayerName->setObjectName("editPlayerName");

        formLayout->setWidget(0, QFormLayout::FieldRole, editPlayerName);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName("label_2");

        formLayout->setWidget(1, QFormLayout::LabelRole, label_2);

        editHost = new QLineEdit(groupBox_2);
        editHost->setObjectName("editHost");

        formLayout->setWidget(1, QFormLayout::FieldRole, editHost);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName("label_3");

        formLayout->setWidget(2, QFormLayout::LabelRole, label_3);

        editPort = new QLineEdit(groupBox_2);
        editPort->setObjectName("editPort");

        formLayout->setWidget(2, QFormLayout::FieldRole, editPort);

        labelPlayers = new QLabel(groupBox_2);
        labelPlayers->setObjectName("labelPlayers");

        formLayout->setWidget(3, QFormLayout::LabelRole, labelPlayers);

        spinPlayers = new QSpinBox(groupBox_2);
        spinPlayers->setObjectName("spinPlayers");
        spinPlayers->setMinimum(2);
        spinPlayers->setMaximum(4);
        spinPlayers->setValue(2);

        formLayout->setWidget(3, QFormLayout::FieldRole, spinPlayers);

        labelCode = new QLabel(groupBox_2);
        labelCode->setObjectName("labelCode");

        formLayout->setWidget(4, QFormLayout::LabelRole, labelCode);

        editCode = new QLineEdit(groupBox_2);
        editCode->setObjectName("editCode");

        formLayout->setWidget(4, QFormLayout::FieldRole, editCode);


        verticalLayout->addWidget(groupBox_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        btnCancel = new QPushButton(NetworkDialog);
        btnCancel->setObjectName("btnCancel");

        horizontalLayout->addWidget(btnCancel);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        btnConnect = new QPushButton(NetworkDialog);
        btnConnect->setObjectName("btnConnect");

        horizontalLayout->addWidget(btnConnect);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(NetworkDialog);
        QObject::connect(btnCancel, &QPushButton::clicked, NetworkDialog, qOverload<>(&QDialog::reject));
        QObject::connect(btnConnect, &QPushButton::clicked, NetworkDialog, qOverload<>(&QDialog::accept));

        QMetaObject::connectSlotsByName(NetworkDialog);
    } // setupUi

    void retranslateUi(QDialog *NetworkDialog)
    {
        NetworkDialog->setWindowTitle(QCoreApplication::translate("NetworkDialog", "Dialog", nullptr));
        groupBox->setTitle(QCoreApplication::translate("NetworkDialog", "\320\242\320\270\320\277 \320\277\320\276\320\264\320\272\320\273\321\216\321\207\320\265\320\275\320\270\321\217", nullptr));
        radioHost->setText(QCoreApplication::translate("NetworkDialog", "\320\241\320\276\320\267\320\264\320\260\321\202\321\214 \321\201\320\265\321\200\320\262\320\265\321\200 (\320\245\320\276\321\201\321\202)", nullptr));
        radioClient->setText(QCoreApplication::translate("NetworkDialog", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217 \320\272 \321\201\320\265\321\200\320\262\320\265\321\200\321\203", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("NetworkDialog", "\320\237\320\260\321\200\320\260\320\274\320\265\321\202\321\200\321\213", nullptr));
        label->setText(QCoreApplication::translate("NetworkDialog", "\320\222\320\260\321\210\320\265 \320\270\320\274\321\217:", nullptr));
        label_2->setText(QCoreApplication::translate("NetworkDialog", "\320\220\320\264\321\200\320\265\321\201 \321\201\320\265\321\200\320\262\320\265\321\200\320\260:", nullptr));
        label_3->setText(QCoreApplication::translate("NetworkDialog", "\320\237\320\276\321\200\321\202:", nullptr));
        labelPlayers->setText(QCoreApplication::translate("NetworkDialog", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\270\320\263\321\200\320\276\320\272\320\276\320\262:", nullptr));
        labelCode->setText(QCoreApplication::translate("NetworkDialog", "\320\232\320\276\320\264 \321\201\320\265\321\201\321\201\320\270\320\270:", nullptr));
        btnCancel->setText(QCoreApplication::translate("NetworkDialog", "\320\236\321\202\320\274\320\265\320\275\320\260", nullptr));
        btnConnect->setText(QCoreApplication::translate("NetworkDialog", "\320\237\321\200\320\270\321\201\320\276\320\265\320\264\320\270\320\275\320\270\321\202\321\214\321\201\321\217", nullptr));
    } // retranslateUi

};

namespace Ui {
    class NetworkDialog: public Ui_NetworkDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NETWORKDIALOG_H
