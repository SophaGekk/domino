#ifndef BAZAAROVERLAY_H
#define BAZAAROVERLAY_H

#include <QWidget>
#include <QGridLayout>
#include "domino_tile.h"
#include "dominolabel.h"
#include "ui_BazaarOverlay.h"
#include "bazaar.h"



class BazaarOverlay : public QWidget {
    Q_OBJECT
public:
    explicit BazaarOverlay(Bazaar* bazaar, QWidget* parent = nullptr);
    void updateTiles();

signals:
    void tileSelected(const DominoTile& tile);
protected:
    void closeEvent(QCloseEvent* event) override;
private:
    Ui::BazaarOverlay ui; // Автоматически сгенерированный UI
    Bazaar* m_bazaar;
    QGridLayout* tilesGrid;
    void centerWindow();    // Метод для центрирования
};

#endif // BAZAAROVERLAY_H
