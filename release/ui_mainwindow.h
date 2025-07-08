/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow 
{
public:
    QWidget *centralwidget;
    QPushButton *playButton;
    QPushButton *rulesButton;
    QLabel *label;
    QPushButton *settingsButton;
    QPushButton *statsButton;

    void setupUi(QMainWindow *MainWindow_)
    {
        if (MainWindow_->objectName().isEmpty())
            MainWindow_->setObjectName("MainWindow_");
        MainWindow_->resize(800, 600);
        centralwidget = new QWidget(MainWindow_);
        centralwidget->setObjectName("centralwidget");
        playButton = new QPushButton(centralwidget);
        playButton->setObjectName("playButton");
        playButton->setEnabled(true);
        playButton->setGeometry(QRect(227, 300, 345, 65));
        playButton->setMaximumSize(QSize(778, 16777215));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        playButton->setFont(font);
        playButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
" background-color: #eaf2e0;\n"
" border-radius: 12px;\n"
" font-size: 20px;\n"
" color: #222;\n"
" border: 2px solid #666;\n"
" min-width: 60px;\n"
" min-height: 60px;\n"
"}\n"
"           "));
        playButton->setAutoDefault(false);
        playButton->setFlat(false);
        rulesButton = new QPushButton(centralwidget);
        rulesButton->setObjectName("rulesButton");
        rulesButton->setGeometry(QRect(227, 395, 345, 65));
        rulesButton->setFont(font);
        rulesButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
" background-color: #eaf2e0;\n"
" border-radius: 12px;\n"
" font-size: 20px;\n"
" color: #222;\n"
" border: 2px solid #666;\n"
" min-width: 60px;\n"
" min-height: 60px;\n"
"}\n"
"           "));
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setEnabled(true);
        label->setGeometry(QRect(73, 5, 654, 327));
        label->setPixmap(QPixmap(QString::fromUtf8(":/icon/domino.png")));
        label->setScaledContents(true);
        settingsButton = new QPushButton(centralwidget);
        settingsButton->setObjectName("settingsButton");
        settingsButton->setGeometry(QRect(36, 507, 200, 80));
        settingsButton->setMinimumSize(QSize(64, 64));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setBold(false);
        settingsButton->setFont(font1);
        settingsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
" background-color: #eaf2e0;\n"
" border-radius: 12px;\n"
" font-size: 20px;\n"
" color: #222;\n"
" border: 2px solid #666;\n"
" min-width: 60px;\n"
" min-height: 60px;\n"
"}\n"
"           "));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/setting.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        settingsButton->setIcon(icon);
        settingsButton->setIconSize(QSize(32, 32));
        statsButton = new QPushButton(centralwidget);
        statsButton->setObjectName("statsButton");
        statsButton->setGeometry(QRect(564, 507, 200, 80));
        statsButton->setMinimumSize(QSize(64, 64));
        statsButton->setFont(font);
        statsButton->setCursor(QCursor(Qt::CursorShape::ArrowCursor));
        statsButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
" background-color: #eaf2e0;\n"
" border-radius: 12px;\n"
" font-size: 20px;\n"
" color: #222;\n"
" border: 2px solid #666;\n"
" min-width: 60px;\n"
" min-height: 60px;\n"
"}\n"
"           "));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icon/stat.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        statsButton->setIcon(icon1);
        statsButton->setIconSize(QSize(32, 32));
        MainWindow_->setCentralWidget(centralwidget);
        settingsButton->raise();
        statsButton->raise();
        label->raise();
        playButton->raise();
        rulesButton->raise();

        retranslateUi(MainWindow_);

        QMetaObject::connectSlotsByName(MainWindow_);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow_)
    {
        MainWindow_->setWindowTitle(QCoreApplication::translate("MainWindow ", "\320\224\320\276\320\274\320\270\320\275\320\276", nullptr));
        centralwidget->setStyleSheet(QCoreApplication::translate("MainWindow ", "background-color: #b4c28a;", nullptr));
        playButton->setText(QCoreApplication::translate("MainWindow ", "\320\230\320\263\321\200\320\260\321\202\321\214", nullptr));
        rulesButton->setText(QCoreApplication::translate("MainWindow ", "\320\237\321\200\320\260\320\262\320\270\320\273\320\260 \320\270\320\263\321\200\321\213", nullptr));
        label->setText(QString());
        settingsButton->setText(QCoreApplication::translate("MainWindow ", "\320\235\320\260\321\201\321\202\321\200\320\276\320\271\320\272\320\270", nullptr));
        statsButton->setText(QCoreApplication::translate("MainWindow ", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow : public Ui_MainWindow  {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
