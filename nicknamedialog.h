#ifndef NICKNAMEDIALOG_H
#define NICKNAMEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

class NicknameDialog : public QDialog {
    Q_OBJECT
public:
    explicit NicknameDialog(int humanPlayers, QWidget *parent = nullptr);
    QStringList getNames() const;

private:
    QList<QLineEdit*> nameEdits;
};

#endif // NICKNAMEDIALOG_H
