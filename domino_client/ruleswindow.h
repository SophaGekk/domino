#ifndef RULESWINDOW_H
#define RULESWINDOW_H

#include <QDialog>
#include <QWidget>

namespace Ui {
class RulesWindow;
}

class RulesWindow : public QDialog {
    Q_OBJECT
signals:
    void returnToMainMenu();

public:
    explicit RulesWindow(QWidget *parent = nullptr);
    ~RulesWindow();

private:
    Ui::RulesWindow *ui; // UI из RulesWindow.ui
};

#endif // RULESWINDOW_H
