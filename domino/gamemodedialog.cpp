#include "gamemodedialog.h"
#include "ui_gamemodedialog.h"

GameModeDialog::GameModeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameModeDialog)
{
    ui->setupUi(this);
    setWindowTitle("Выбор режима игры");
    setFixedSize(300, 150);
}

GameModeDialog::~GameModeDialog()
{
    delete ui;
}

bool GameModeDialog::isNetworkGameSelected() const
{
    return ui->radioNetwork->isChecked();
}
