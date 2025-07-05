/********************************************************************************
** Form generated from reading UI file 'ruleswindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RULESWINDOW_H
#define UI_RULESWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RulesWindow
{
public:
    QVBoxLayout *verticalLayoutMain;
    QHBoxLayout *headerLayout;
    QPushButton *homeButton;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacer_2;
    QHBoxLayout *centerLayout;
    QFrame *frameRules;
    QVBoxLayout *verticalLayoutRules;
    QTextBrowser *textRules;

    void setupUi(QWidget *RulesWindow)
    {
        if (RulesWindow->objectName().isEmpty())
            RulesWindow->setObjectName("RulesWindow");
        RulesWindow->resize(800, 600);
        RulesWindow->setStyleSheet(QString::fromUtf8("background-color: #a4bb7b;"));
        verticalLayoutMain = new QVBoxLayout(RulesWindow);
        verticalLayoutMain->setObjectName("verticalLayoutMain");
        headerLayout = new QHBoxLayout();
        headerLayout->setObjectName("headerLayout");
        homeButton = new QPushButton(RulesWindow);
        homeButton->setObjectName("homeButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        homeButton->setIcon(icon);
        homeButton->setIconSize(QSize(50, 50));
        homeButton->setFlat(true);

        headerLayout->addWidget(homeButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(horizontalSpacer);

        labelTitle = new QLabel(RulesWindow);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setPointSize(36);
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        headerLayout->addWidget(labelTitle);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        headerLayout->addItem(horizontalSpacer_2);


        verticalLayoutMain->addLayout(headerLayout);

        centerLayout = new QHBoxLayout();
        centerLayout->setObjectName("centerLayout");
        frameRules = new QFrame(RulesWindow);
        frameRules->setObjectName("frameRules");
        verticalLayoutRules = new QVBoxLayout(frameRules);
        verticalLayoutRules->setObjectName("verticalLayoutRules");
        textRules = new QTextBrowser(frameRules);
        textRules->setObjectName("textRules");
        textRules->setStyleSheet(QString::fromUtf8("\n"
"QTextBrowser {\n"
" background: transparent;\n"
" border: none;\n"
" font-size: 18px;\n"
" color: #222;\n"
"}\n"
"           "));
        textRules->setHtml(QString::fromUtf8("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:18px; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt;\">\320\222\320\260\320\274 \320\270 \320\262\320\260\321\210\320\270\320\274 \321\201\320\276\320\277\320\265\321\200\320\275\320\270\320\272\320\260\320\274 \320\262\321\213\320\264\320\260\320\265\321\202\321\201\321\217 \320\275\320\265\321\201\320\272\320\276\320\273\321\214\320\272\320\276 (5-7) \320\264\320\276\320\274\320\270\320\275\320\276\321\210\320\265\320"
                        "\272.<br />\320\237\320\276 \320\276\321\207\320\265\321\200\320\265\320\264\320\270 \320\262\321\213\320\272\320\273\320\260\320\264\321\213\320\262\320\260\320\271\321\202\320\265 \321\201\320\262\320\276\320\270 \320\264\320\276\320\274\320\270\320\275\320\276\321\210\320\272\320\270 \320\272 \320\276\320\264\320\275\320\276\320\274\321\203 \320\270\320\267 \320\272\320\276\320\275\321\206\320\276\320\262 \321\206\320\265\320\277\320\276\321\207\320\272\320\270, \320\272\320\276\321\202\320\276\321\200\320\260\321\217 \321\204\320\276\321\200\320\274\320\270\321\200\321\203\320\265\321\202\321\201\321\217 \320\275\320\260 \320\270\320\263\321\200\320\276\320\262\320\276\320\274 \320\277\320\276\320\273\320\265.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt; font-weight:700;\">\320\236\321\201\320\275\320\276\320\262\320\275\320\276\320\265 \320\277\321\200\320\260\320\262\320\270\320\273"
                        "\320\276:</span><span style=\" font-size:18pt;\"> \320\275\321\203\320\266\320\275\320\276 \321\201\320\276\320\265\320\264\320\270\320\275\321\217\321\202\321\214 \320\277\320\276\320\273\320\276\320\262\320\270\320\275\320\272\320\270 \320\264\320\276\320\274\320\270\320\275\320\276\321\210\320\265\320\272 \321\201 \320\276\320\264\320\270\320\275\320\260\320\272\320\276\320\262\321\213\320\274 \320\272\320\276\320\273\320\270\321\207\320\265\321\201\321\202\320\262\320\276\320\274 \321\202\320\276\321\207\320\265\320\272. \320\255\321\202\320\276 \320\267\320\275\320\260\321\207\320\270\321\202, \321\207\321\202\320\276 \320\265\321\201\320\273\320\270 \320\275\320\260 \320\276\320\264\320\275\320\276\320\274 \320\270\320\267 \320\272\320\276\320\275\321\206\320\276\320\262 \321\206\320\265\320\277\320\276\321\207\320\272\320\270 (\320\276\321\202\320\272\321\200\321\213\321\202\320\276\320\274 \320\272\320\276\320\275\321\206\320\265) \320\275\320\260\321\205\320\276\320\264\320\270\321\202\321\201\321\217"
                        " \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265 &quot;3&quot;, \321\202\320\276 \320\262\321\213 \320\274\320\276\320\266\320\265\321\202\320\265 \320\277\321\200\320\270\321\201\321\202\320\260\320\262\320\270\321\202\321\214 \320\272 \320\275\320\265\320\274\321\203 \321\202\320\276\320\273\321\214\320\272\320\276 \320\264\320\276\320\274\320\270\320\275\320\276\321\210\320\272\321\203, \321\203 \320\272\320\276\321\202\320\276\321\200\320\276\320\271 \321\202\320\276\320\266\320\265 \320\265\321\201\321\202\321\214 \320\277\320\276\320\273\320\276\320\262\320\270\320\275\320\272\320\260 \321\201\320\276 \320\267\320\275\320\260\321\207\320\265\320\275\320\270\320\265\320\274 &quot;3&quot;.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt;\">\320\225\321\201\320\273\320\270 \321\203 \320\262\320\260\321\201 \320\275\320\265\321\202 \320\277\320\276\320\264\321\205\320\276\320"
                        "\264\321\217\321\211\320\265\320\271 \320\264\320\276\320\274\320\270\320\275\320\276\321\210\320\272\320\270, \320\262\320\260\320\274 \320\277\321\200\320\270\320\264\320\265\321\202\321\201\321\217 \320\262\320\267\321\217\321\202\321\214 \320\275\320\276\320\262\321\203\321\216 \320\270\320\267 &quot;\320\261\320\260\320\267\320\260\321\200\320\260&quot; (\320\265\321\201\320\273\320\270 \320\276\320\275 \320\275\320\265 \320\277\321\203\321\201\321\202). \320\225\321\201\320\273\320\270 &quot;\320\261\320\260\320\267\320\260\321\200&quot; \320\277\321\203\321\201\321\202, \320\262\321\213 \320\277\321\200\320\276\320\277\321\203\321\201\320\272\320\260\320\265\321\202\320\265 \321\205\320\276\320\264.</span></p>\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:18pt;\">\320\225\321\201\320\273\320\270 \320\262\321\213 \320\277\320\265\321\200\320\262\321\213\320\274 \320\262\321\213\320\273\320\276\320\266"
                        "\320\270\320\273\320\270 \320\262\321\201\320\265 \321\201\320\262\320\276\320\270 \320\264\320\276\320\274\320\270\320\275\320\276\321\210\320\272\320\270, \320\262\321\213 \342\200\223 \320\277\320\276\320\261\320\265\320\264\320\270\321\202\320\265\320\273\321\214!<br />\320\225\321\201\320\273\320\270 \320\275\320\270\320\272\321\202\320\276 \320\275\320\265 \320\274\320\276\320\266\320\265\321\202 \321\201\320\264\320\265\320\273\320\260\321\202\321\214 \321\205\320\276\320\264 (&quot;\321\200\321\213\320\261\320\260&quot;), \320\277\320\276\320\261\320\265\320\266\320\264\320\260\320\265\321\202 \321\202\320\276\321\202, \321\203 \320\272\320\276\320\263\320\276 \321\201\321\203\320\274\320\274\320\260 \321\202\320\276\321\207\320\265\320\272 \320\275\320\260 \320\276\321\201\321\202\320\260\320\262\321\210\320\270\321\205\321\201\321\217 \320\264\320\276\320\274\320\270\320\275\320\276\321\210\320\272\320\260\321\205 \320\274\320\265\320\275\321\214\321\210\320\265 \320\262\321\201\320\265\320\263\320\276"
                        ". </span></p></body></html>"));

        verticalLayoutRules->addWidget(textRules);


        centerLayout->addWidget(frameRules);


        verticalLayoutMain->addLayout(centerLayout);


        retranslateUi(RulesWindow);

        QMetaObject::connectSlotsByName(RulesWindow);
    } // setupUi

    void retranslateUi(QWidget *RulesWindow)
    {
        RulesWindow->setWindowTitle(QCoreApplication::translate("RulesWindow", "\320\237\321\200\320\260\320\262\320\270\320\273\320\260", nullptr));
        homeButton->setStyleSheet(QCoreApplication::translate("RulesWindow", "background: transparent;", nullptr));
        labelTitle->setStyleSheet(QCoreApplication::translate("RulesWindow", "color:#222;", nullptr));
        labelTitle->setText(QCoreApplication::translate("RulesWindow", "\320\237\321\200\320\260\320\262\320\270\320\273\320\260", nullptr));
        frameRules->setStyleSheet(QCoreApplication::translate("RulesWindow", "\n"
"background-color: #c7e09c;\n"
"border-radius: 12px;\n"
"border: 2px solid #b2c28a;\n"
"        ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RulesWindow: public Ui_RulesWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RULESWINDOW_H
