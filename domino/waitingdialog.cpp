#include "waitingdialog.h"
#include "ui_waitingdialog.h"

WaitingDialog::WaitingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitingDialog)
{
    ui->setupUi(this);
    setWindowTitle("Ожидание игроков");
    setFixedSize(400, 300);
}

WaitingDialog::~WaitingDialog()
{
    delete ui;
}

void WaitingDialog::setSessionCode(const QString& code)
{
    ui->lblCode->setText("Код сессии: " + code);
    ui->lblCode->setVisible(true);
}

void WaitingDialog::addPlayer(const QString& playerName)
{
    ui->listPlayers->addItem(playerName);
}

void WaitingDialog::setPlayersCount(int current, int required)
{
    ui->lblStatus->setText(QString("Ожидание игроков: %1/%2").arg(current).arg(required));
}
