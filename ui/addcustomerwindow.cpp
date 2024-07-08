#include "addcustomerwindow.h"
#include "ui_addcustomerwindow.h"

AddCustomerWindow::AddCustomerWindow(QWidget *parent)
    : QDialog(parent),
    m_ui(new Ui::AddCustomerWindow)
{
    m_ui->setupUi(this);
    m_ui->lineEdit->setMaxLength(13);
    m_ui->OKButton->setDisabled(true);
    connect(m_ui->lineEdit, &QLineEdit::textEdited, this, &AddCustomerWindow::onNameTextChanged);
    connect(m_ui->OKButton, &QPushButton::clicked, this, &AddCustomerWindow::onOKButtonClicked);
    connect(m_ui->cancelButton, &QPushButton::clicked, this, &AddCustomerWindow::onCancelButtonClicked);
}

AddCustomerWindow::~AddCustomerWindow()
{
    delete m_ui;
}


void AddCustomerWindow::reset(){

    m_ui->lineEdit->clear();
}

void AddCustomerWindow::onOKButtonClicked(){

    emit nameAccepted(m_ui->lineEdit->text());
    reset();
}

void AddCustomerWindow::onCancelButtonClicked(){

    reset();
    emit nameCancelled();
}

void AddCustomerWindow::onNameTextChanged(){

    (m_ui->lineEdit->text() == "") ? m_ui->OKButton->setDisabled(true) : m_ui->OKButton->setDisabled(false);
}
