#ifndef RECEIPTSDATACONTROLLER_H
#define RECEIPTSDATACONTROLLER_H

#include <QObject>
#include <unordered_map>
#include "receipt.h"

#include "subwidgettype.h"


namespace CheckMe{
class ReceiptsDataController;
}


class ReceiptsDataController : public QObject
{
    Q_OBJECT
public:
    explicit ReceiptsDataController(QObject *parent = nullptr);

    const std::unordered_map<QString, Receipt>& getReceipts() const;;
    const Receipt& getReceiptByName(const QString& name) const;
    size_t getReceiptListSize() const;
    bool isReceipt(const QString& name) const;

public slots:
    void addReceipt(const SubwidgetInfo::Container& stringInfo, std::vector<std::pair<QString, double>> items);
    //void removeReceipt(QString name);

signals:
    void creatingReceiptFailure(QString errorText);
    void creatingReceiptSuccess(Receipt& receipt);

private:
    std::unordered_map<QString, Receipt> m_receipts;
};

#endif // RECEIPTSDATACONTROLLER_H
