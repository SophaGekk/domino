#include "ruleswindow.h"
#include "ui_ruleswindow.h" // Автоматически генерируется из RulesWindow.ui

RulesWindow::RulesWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::RulesWindow) {
    ui->setupUi(this); // Инициализация UI
    // Настройки окна
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint & ~Qt::WindowFullscreenButtonHint);
    setFixedSize(size()); // Фиксирует размер после инициализации интерфейса
    // Исправленное подключение кнопки "Домой"
    connect(ui->homeButton, &QPushButton::clicked, this, [this]() {
        emit returnToMainMenu(); // Эмитируем сигнал
    });
}

RulesWindow::~RulesWindow() {
    delete ui;
}
