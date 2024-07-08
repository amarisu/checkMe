#ifndef TOTALCALCULATOR_H
#define TOTALCALCULATOR_H

#include <QString>
#include <QHash>

namespace CheckMe{
class TotalCalculator;
}

class TotalCalculator
{
public:
    TotalCalculator(std::unordered_map<QString, std::unordered_map<char, bool>>& customers, std::vector<std::pair<QString, double>>& m_items);
    double total(const QString& customer, std::vector<int> checksOnRows) const;

private:
    std::unordered_map<QString, std::unordered_map<char, bool>>& m_customers;
    std::vector<std::pair<QString, double>>& m_items;
};

#endif // TOTALCALCULATOR_H
