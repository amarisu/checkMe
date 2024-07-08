#include "receiptcontroller.h"

ReceiptController::ReceiptController(Receipt& receipt, QObject *parent)
    : QObject{parent},
    m_receipt(receipt),
    m_calc(receipt.m_customers, receipt.m_items)
{}


size_t ReceiptController::getItemsSize() const { return m_receipt.m_items.size(); }

size_t ReceiptController::getCustomersSize() const { return m_receipt.m_customers.size(); }

const std::unordered_map<QString, std::unordered_map<char, bool>>& ReceiptController::readCustomers() const { return m_receipt.m_customers; }

const std::vector<std::pair<QString, double>>& ReceiptController::readItems() const { return m_receipt.m_items; }


void ReceiptController::editCustomers(std::vector<QString> editedCustomers){

 for(auto& i : editedCustomers){

    if(readCustomers().contains(i)){
        qDebug() << "read\n";
        m_receipt.m_customers.erase(i);
        qDebug() << "erase\n";
    }

    else{
        std::unordered_map<char,bool> checks;
        for(size_t i = 0; i<getItemsSize(); ++i){
            checks[i] = false;
        }
        qDebug() << "cycle\n";
        m_receipt.m_customers.emplace(i, std::move(checks));
        qDebug() << "move\n";
        }
    }
}


double ReceiptController::calc(const QString& customer, std::vector<int> checksOnRows){

    return m_calc.total(customer, checksOnRows);
}


void ReceiptController::changeState(QString customer, int pos, bool newState){

    auto& i = m_receipt.m_customers.at(customer);
    i.at(pos) = newState;
}




