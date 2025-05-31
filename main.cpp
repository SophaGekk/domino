#include <QMainWindow>
#include <QApplication>
#include <QGraphicsScene>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QFile>
#include <QVBoxLayout>

#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDockWidget>



#include "domino_game.h"
#include "player.h"
#include "domino_tile.h"
#include "mainwindow.h"
#include "bazaar.h"
#include "test.h"


int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow  w;
    runTests();
    w.show();

    return a.exec();
}
