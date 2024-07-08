#include "receiptwindow.h"
#include "ui_receiptwindow.h"

ReceiptWindow::ReceiptWindow(Receipt& receipt, QWidget *parent)
    : QDialog(parent),
    m_receiptController(receipt, this),
    m_ui(new Ui::ReceiptWindow),
    m_emptyTableText(new QLabel("No customers yet!")),
    m_editCustomersWindow(new EditCustomersWindow(this)),
    m_receiptImage(receipt.getImagePath()),
    c_align_horizontal(2),
    c_align_vertical(1),

    m_totalSum_global(nullptr),
    m_prices_label(new QTableWidgetItem()),
    m_items_label(new QTableWidgetItem())
{
    m_ui->setupUi(this);

    m_emptyTableText->setFont({"Sitka", 8});
    m_emptyTableText->setStyleSheet("color: #FF6347;");
    m_emptyTableText->setAlignment(Qt::AlignCenter);

    setWindowTitle(receipt.getName());

    m_ui->label_receiptImage->setFixedSize(m_receiptImage.size());
    m_ui->scrollAreaWidgetContents_receiptImage->setFixedSize({m_receiptImage.width(), m_receiptImage.height()});
    m_ui->scrollArea_receiptImage->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_ui->scrollArea_receiptImage->setFixedSize({220, 250});
    m_ui->label_receiptImage->setPixmap(m_receiptImage);
    m_ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_ui->scrollArea_receiptImage->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    m_ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //m_ui->tableWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    m_ui->tableWidget->setStyleSheet("QTableWidget { border: 1px solid #A9A9A9; }");
    m_ui->tableWidget->horizontalHeader()->setFixedHeight(24);
    m_ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color:#FFA07A; color:#FFFFFF; border: 1px solid #FF7F50; }");
    //m_ui->tableWidget->verticalHeader()->setSectionResizeMode (QHeaderView::Fixed);
    //m_ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_ui->tableWidget->setFocusPolicy(Qt::StrongFocus);
    m_ui->tableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    m_ui->tableWidget->setAutoScroll(false);
    m_ui->tableWidget->setRowCount(1);
    m_ui->tableWidget->setColumnCount(1);

    tableUpdate();


    if(parent){
        resize(parent->size());
        setSizePolicy({QSizePolicy::Fixed, QSizePolicy::Fixed});
        move(parent->pos());
    }

    connect(m_ui->editCustomersButton, &QPushButton::clicked, this, &ReceiptWindow::onEditCustomersButtonClicked);
    connect(m_editCustomersWindow, &EditCustomersWindow::sendInfo, this, &ReceiptWindow::onEditCustomersSendInfo);

}

ReceiptWindow::~ReceiptWindow()
{
    delete m_ui;
    delete m_editCustomersWindow;
    delete m_emptyTableText;
    delete m_prices_label;
    delete m_items_label;
}

void ReceiptWindow::hideEvent(QHideEvent* event){

    emit hidden();
    QDialog::hideEvent(event);
}

void ReceiptWindow::showEvent(QShowEvent* event){

    emit displayed();
    QDialog::showEvent(event);
}


void ReceiptWindow::onEditCustomersButtonClicked(){

    m_editCustomersWindow->fill(m_receiptController.readCustomers());
    if(m_editCustomersWindow->isValid)
        m_editCustomersWindow->show();
}


void ReceiptWindow::onEditCustomersSendInfo(std::vector<QString> editedCustomers){  // unique values

    m_receiptController.editCustomers(editedCustomers);
    qDebug() << "edit customers slot\n";
    tableUpdate();
}

void ReceiptWindow::tableToEmpty(){

    m_ui->tableWidget->horizontalHeader()->setVisible(false);
    m_ui->tableWidget->verticalHeader()->setVisible(false);
    m_ui->tableWidget->setRowCount(1);
    m_ui->tableWidget->setColumnCount(1);
    m_ui->tableWidget->setCellWidget(0, 0, m_emptyTableText);

    m_ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_ui->tableWidget->resize(m_ui->tableWidget->baseSize());
    m_ui->tableWidget->verticalHeader()->resize(m_ui->tableWidget->baseSize());
    m_ui->tableWidget->horizontalHeader()->resize(m_ui->tableWidget->baseSize());
}


void ReceiptWindow::tableToNormal(){

    m_ui->tableWidget->horizontalHeader()->setVisible(true);
    m_ui->tableWidget->verticalHeader()->setVisible(true);
    m_ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

void ReceiptWindow::tableUpdate(){


    m_ui->tableWidget->setRowCount(m_receiptController.getItemsSize() + c_align_vertical + c_align_vertical);
    m_ui->tableWidget->setColumnCount(m_receiptController.getCustomersSize() + c_align_horizontal);
    //m_ui->tableWidget->horizontalHeader()->setSectionResizeMode(m_ui->tableWidget->rowCount()-1, QHeaderView::Fixed);     ///

    m_ui->tableWidget->setColumnWidth(0, 250);
    m_ui->tableWidget->setColumnWidth(1, 60);

    clearFields();

    fillFields();

    m_ui->tableWidget->resizeRowsToContents();
    m_ui->tableWidget->resizeColumnsToContents();

}

void ReceiptWindow::onMarksChanged(bool state){

    // обновление итогов!
}


void ReceiptWindow::clearFields(){

    m_ui->tableWidget->hide();

    m_ui->tableWidget->clearContents();

    for(size_t i = 0; i < m_marks.size(); ++i){
        for(auto j : m_marks[i])    ///
            delete j;
        m_marks[i].clear();
    }
    m_marks.clear();

    for(auto& i : m_itemNames){
        delete i;
    }
    m_itemNames.clear();

    for(auto& i : m_itemPrices){
        delete i;
    }
    m_itemPrices.clear();

    for(auto& i : m_customersNames){
        delete i;
    }
    m_customersNames.clear();

    for(auto& i : m_totalSums_local){
        delete i;
    }
    m_totalSums_local.clear();

    delete m_totalSum_global;
    m_totalSum_global = nullptr;
}


void ReceiptWindow::fillFields(){

    m_prices_label->setText("Price");
    m_prices_label->setFont({"Sitka"});
    m_items_label->setText("Items");
    m_items_label->setFont({"Sitka"});

    m_ui->tableWidget->setHorizontalHeaderItem(0, m_items_label);
    m_ui->tableWidget->setHorizontalHeaderItem(1, m_prices_label);
    m_ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);

    auto& items = m_receiptController.readItems();
    auto& customers = m_receiptController.readCustomers();
    size_t itemsSize = m_receiptController.getItemsSize();
    size_t customersSize = m_receiptController.getCustomersSize();


    m_itemNames.resize(itemsSize);
    m_itemPrices.resize(itemsSize);
    double totalSum = 0;

    for(size_t i = 0; i < itemsSize; ++i){

        m_itemNames[i] = new QLabel();
        m_itemNames[i]->setText(QString((items[i].first.size() > 16) ? items[i].first.chopped(16) : items[i].first));
        m_itemNames[i]->setFont({"Sitka"});
        m_ui->tableWidget->setCellWidget(i, 0, m_itemNames[i]); //

        m_itemPrices[i] = new QLabel();
        m_itemPrices[i]->setText(QString::number(items[i].second, 'f', 2));
        m_itemPrices[i]->setAlignment(Qt::AlignCenter);
        m_itemPrices[i]->setFont({"Sitka"});
        m_ui->tableWidget->setCellWidget(i, 1, m_itemPrices[i]); //

        totalSum += items[i].second;
    }

    m_totalSum_global = new QLabel();
    m_totalSum_global->setText(QString::number(totalSum, 'f', 2));
    m_totalSum_global->setAlignment(Qt::AlignCenter);
    m_totalSum_global->setFont({"Sitka"});
    m_ui->tableWidget->setCellWidget(m_ui->tableWidget->rowCount()-1, 1, m_totalSum_global);


    m_customersNames.resize(customersSize);
    m_totalSums_local.resize(customersSize);
    m_marks.resize(customersSize);


    auto it = customers.begin();

    std::vector<int> checkCount(itemsSize, 0);
    for(size_t i = 0; i < customersSize; ++i){

        m_customersNames[i] = new QTableWidgetItem;
        m_customersNames[i]->setText(it->first);
        m_customersNames[i]->setFont({"Sitka"});
        m_ui->tableWidget->setHorizontalHeaderItem(i+c_align_horizontal, m_customersNames[i]);

        m_marks[i].resize(itemsSize);
        for(size_t j = 0; j<itemsSize; ++j){

            m_marks[i][j] = new CheckBox(m_ui->tableWidget, j, i);

            it->second.at(j) ? m_marks[i][j]->setCheckState(Qt::Checked) : m_marks[i][j]->setCheckState(Qt::Unchecked);

            if(m_marks[i][j]->checkState())
                checkCount[j]++;

            connect(m_marks[i][j], &CheckBox::checkStateChangedOnPos, this, &ReceiptWindow::onCheckBoxChanged);
            m_ui->tableWidget->setCellWidget(j, i+c_align_horizontal, m_marks[i][j]);

        }

        m_totalSums_local[i] = new QLabel;
        m_totalSums_local[i]->setAlignment(Qt::AlignCenter);
        m_totalSums_local[i]->setFont({"Sitka"});
        m_ui->tableWidget->setCellWidget(m_ui->tableWidget->rowCount()-1, i+c_align_horizontal, m_totalSums_local[i]);
        it++;

    }


    it = customers.begin();
    for(size_t i = 0; i < customersSize; ++i){
        double customerTotal = m_receiptController.calc(it->first, checkCount);
        qDebug() << QString::number(customerTotal, 'f', 2) << "\n";
        m_totalSums_local[i]->setText(QString::number(customerTotal, 'f', 2));
        it++;
    }




    m_ui->tableWidget->setStyleSheet("QTableWidget { border: 1px solid #A9A9A9; }");
    m_ui->tableWidget->setShowGrid(false);
    m_ui->tableWidget->show();
}



void ReceiptWindow::onCheckBoxChanged(Qt::CheckState newState, int row, int column){

    QString customer = m_ui->tableWidget->horizontalHeaderItem(column+c_align_horizontal)->text();      //
    m_receiptController.changeState(std::move(customer), row, newState);
    updateTotals();
}


void ReceiptWindow::updateTotals(){

    size_t itemsSize = m_receiptController.getItemsSize();
    size_t customersSize = m_receiptController.getCustomersSize();
    auto& customers = m_receiptController.readCustomers();


    auto it = customers.begin();

    std::vector<int> checkCount(itemsSize, 0);
    for(size_t i = 0; i < customersSize; ++i){

        for(size_t j = 0; j<itemsSize; ++j){

            if(m_marks[i][j]->checkState()){
                qDebug() << checkCount[j];
                checkCount[j]++;
            }

        }

        it++;
    }


    it = customers.begin();
    for(size_t i = 0; i < customersSize; ++i){
        double customerTotal = m_receiptController.calc(it->first, checkCount);
        qDebug() << QString::number(customerTotal, 'f', 2) << "\n";
        m_totalSums_local[i]->setText(QString::number(customerTotal, 'f', 2));
        it++;
    }
}








