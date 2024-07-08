#ifndef RECEIPT_H
#define RECEIPT_H

#include <QString>
#include <QImage>
#include <QDateTime>
#include <QHash>

#include "totalcalculator.h"


QT_BEGIN_NAMESPACE
namespace CheckMe {
    class Receipt;
}
QT_END_NAMESPACE


class Receipt
{

    friend class ReceiptController;

public:
    Receipt(QString imagePath, QString name, std::vector<std::pair<QString, double>> items);
    const QString& getName() const;
    const QString& getImagePath() const;
    void addCustomer(QString customer);
    void removeCustomer(QString customer);
    void remove();


public:
    bool isActive;

private:
    QString m_name;
    QString m_receiptImagePath;

    std::unordered_map<QString, std::unordered_map<char, bool>> m_customers;    // - mark
    std::vector<std::pair<QString, double>> m_items;


#if 0
    //const QDateTime m_creationTime;
    //QString m_purchaseDate;
    //unsigned char pos;

private:
    //void changePos();   // rearrange() вызывает changePos для всех чеков в хеш таблице
#endif

};


#endif // RECEIPT_H
