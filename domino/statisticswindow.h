#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include "ui_statisticswindow.h"

class StatisticsWindow : public QWidget {
    Q_OBJECT

public:
    explicit StatisticsWindow(QWidget *parent = nullptr);
    void loadStatistics(const QString &filePath);
    void addEntry(const QString &name, int wins, int points, int rounds, int players, const QString &date);

signals:
    void returnToMainMenu();

private slots:
    void onHomeClicked();

private:
    Ui::StatisticsWindow *ui; // Указатель на UI
};

#endif // STATISTICSWINDOW_H
