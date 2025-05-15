#include "statisticswindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

StatisticsWindow::StatisticsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::StatisticsWindow) {
    ui->setupUi(this); // Загрузка UI из файла

    // Настройка кнопки "Домой"
    connect(ui->homeButton, &QPushButton::clicked, this, &StatisticsWindow::onHomeClicked);

    ui->tableStats->setColumnWidth(0, 130);
    ui->tableStats->setColumnWidth(1, 70);
    ui->tableStats->setColumnWidth(2, 80);
    ui->tableStats->setColumnWidth(3, 80);
    ui->tableStats->setColumnWidth(4, 140);
    // ui->tableStats->setColumnWidth(5, 120); // 120 - ширина в пикселях, подберите под себя

}

void StatisticsWindow::loadStatistics(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл статистики";
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonArray statsArray = doc.array();

    ui->tableStats->setRowCount(0); // Очистка таблицы перед загрузкой

    for (const QJsonValue &value : statsArray) {
        QJsonObject obj = value.toObject();
        addEntry(
            obj["name"].toString(),
            obj["wins"].toInt(),
            obj["points"].toInt(),
            obj["rounds"].toInt(),
            obj["players"].toInt(),
            obj["date"].toString()
            );
    }
}

void StatisticsWindow::addEntry(const QString &name, int wins, int points, int rounds, int players, const QString &date) {
    int row = ui->tableStats->rowCount();
    ui->tableStats->insertRow(row);

    // Заполнение данных в колонки (соответствуют UI)
    ui->tableStats->setItem(row, 0, new QTableWidgetItem(name));
    ui->tableStats->setItem(row, 1, new QTableWidgetItem(QString::number(wins)));
    ui->tableStats->setItem(row, 2, new QTableWidgetItem(QString::number(points)));
    ui->tableStats->setItem(row, 3, new QTableWidgetItem(QString::number(rounds)));
    ui->tableStats->setItem(row, 4, new QTableWidgetItem(QString::number(players)));
    ui->tableStats->setItem(row, 5, new QTableWidgetItem(date));

}

void StatisticsWindow::onHomeClicked() {
    this->close();
    emit returnToMainMenu();
}
