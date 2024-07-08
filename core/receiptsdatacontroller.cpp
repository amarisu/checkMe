#include "receiptsdatacontroller.h"

ReceiptsDataController::ReceiptsDataController(QObject *parent)
    : QObject{parent}
{}


void ReceiptsDataController::addReceipt(const SubwidgetInfo::Container& stringInfo, std::vector<std::pair<QString, double>> items) {

    if(stringInfo.size() == static_cast<size_t>(SubwidgetInfo::Type::ENUM_COUNT)){

        QString imagePath = stringInfo.at(SubwidgetInfo::Type::RECEIPT_IMAGE).second;
        QString name = stringInfo.at(SubwidgetInfo::Type::RECEIPT_NAME).second;

        if(!items.empty()){
            m_receipts.emplace(name, Receipt(imagePath, name, items));
            emit creatingReceiptSuccess(m_receipts.at(name));
        }
        else emit creatingReceiptFailure("OCR module error. Receipt not created.\n");

    }

    else emit creatingReceiptFailure("Internal error. Receipt not created.\n");
}


//void ReceiptsDataController::loadItems(std::vector<double> items);


bool ReceiptsDataController::isReceipt(const QString& name) const { return m_receipts.contains(name); }

const Receipt& ReceiptsDataController::getReceiptByName(const QString& name) const{  return m_receipts.at(name); } // аккуратно с наличием (возможно стоит возвр. указатель)

size_t ReceiptsDataController::getReceiptListSize() const { return m_receipts.size(); }
