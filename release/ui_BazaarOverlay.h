/********************************************************************************
** Form generated from reading UI file 'BazaarOverlay.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BAZAAROVERLAY_H
#define UI_BAZAAROVERLAY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BazaarOverlay
{
public:
    QVBoxLayout *overlayLayout;
    QSpacerItem *verticalSpacerTop;
    QVBoxLayout *centralLayout;
    QLabel *infoLabel;
    QGridLayout *tilesGrid;
    QSpacerItem *verticalSpacerBottom;

    void setupUi(QWidget *BazaarOverlay)
    {
        if (BazaarOverlay->objectName().isEmpty())
            BazaarOverlay->setObjectName("BazaarOverlay");
        BazaarOverlay->setWindowModality(Qt::WindowModality::ApplicationModal);
        BazaarOverlay->resize(462, 296);
        overlayLayout = new QVBoxLayout(BazaarOverlay);
        overlayLayout->setObjectName("overlayLayout");
        verticalSpacerTop = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        overlayLayout->addItem(verticalSpacerTop);

        centralLayout = new QVBoxLayout();
        centralLayout->setObjectName("centralLayout");
        infoLabel = new QLabel(BazaarOverlay);
        infoLabel->setObjectName("infoLabel");
        infoLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; color: #ffffff; background: transparent;"));
        infoLabel->setAlignment(Qt::AlignmentFlag::AlignCenter);

        centralLayout->addWidget(infoLabel);

        tilesGrid = new QGridLayout();
        tilesGrid->setObjectName("tilesGrid");

        centralLayout->addLayout(tilesGrid);


        overlayLayout->addLayout(centralLayout);

        verticalSpacerBottom = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        overlayLayout->addItem(verticalSpacerBottom);


        retranslateUi(BazaarOverlay);

        QMetaObject::connectSlotsByName(BazaarOverlay);
    } // setupUi

    void retranslateUi(QWidget *BazaarOverlay)
    {
        BazaarOverlay->setStyleSheet(QCoreApplication::translate("BazaarOverlay", "\n"
"     background-color: rgba(0, 0, 0, 128);\n"
"   ", nullptr));
        infoLabel->setText(QCoreApplication::translate("BazaarOverlay", "\320\235\320\265\321\207\320\265\320\274 \321\205\320\276\320\264\320\270\321\202\321\214, \320\275\320\265\320\276\320\261\321\205\320\276\320\264\320\270\320\274\320\276 \320\262\320\267\321\217\321\202\321\214 \320\272\320\276\321\201\321\202\321\217\321\210\320\272\321\203", nullptr));
    } // retranslateUi

};

namespace Ui {
    class BazaarOverlay: public Ui_BazaarOverlay {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BAZAAROVERLAY_H
