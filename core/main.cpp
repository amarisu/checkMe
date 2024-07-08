#include <QApplication>

#include "mainwindow.h"
#include "receiptsdatacontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReceiptsDataController dataController;
    MainWindow w(dataController);
    w.show();
    return a.exec();
}
