/********************************************************************************
** Form generated from reading UI file 'StatisticsWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_STATISTICSWINDOW_H
#define UI_STATISTICSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_StatisticsWindow
{
public:
    QFormLayout *formLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *homeButton;
    QSpacerItem *horizontalSpacer;
    QLabel *labelTitle;
    QSpacerItem *horizontalSpacer_2;
    QVBoxLayout *verticalLayout_2;
    QFrame *frameTable;
    QFormLayout *formLayout;
    QTableWidget *tableStats;

    void setupUi(QWidget *StatisticsWindow)
    {
        if (StatisticsWindow->objectName().isEmpty())
            StatisticsWindow->setObjectName("StatisticsWindow");
        StatisticsWindow->resize(800, 600);
        StatisticsWindow->setStyleSheet(QString::fromUtf8("background-color: #a4bb7b;"));
        formLayout_2 = new QFormLayout(StatisticsWindow);
        formLayout_2->setObjectName("formLayout_2");
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        homeButton = new QPushButton(StatisticsWindow);
        homeButton->setObjectName("homeButton");
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icon/home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        homeButton->setIcon(icon);
        homeButton->setIconSize(QSize(60, 60));
        homeButton->setFlat(true);

        horizontalLayout_3->addWidget(homeButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        labelTitle = new QLabel(StatisticsWindow);
        labelTitle->setObjectName("labelTitle");
        QFont font;
        font.setBold(true);
        labelTitle->setFont(font);
        labelTitle->setStyleSheet(QString::fromUtf8("color:#222;  font-size: 38px\n"
""));
        labelTitle->setAlignment(Qt::AlignmentFlag::AlignCenter);

        horizontalLayout_3->addWidget(labelTitle);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_3);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        frameTable = new QFrame(StatisticsWindow);
        frameTable->setObjectName("frameTable");
        formLayout = new QFormLayout(frameTable);
        formLayout->setObjectName("formLayout");
        tableStats = new QTableWidget(frameTable);
        if (tableStats->columnCount() < 6)
            tableStats->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableStats->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        if (tableStats->rowCount() < 11)
            tableStats->setRowCount(11);
        tableStats->setObjectName("tableStats");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tableStats->sizePolicy().hasHeightForWidth());
        tableStats->setSizePolicy(sizePolicy);
        tableStats->setMinimumSize(QSize(700, 450));
        QFont font1;
        tableStats->setFont(font1);
        tableStats->setStyleSheet(QString::fromUtf8("\n"
"QTableWidget {\n"
" background-color: #c4e09b; \n"
" font-size: 28px;\n"
" color: #222;\n"
" gridline-color: #b2c28a;\n"
" border-radius: 12px;\n"
"}\n"
"QHeaderView::section {\n"
" background-color: #c4e09b; \n"
" font-size: 18px;\n"
" font-weight: bold;\n"
" color: #222;\n"
" border: none;\n"
"}\n"
"QScrollBar:vertical {\n"
"    background: transparent;\n"
"    width: 8px;\n"
"    margin: 0px 0px 0px 0px;\n"
"    border-radius: 4px;\n"
"}\n"
"QScrollBar::handle:vertical {\n"
"    background: #888;\n"
"    min-height: 20px;\n"
"    border-radius: 4px;\n"
"}\n"
"QScrollBar:vertical:hover {\n"
"    width: 16px;\n"
"    background: #b2c28a;\n"
"}\n"
"QScrollBar::handle:vertical:hover {\n"
"    background: #5a5;\n"
"}\n"
"QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {\n"
"    height: 0px;\n"
"}\n"
"           "));
        tableStats->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAsNeeded);
        tableStats->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        tableStats->setSizeAdjustPolicy(QAbstractScrollArea::SizeAdjustPolicy::AdjustToContents);
        tableStats->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
        tableStats->setAlternatingRowColors(false);
        tableStats->setSelectionMode(QAbstractItemView::SelectionMode::NoSelection);
        tableStats->setShowGrid(true);
        tableStats->setRowCount(11);
        tableStats->setColumnCount(6);
        tableStats->horizontalHeader()->setCascadingSectionResizes(true);
        tableStats->horizontalHeader()->setMinimumSectionSize(25);
        tableStats->horizontalHeader()->setDefaultSectionSize(115);
        tableStats->horizontalHeader()->setHighlightSections(false);
        tableStats->horizontalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableStats->horizontalHeader()->setStretchLastSection(true);
        tableStats->verticalHeader()->setMinimumSectionSize(30);
        tableStats->verticalHeader()->setDefaultSectionSize(40);
        tableStats->verticalHeader()->setProperty("showSortIndicator", QVariant(false));
        tableStats->verticalHeader()->setStretchLastSection(false);

        formLayout->setWidget(0, QFormLayout::FieldRole, tableStats);


        verticalLayout_2->addWidget(frameTable);


        formLayout_2->setLayout(2, QFormLayout::FieldRole, verticalLayout_2);


        retranslateUi(StatisticsWindow);

        QMetaObject::connectSlotsByName(StatisticsWindow);
    } // setupUi

    void retranslateUi(QWidget *StatisticsWindow)
    {
        StatisticsWindow->setWindowTitle(QCoreApplication::translate("StatisticsWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        homeButton->setStyleSheet(QCoreApplication::translate("StatisticsWindow", "background: transparent;", nullptr));
        labelTitle->setText(QCoreApplication::translate("StatisticsWindow", "\320\241\321\202\320\260\321\202\320\270\321\201\321\202\320\270\320\272\320\260", nullptr));
        frameTable->setStyleSheet(QCoreApplication::translate("StatisticsWindow", "\n"
"background-color: #c7e09c;\n"
"border-radius: 16px;\n"
"border: 3px solid #a8b98d;\n"
"        ", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableStats->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("StatisticsWindow", "\320\230\320\274\321\217", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableStats->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("StatisticsWindow", "\320\277\320\276\320\261\320\265\320\264", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableStats->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("StatisticsWindow", "\320\261\320\260\320\273\320\273\321\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableStats->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("StatisticsWindow", "\321\200\320\260\321\203\320\275\320\264\321\213", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableStats->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("StatisticsWindow", "\320\272\320\276\320\273-\320\262\320\276 \320\270\320\263\321\200\320\276\320\272\320\276\320\262", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableStats->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("StatisticsWindow", "\320\224\320\260\321\202\320\260", nullptr));
        tableStats->setProperty("horizontalHeaderLabels", QVariant(QStringList{
            QCoreApplication::translate("StatisticsWindow", "\320\230\320\274\321\217", nullptr),
            QCoreApplication::translate("StatisticsWindow", "\320\272\320\276\320\273-\320\262\320\276 \320\277\320\276\320\261\320\265\320\264", nullptr),
            QCoreApplication::translate("StatisticsWindow", "\321\201\321\203\320\274\320\274\320\260 \320\261\320\260\320\273\320\273\320\276\320\262", nullptr),
            QCoreApplication::translate("StatisticsWindow", "\320\272\320\276\320\273-\320\262\320\276 \321\200\320\260\321\203\320\275\320\264\320\276\320\262", nullptr),
            QCoreApplication::translate("StatisticsWindow", "\320\272\320\276\320\273-\320\262\320\276 \320\270\320\263\321\200\320\276\320\272\320\276\320\262", nullptr),
            QCoreApplication::translate("StatisticsWindow", "\320\224\320\260\321\202\320\260", nullptr)}));
    } // retranslateUi

};

namespace Ui {
    class StatisticsWindow: public Ui_StatisticsWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_STATISTICSWINDOW_H
