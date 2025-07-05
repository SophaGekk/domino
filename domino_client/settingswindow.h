#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSettings>
#include <QPushButton>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    int getPlayers() const;
    int getBots() const;
    bool getReserveVisible() const;
    bool getSoundEnabled() const;
    bool getHighlightEnabled() const;
    ~SettingsWindow();
    void loadSettings();


signals:
    void settingsChanged(int players, int bots, bool showReserve, bool soundEnabled, bool highlight);
    void settingsConfirmed(int players, int bots, bool reserve, bool sound, bool highlight);
    void returnToMainMenu();

private slots:
    void updateSettings();
    void saveSettings();
    void validateBots(int playersCount);
    void validatePlayers(int botsCount);
    void onOkClicked();   //  кнопка OK
    void onCancelClicked(); //  кнопка Отмена

private:
    Ui::SettingsWindow *ui;
    QSettings m_settings;
    int currentPlayers = 2;
    int botCounts = 5;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // SETTINGSWINDOW_H
