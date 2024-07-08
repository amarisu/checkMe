#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "addreceiptwindow.h"
#include "receiptwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(ReceiptsDataController& dataController_, QWidget *parent = nullptr);
    ~MainWindow();

public:
    const unsigned char ReceiptWindowsMaxCount;

private:
    Ui::MainWindow* m_ui;
    AddReceiptWindow* m_addReceiptWindow;
    ReceiptsDataController& dataController;

    std::unordered_map<QString, ReceiptWindow*> receiptWindows;
    QMessageBox errMsg;

    //ReceiptsDataController& dataController; // maybe not here at all (mainwindowcontroller)
private:
    void showReceiptWindow(const QString& name);

private slots:
    void onNewButtonClicked();
    void atReceiptCreationSuccess(Receipt& receipt);
    void atReceiptCreationFailure(QString errorText);
};

#endif // MAINWINDOW_H
