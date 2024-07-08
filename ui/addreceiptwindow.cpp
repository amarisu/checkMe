#include "addreceiptwindow.h"
#include "ui_addreceiptwindow.h"

AddReceiptWindow::AddReceiptWindow(ReceiptsDataController& dataController, QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AddReceiptWindow),
    m_errMsg(this),
    c_explorerWindowTitle("Выберите изображение"),
    c_imageTypes("Изображение (*.png *.jpg *.jpeg)"),
    m_addReceiptWindowControl(*this, dataController, this)
{
    m_ui->setupUi(this);
    m_ui->submitButton->setDisabled(true);
    m_ui->submitButton->setFocusPolicy(Qt::NoFocus);
    m_ui->uploadReceiptButton->setFocusPolicy(Qt::NoFocus);

    setFocusPolicy(Qt::ClickFocus);

    connect(&m_addReceiptWindowControl, &AddReceiptWindowController::checkSuccessed, this, &AddReceiptWindow::atFieldCheckSuccess);
    connect(&m_addReceiptWindowControl, &AddReceiptWindowController::checkFailed, this, &AddReceiptWindow::atFieldCheckError);

    connect(m_ui->uploadReceiptButton, &QPushButton::clicked, this, &AddReceiptWindow::onUploadImageButtonClicked);
    connect(m_ui->receiptNameLineEdit, &QLineEdit::editingFinished, this, &AddReceiptWindow::onNameFilled);
    connect(m_ui->receiptNameLineEdit, &QLineEdit::returnPressed, this, &AddReceiptWindow::onNameFilled);
    connect(m_ui->submitButton, &QPushButton::clicked, this, &AddReceiptWindow::onSubmitButtonClicked);

}

AddReceiptWindow::~AddReceiptWindow()
{
    delete m_ui;
}


void AddReceiptWindow::reject(){

    reset();
    QDialog::reject();
}


void AddReceiptWindow::accept(){
    QDialog::accept();
}


void AddReceiptWindow::reset(){

    m_addReceiptWindowControl.resetState();
    m_ui->submitButton->setDisabled(true);
    m_ui->receiptNameLineEdit->clear();

}



void AddReceiptWindow::show(){

    QDialog::show();

    qDebug() << m_addReceiptWindowControl.getReceiptListSize() <<"\n";

}


void AddReceiptWindow::hide(){

    reset();
    QDialog::hide();
}


void AddReceiptWindow::onUploadImageButtonClicked(){

    m_addReceiptWindowControl.checkImageFile(getReceiptImagePath());
}


void AddReceiptWindow::onSubmitButtonClicked(){

    m_addReceiptWindowControl.sendInfoToCreate();
    hide();
}


void AddReceiptWindow::onNameFilled(){      //

    setFocus();
    m_addReceiptWindowControl.checkName(m_ui->receiptNameLineEdit->text());
}


void AddReceiptWindow::atFieldCheckSuccess(){

    m_ui->submitButton->setDisabled(!m_addReceiptWindowControl.isSubmitButtonEnable());

}


void AddReceiptWindow::atFieldCheckError(QString errorText){

    m_ui->submitButton->setDisabled(true);

    if(!errorText.isEmpty())
        m_errMsg.critical(this, "Warning", errorText);
}


QString AddReceiptWindow::getReceiptImagePath() const{

    QString path = QFileDialog::getOpenFileName(0,QObject::tr(c_explorerWindowTitle),QDir::homePath(), QObject::tr(c_imageTypes));

    return path;
}

