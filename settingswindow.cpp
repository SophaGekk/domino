#include "SettingsWindow.h"
#include "ui_SettingsWindow.h"
#include <QShortcut>


SettingsWindow::SettingsWindow(QWidget *parent) : QWidget(parent), ui(new Ui::SettingsWindow), settings(new QSettings("MyCompany", "DominoGame")) {
    ui->setupUi(this);
    loadSettings();

    // Подключение кнопок выбора количества игроков

    connect(ui->spinPlayers, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SettingsWindow::validateBots);
    connect(ui->spinBots, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &SettingsWindow::validatePlayers);

    // Подключение чекбоксов
    connect(ui->checkReserve, &QCheckBox::stateChanged, this, &SettingsWindow::updateSettings);
    connect(ui->checkSound, &QCheckBox::stateChanged, this, &SettingsWindow::updateSettings);
    connect(ui->checkHighlight, &QCheckBox::stateChanged, this, &SettingsWindow::updateSettings);

    // Исправленное подключение кнопки "Домой"
    connect(ui->homeButton, &QPushButton::clicked, this, [this]() {
        emit returnToMainMenu(); // Эмитируем сигнал
    });
    QShortcut* saveShortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    connect(saveShortcut, &QShortcut::activated, this, &SettingsWindow::updateSettings);
}

void SettingsWindow::validateBots(int playersCount) {
    ui->spinBots->setMaximum(playersCount - 1);
    if(ui->spinBots->value() >= playersCount) {
        ui->spinBots->setValue(playersCount - 1);
    }
}

void SettingsWindow::validatePlayers(int botsCount) {
    int minPlayers = botsCount + 1;
    if(ui->spinPlayers->value() < minPlayers) {
        ui->spinPlayers->setValue(minPlayers);
    }
}

int SettingsWindow::getPlayers() const { return ui->spinPlayers->value(); }
int SettingsWindow::getBots() const { return ui->spinBots->value(); }

void SettingsWindow::updateSettings() {
    saveSettings();
}

void SettingsWindow::saveSettings() {
    // Сохраняем текущие значения из UI
    settings->setValue("players", ui->spinPlayers->value());
    settings->setValue("bots", ui->spinBots->value());
    settings->setValue("showReserve", ui->checkReserve->isChecked());
    settings->setValue("soundEnabled", ui->checkSound->isChecked());
    settings->setValue("highlight", ui->checkHighlight->isChecked());

    // Отправляем сигнал с новыми значениями
    emit settingsChanged(
        ui->spinPlayers->value(),
        ui->spinBots->value(),
        ui->checkReserve->isChecked(),
        ui->checkSound->isChecked(),
        ui->checkHighlight->isChecked()
        );
    close();
}

void SettingsWindow::loadSettings() {
    QSettings settings("MyCompany", "DominoGame");
    ui->spinPlayers->setValue(settings.value("players", 2).toInt());
    ui->spinBots->setValue(settings.value("bots", 0).toInt());
    ui->checkReserve->setChecked(settings.value("showReserve", ui->checkReserve->isChecked()).toBool());
    ui->checkSound->setChecked(settings.value("soundEnabled", false).toBool());
    ui->checkHighlight->setChecked(settings.value("highlight", false).toBool());
}

SettingsWindow::~SettingsWindow() {
    delete ui;
    delete settings;
}


