#ifndef RECEIPTCONTROLLER_H
#define RECEIPTCONTROLLER_H

#include <QObject>
#include "receipt.h"

namespace CheckMe{
    class ReceiptController;
}

class ReceiptController : public QObject
{
    Q_OBJECT
public:
    ReceiptController(Receipt& receipt, QObject *parent = nullptr);

    size_t getItemsSize() const;
    size_t getCustomersSize() const;
    const std::unordered_map<QString, std::unordered_map<char, bool>>& readCustomers() const;
    const std::vector<std::pair<QString, double>>& readItems() const;
    void editCustomers(std::vector<QString> editedCustomers);
    double calc(const QString& customer, std::vector<int> checksOnRows);
    void changeState(QString customer, int pos, bool newState);

signals:

private:
    Receipt& m_receipt;
    TotalCalculator m_calc;

};

#endif // RECEIPTCONTROLLER_H
