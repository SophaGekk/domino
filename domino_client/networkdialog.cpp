#include "networkdialog.h"
#include "ui_networkdialog.h"

NetworkDialog::NetworkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetworkDialog)
{
    ui->setupUi(this);
    setWindowTitle("Сетевая игра");
    setFixedSize(400, 300);

    // Установка значений по умолчанию
    ui->editHost->setText("127.0.0.1");
    ui->editPort->setText("2323");
    ui->editPlayerName->setText("Игрок");

    // Соединение сигналов
    connect(ui->radioHost, &QRadioButton::toggled, this, &NetworkDialog::onHostToggled);
    connect(ui->radioHost, &QRadioButton::toggled, this, &NetworkDialog::updateUi);
    connect(ui->radioClient, &QRadioButton::toggled, this, &NetworkDialog::updateUi);
    updateUi();
}

NetworkDialog::~NetworkDialog()
{
    delete ui;
}

void NetworkDialog::updateUi()
{
    const bool isHost = ui->radioHost->isChecked();

    // Видимость элементов
    ui->labelPlayers->setVisible(isHost);
    ui->spinPlayers->setVisible(isHost);
    ui->labelCode->setVisible(!isHost);
    ui->editCode->setVisible(!isHost);
}

bool NetworkDialog::isHosting() const
{
    return ui->radioHost->isChecked();
}

QString NetworkDialog::getPlayerName() const
{
    return ui->editPlayerName->text();
}

QString NetworkDialog::getHostAddress() const
{
    return ui->editHost->text();
}

quint16 NetworkDialog::getPort() const
{
    return ui->editPort->text().toUShort();
}

void NetworkDialog::onHostToggled(bool checked)
{
    ui->editHost->setEnabled(!checked);
    if (checked) {
        ui->editHost->setText("127.0.0.1");
    }
}

int NetworkDialog::getPlayersCount() const
{
    return ui->spinPlayers->value();
}

QString NetworkDialog::getSessionCode() const
{
    return ui->editCode->text();
}

bool NetworkDialog::isJoining() const
{
    return ui->radioClient->isChecked();
}
