#include "nicknamedialog.h"

NicknameDialog::NicknameDialog(int humanPlayers, QWidget *parent)
    : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    for (int i = 0; i < humanPlayers; ++i) {
        QLabel *label = new QLabel(QString("Игрок %1:").arg(i + 1), this);
        QLineEdit *lineEdit = new QLineEdit(this);
        lineEdit->setPlaceholderText("Введите никнейм");
        layout->addWidget(label);
        layout->addWidget(lineEdit);
        nameEdits.append(lineEdit);
    }

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addWidget(buttonBox);
}

QStringList NicknameDialog::getNames() const {
    QStringList names;
    for (QLineEdit *edit : nameEdits) {
        QString name = edit->text().trimmed();
        names.append(name.isEmpty() ? QString("Игрок %1").arg(names.size() + 1) : name);
    }
    return names;
}
