#include "ColorTest.h"
#include <QApplication>
#include "ColorDialog.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();
    mainwindow.config();
//    ColorDialog dialog;
//    dialog.showDialog();

//    ColorTest w;
//    w.show();

    return a.exec();
}
