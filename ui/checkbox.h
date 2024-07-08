#ifndef CHECKBOX_H
#define CHECKBOX_H

#include <QCheckBox>
#include <QTableWidget>

namespace Ui{
class CheckBox;
}


class CheckBox : public QCheckBox
{
    Q_OBJECT
public:
    CheckBox(QTableWidget* parent, int row, int column);
signals:
    void checkStateChangedOnPos(Qt::CheckState newState, int row, int column);
private slots:
    void onCheckState(Qt::CheckState newState);
private:
    QTableWidget* m_parent;
    int m_row;
    int m_column;
};

#endif // CHECKBOX_H
