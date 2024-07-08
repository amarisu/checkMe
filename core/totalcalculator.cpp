#include "totalcalculator.h"
#include "qdebug.h"

TotalCalculator::TotalCalculator(std::unordered_map<QString, std::unordered_map<char, bool>>& customers,
                                 std::vector<std::pair<QString, double>>& items)
    : m_customers(customers),
      m_items(items)
{}


double TotalCalculator::total(const QString& customer, std::vector<int> checksOnRows) const{   // строка

    if(!m_customers.contains(customer)){
        return 0.0;
    }

    double sum = 0.0;

    auto checkedItems = m_customers.at(customer);

    for(size_t i=0; i < m_items.size(); ++i){

        int cnt = checksOnRows[i];

        if(!cnt)
            continue;

        double multiplier = 1/double(cnt);

        if(m_customers.at(customer).at(i))
            sum += m_items[i].second * multiplier;
    }

    return sum;
}
