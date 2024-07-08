#include "receipt.h"


Receipt::Receipt(QString imagePath, QString name, std::vector<std::pair<QString, double>> items)
    : isActive(true),
      m_name(name),
      m_receiptImagePath(imagePath),
      m_items(items)
{

    //m_customers["aaa"] = {0, 1};
    //m_customers["bbb"] = {0, 1};

}

const QString& Receipt::getName() const { return m_name; }

void Receipt::remove() { isActive = false; }

const QString& Receipt::getImagePath() const { return m_receiptImagePath; }
