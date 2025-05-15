#include "bazaaroverlay.h"

BazaarOverlay::BazaarOverlay(Bazaar* bazaar, QWidget* parent)
    : QWidget(parent), m_bazaar(bazaar) {
    ui.setupUi(this);

    setWindowModality(Qt::ApplicationModal);
    centerWindow(); // Центрирование

    setStyleSheet(R"(
    QWidget {
        background: white;
        border: 2px solid #444;
        border-radius: 8px;
    }
    QLabel {
        padding: 10px;
    }
)");
    setWindowModality(Qt::ApplicationModal);

    QPushButton* closeBtn = new QPushButton("×", this);
    closeBtn->setStyleSheet("font-size: 18px; border: none;");
    connect(closeBtn, &QPushButton::clicked, this, &BazaarOverlay::close);
    closeBtn->move(width() - 30, 10);
}


void BazaarOverlay::closeEvent(QCloseEvent* event) {
    deleteLater(); // Удалить объект при закрытии
    event->accept();
}

void BazaarOverlay::centerWindow() {
    // Явно задаем размер окна базара (примерные размеры)
    resize(400, 400); // Настройте под ваши нужды

    // Получаем геометрию экрана

    // Вычисляем позицию для центрирования
    int x = (800 - width()) / 2;
    int y = (600 - height()) / 2;

    move(x, y); // Устанавливаем позицию
}

void BazaarOverlay::updateTiles() {
    // Очистить текущие элементы
    QLayoutItem* item;
    while ((item = ui.tilesGrid->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Добавить домино из базара (тыльной стороной)
    const QVector<DominoTile>& bazaarTiles = m_bazaar->getRemainingTiles();
    for (int i = 0; i < bazaarTiles.size(); ++i) {
        DominoLabel* label = new DominoLabel(this);
        // Установка стиля как у верхнего игрока
        label->setFixedSize(40, 80); // Размер как у нижнего игрока
        label->setStyleSheet("background: white; border: 1px solid black;");
        label->setDots(-1, -1, false, false); // Скрыть точки, линию

        // Обработка клика
        connect(label, &DominoLabel::clicked, this, [this, i]() {
            emit tileSelected(m_bazaar->getRemainingTiles()[i]);
            close();
        });

        // Добавление в сетку (5 колонок на строку)
        ui.tilesGrid->addWidget(label, i / 5, i % 5);
    }
}
