/********************************************************************************
** Form generated from reading UI file 'settingswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSWINDOW_H
#define UI_SETTINGSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SettingsWindow
{
public:
    QFrame *frameSettings;
    QLabel *labelReserve;
    QCheckBox *checkReserve;
    QLabel *labelSound;
    QCheckBox *checkSound;
    QLabel *labelHighlight;
    QCheckBox *checkHighlight;
    QLabel *labelPlayers;
    QSpinBox *spinPlayers;
    QLabel *labelBots;
    QSpinBox *spinBots;
    QLabel *labelReserve_2;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *buttonLayout;
    QLabel *labelTitle;
    QPushButton *homeButton;

    void setupUi(QWidget *SettingsWindow)
    {
        if (SettingsWindow->objectName().isEmpty())
            SettingsWindow->setObjectName("SettingsWindow");
        SettingsWindow->resize(800, 600);
        SettingsWindow->setStyleSheet(QString::fromUtf8("background-color: #a4bb7b;"));
        frameSettings = new QFrame(SettingsWindow);
        frameSettings->setObjectName("frameSettings");
        frameSettings->setGeometry(QRect(0, 70, 800, 541));
        frameSettings->setStyleSheet(QString::fromUtf8("background-color: #c4e09b; border-radius: 14px;"));
        labelReserve = new QLabel(frameSettings);
        labelReserve->setObjectName("labelReserve");
        labelReserve->setGeometry(QRect(10, 20, 388, 48));
        QFont font;
        font.setPointSize(20);
        labelReserve->setFont(font);
        checkReserve = new QCheckBox(frameSettings);
        checkReserve->setObjectName("checkReserve");
        checkReserve->setGeometry(QRect(570, 30, 171, 91));
        checkReserve->setStyleSheet(QString::fromUtf8("\n"
"QCheckBox::indicator { width: 180px; height: 80px; }\n"
"QCheckBox::indicator:checked { image: url(:/icon/on.png); }\n"
"QCheckBox::indicator:unchecked { image: url(:/icon/off.png); }\n"
"         "));
        checkReserve->setIconSize(QSize(15, 15));
        checkReserve->setChecked(true);
        labelSound = new QLabel(frameSettings);
        labelSound->setObjectName("labelSound");
        labelSound->setGeometry(QRect(20, 150, 381, 45));
        labelSound->setFont(font);
        checkSound = new QCheckBox(frameSettings);
        checkSound->setObjectName("checkSound");
        checkSound->setGeometry(QRect(570, 130, 171, 81));
        checkSound->setStyleSheet(QString::fromUtf8("\n"
"QCheckBox::indicator { width: 180px; height: 80px; }\n"
"QCheckBox::indicator:checked { image: url(:/icon/on.png); }\n"
"QCheckBox::indicator:unchecked { image: url(:/icon/off.png); }\n"
"         "));
        checkSound->setChecked(false);
        labelHighlight = new QLabel(frameSettings);
        labelHighlight->setObjectName("labelHighlight");
        labelHighlight->setGeometry(QRect(10, 240, 539, 45));
        labelHighlight->setFont(font);
        checkHighlight = new QCheckBox(frameSettings);
        checkHighlight->setObjectName("checkHighlight");
        checkHighlight->setGeometry(QRect(570, 230, 171, 81));
        checkHighlight->setStyleSheet(QString::fromUtf8("\n"
"QCheckBox::indicator { width: 180px; height: 80px; }\n"
"QCheckBox::indicator:checked { image: url(:/icon/on.png); }\n"
"QCheckBox::indicator:unchecked { image: url(:/icon/off.png); }\n"
"         "));
        checkHighlight->setChecked(false);
        labelPlayers = new QLabel(frameSettings);
        labelPlayers->setObjectName("labelPlayers");
        labelPlayers->setGeometry(QRect(20, 370, 350, 50));
        labelPlayers->setFont(font);
        spinPlayers = new QSpinBox(frameSettings);
        spinPlayers->setObjectName("spinPlayers");
        spinPlayers->setGeometry(QRect(100, 440, 150, 60));
        spinPlayers->setMinimum(2);
        spinPlayers->setMaximum(4);
        spinPlayers->setValue(2);
        labelBots = new QLabel(frameSettings);
        labelBots->setObjectName("labelBots");
        labelBots->setGeometry(QRect(450, 370, 300, 50));
        labelBots->setFont(font);
        spinBots = new QSpinBox(frameSettings);
        spinBots->setObjectName("spinBots");
        spinBots->setGeometry(QRect(500, 440, 150, 60));
        spinBots->setMinimum(0);
        spinBots->setMaximum(3);
        spinBots->setValue(0);
        labelReserve_2 = new QLabel(frameSettings);
        labelReserve_2->setObjectName("labelReserve_2");
        labelReserve_2->setGeometry(QRect(10, 60, 381, 51));
        labelReserve_2->setFont(font);
        verticalLayoutWidget = new QWidget(frameSettings);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(309, 489, 151, 41));
        buttonLayout = new QVBoxLayout(verticalLayoutWidget);
        buttonLayout->setObjectName("buttonLayout");
        buttonLayout->setContentsMargins(0, 0, 0, 0);
        labelTitle = new QLabel(SettingsWindow);
        labelTitle->setObjectName("labelTitle");
        labelTitle->setGeometry(QRect(270, 0, 280, 70));
        QFont font1;
        font1.setBold(true);
        labelTitle->setFont(font1);
        labelTitle->setStyleSheet(QString::fromUtf8("color:#222;  font-size: 38px\n"
""));
        labelTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);
        homeButton = new QPushButton(SettingsWindow);
        homeButton->setObjectName("homeButton");
        homeButton->setGeometry(QRect(40, 20, 51, 41));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        homeButton->setIcon(icon);
        homeButton->setIconSize(QSize(60, 60));
        homeButton->setFlat(true);

        retranslateUi(SettingsWindow);

        QMetaObject::connectSlotsByName(SettingsWindow);
    } // setupUi

    void retranslateUi(QWidget *SettingsWindow)
    {
        SettingsWindow->setWindowTitle(QCoreApplication::translate("SettingsWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        labelReserve->setStyleSheet(QCoreApplication::translate("SettingsWindow", "color:#222;", nullptr));
        labelReserve->setText(QCoreApplication::translate("SettingsWindow", "\320\237\320\276\320\272\320\260\320\267\321\213\320\262\320\260\321\202\321\214 \320\272\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 ", nullptr));
        checkReserve->setText(QString());
        labelSound->setStyleSheet(QCoreApplication::translate("SettingsWindow", "color:#222;", nullptr));
        labelSound->setText(QCoreApplication::translate("SettingsWindow", "\320\227\320\262\321\203\320\272", nullptr));
        checkSound->setText(QString());
        labelHighlight->setStyleSheet(QCoreApplication::translate("SettingsWindow", "color:#222;", nullptr));
        labelHighlight->setText(QCoreApplication::translate("SettingsWindow", "\320\237\320\276\320\264\321\201\320\262\320\265\321\202\320\272\320\260 \320\277\320\276\320\264\321\205\320\276\320\264\321\217\321\211\320\270\321\205 \320\264\320\276\320\274\320\270\320\275\320\276", nullptr));
        checkHighlight->setText(QString());
        labelPlayers->setText(QCoreApplication::translate("SettingsWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\270\320\263\321\200\320\276\320\272\320\276\320\262", nullptr));
        labelBots->setText(QCoreApplication::translate("SettingsWindow", "\320\232\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276 \320\261\320\276\321\202\320\276\320\262", nullptr));
        labelReserve_2->setStyleSheet(QCoreApplication::translate("SettingsWindow", "color:#222;", nullptr));
        labelReserve_2->setText(QCoreApplication::translate("SettingsWindow", "\320\264\320\276\320\274\320\270\320\275\320\276 \320\262 \321\200\320\265\320\267\320\265\321\200\320\262\320\265", nullptr));
        labelTitle->setText(QCoreApplication::translate("SettingsWindow", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        homeButton->setStyleSheet(QCoreApplication::translate("SettingsWindow", "background: transparent;", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SettingsWindow: public Ui_SettingsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSWINDOW_H
