#include "editcustomerswindow.h"
#include "ui_editcustomerswindow.h"

char EditCustomersWindow::tablePos = 0;

EditCustomersWindow::EditCustomersWindow(QWidget *parent)
    : QDialog(parent),
    isValid(false),
    m_ui(new Ui::EditCustomersWindow),
    m_addCustomerWindow(new AddCustomerWindow(this))
{
    m_ui->setupUi(this);
    m_ui->pushButton_saveDelete->hide();

    m_ui->tableWidget->horizontalHeader()->setVisible(false);
    m_ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    m_ui->tableWidget->horizontalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    m_ui->tableWidget->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    m_ui->tableWidget->setFocusPolicy(Qt::NoFocus);


    connect(m_ui->pushButton_add, &QPushButton::clicked, this, &EditCustomersWindow::onAddButtonClicked);
    connect(m_ui->pushButton_delete, &QPushButton::clicked, this, &EditCustomersWindow::onDeleteButtonClicked);
    connect(m_ui->pushButton_save, &QPushButton::clicked, this, &EditCustomersWindow::onSaveButtonClicked);
    connect(m_ui->pushButton_saveDelete, &QPushButton::clicked, this, &EditCustomersWindow::onSaveDeleteButtonClicked);
    connect(m_addCustomerWindow, &AddCustomerWindow::nameAccepted, this, &EditCustomersWindow::atNameAccepted);
    connect(m_addCustomerWindow, &AddCustomerWindow::nameCancelled, this, &EditCustomersWindow::atNameCancelled);

}

EditCustomersWindow::~EditCustomersWindow()
{
    delete m_ui;
    delete m_addCustomerWindow;
}


void EditCustomersWindow::hideEvent(QHideEvent* event){

    m_ui->tableWidget->clear();
    m_ui->tableWidget->setRowCount(0);
    m_ui->tableWidget->setColumnCount(0);

    destructAllNames();
    tablePos = 0;
    isValid = false;

    QDialog::hideEvent(event);
}


void EditCustomersWindow::fill(const std::unordered_map<QString, std::unordered_map<char,bool>>& customers){

    m_ui->tableWidget->setRowCount(customers.size());
    m_ui->tableWidget->setColumnCount(1);

    for(auto& i: customers)
        addName(i.first);

    isValid = true;
}

void EditCustomersWindow::showEvent(QShowEvent* event){

    QDialog::showEvent(event);
}



void EditCustomersWindow::onDeleteButtonClicked(){

    m_ui->pushButton_saveDelete->show();

    m_ui->pushButton_add->hide();
    m_ui->pushButton_save->hide();
    m_ui->pushButton_delete->hide();


    for(auto i : m_currentCustomers)
        i->setCheckable(true);

}



void EditCustomersWindow::onSaveDeleteButtonClicked(){

    m_ui->pushButton_saveDelete->hide();
    m_ui->pushButton_add->show();
    m_ui->pushButton_delete->show();
    m_ui->pushButton_save->show();


    for(auto i : m_currentCustomers){
        if(i->isChecked())
            i->setDisabled(true); //i->setStyleSheet( (i->styleSheet() == "color: black;") ? "color: grey;" :  "color: black;");

        i->setCheckable(false);
    }
}


void EditCustomersWindow::onSaveButtonClicked(){

    for(auto& i : m_currentCustomers){
        if(!i->isEnabled()){

            auto it = std::find(m_editedCustomers.begin(), m_editedCustomers.end(), i->text());
            if(it == m_editedCustomers.end())
                m_editedCustomers.push_back(i->text());
            else
                m_editedCustomers.erase(it);
        }
    }

    emit sendInfo(std::move(m_editedCustomers));
    qDebug() << "save button\n";
    hide();
}

void EditCustomersWindow::onAddButtonClicked(){

    m_addCustomerWindow->show();
}

void EditCustomersWindow::atNameCancelled(){

    m_addCustomerWindow->hide();
}


void EditCustomersWindow::addName(QString text){

    m_ui->tableWidget->setRowCount(tablePos+1);

    QPushButton* customer = new QPushButton(text, this);
    customer->setFlat(true);
    customer->setFocusPolicy(Qt::FocusPolicy::NoFocus);
    customer->setMinimumHeight(20);
    customer->setMinimumWidth(110);
    customer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);

    m_currentCustomers.push_back(customer);

    m_ui->tableWidget->setColumnWidth(0, customer->minimumWidth());
    m_ui->tableWidget->setCellWidget(tablePos, 0, customer);
    m_ui->tableWidget->update();

    //m_newCustomers.emplace_back(text);

    tablePos++;
}


void EditCustomersWindow::destructAllNames(){

    for(auto button : m_currentCustomers){
        disconnect(button, nullptr, nullptr, nullptr);
        delete button;
    }


    m_currentCustomers.clear();
    m_editedCustomers.clear();
}


void EditCustomersWindow::atNameAccepted(QString text){

    m_addCustomerWindow->hide();

    bool isAlreadyExist = false;
    for(size_t i = 0; i < m_currentCustomers.size(); ++i){
        if (m_currentCustomers[i]->text() == text){
            isAlreadyExist = true;
            break;
        }
    }

    if (isAlreadyExist) {
        msg.information(this, "Warning", "Customer already exists.");
        return;
    }

    addName(text);
    m_editedCustomers.emplace_back(text);

}




