#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(ReceiptsDataController& dataController_, QWidget *parent)
    : QMainWindow(parent),
    ReceiptWindowsMaxCount(50),
    m_ui(new Ui::MainWindow),
    m_addReceiptWindow(new AddReceiptWindow(dataController_, this)),
    dataController(dataController_)
{
    receiptWindows.reserve(ReceiptWindowsMaxCount);
    m_ui->setupUi(this);

    connect(m_ui->newButton, &QPushButton::clicked, this, &MainWindow::onNewButtonClicked);
    connect(&dataController, &ReceiptsDataController::creatingReceiptFailure, this, &MainWindow::atReceiptCreationFailure);
    connect(&dataController, &ReceiptsDataController::creatingReceiptSuccess, this, &MainWindow::atReceiptCreationSuccess);
}

MainWindow::~MainWindow()
{
    for(auto& i: receiptWindows)
        delete i.second;
    delete m_ui;
    delete m_addReceiptWindow;
}


void MainWindow::onNewButtonClicked(){

    m_addReceiptWindow->show();
}


void MainWindow::atReceiptCreationSuccess(Receipt& receipt) {

    QString receiptName = receipt.getName();


    receiptWindows.emplace(receiptName, new ReceiptWindow(receipt, this));
    connect(receiptWindows.at(receiptName), &ReceiptWindow::displayed, this, &QWidget::hide);
    connect(receiptWindows.at(receiptName), &ReceiptWindow::hidden, this, &QWidget::show);
    //connect() с dataController

    showReceiptWindow(receiptName);
}

void MainWindow::showReceiptWindow(const QString& name){    // потому что show можно не только при создании

    if(receiptWindows.contains(name)){

        receiptWindows.at(name)->show();
    }
}


//void MainWindow:atReceiptDisabled(QString receiptName) { receiptsWindows.at(receiptName).hide(); }


void MainWindow::atReceiptCreationFailure(QString errorText) {

    errMsg.critical(this, "Error", errorText);
}

