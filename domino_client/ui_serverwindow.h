/********************************************************************************
** Form generated from reading UI file 'serverwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVERWINDOW_H
#define UI_SERVERWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Serverwindow 
{
public:
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout_2;
    QTextBrowser *textBrowser;
    QPushButton *serverButton;
    QPushButton *localButton;

    void setupUi(QWidget *Serverwindow_)
    {
        if (Serverwindow_->objectName().isEmpty())
            Serverwindow_->setObjectName("Serverwindow_");
        Serverwindow_->resize(800, 600);
        gridLayout = new QGridLayout(Serverwindow_);
        gridLayout->setObjectName("gridLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        textBrowser = new QTextBrowser(Serverwindow_);
        textBrowser->setObjectName("textBrowser");

        verticalLayout_2->addWidget(textBrowser);

        serverButton = new QPushButton(Serverwindow_);
        serverButton->setObjectName("serverButton");
        serverButton->setEnabled(true);
        serverButton->setMaximumSize(QSize(778, 16777215));
        QFont font;
        font.setFamilies({QString::fromUtf8("Segoe UI")});
        serverButton->setFont(font);
        serverButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
" background-color: #eaf2e0;\n"
" border-radius: 12px;\n"
" font-size: 20px;\n"
" color: #222;\n"
" border: 2px solid #666;\n"
" min-width: 60px;\n"
" min-height: 60px;\n"
"}\n"
"           "));
        serverButton->setAutoDefault(false);
        serverButton->setFlat(false);

        verticalLayout_2->addWidget(serverButton);

        localButton = new QPushButton(Serverwindow_);
        localButton->setObjectName("localButton");
        localButton->setFont(font);
        localButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
" background-color: #eaf2e0;\n"
" border-radius: 12px;\n"
" font-size: 20px;\n"
" color: #222;\n"
" border: 2px solid #666;\n"
" min-width: 60px;\n"
" min-height: 60px;\n"
"}\n"
"           "));

        verticalLayout_2->addWidget(localButton);


        gridLayout->addLayout(verticalLayout_2, 0, 0, 1, 1);


        retranslateUi(Serverwindow_);

        QMetaObject::connectSlotsByName(Serverwindow_);
    } // setupUi

    void retranslateUi(QWidget *Serverwindow_)
    {
        Serverwindow_->setWindowTitle(QCoreApplication::translate("Serverwindow ", "\320\224\320\276\320\274\320\270\320\275\320\276", nullptr));
        serverButton->setText(QCoreApplication::translate("Serverwindow ", "\320\237\320\276\320\264\320\272\320\273\321\216\321\207\320\270\321\202\321\214\321\201\321\217 \320\272 \321\201\320\265\321\200\320\262\320\265\321\200\321\203", nullptr));
        localButton->setText(QCoreApplication::translate("Serverwindow ", "\320\230\320\263\321\200\320\260\321\202\321\214 \320\273\320\276\320\272\320\260\320\273\321\214\320\275\320\276", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Serverwindow : public Ui_Serverwindow  {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVERWINDOW_H
