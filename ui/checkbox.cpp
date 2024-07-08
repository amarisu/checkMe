#include "checkbox.h"

CheckBox::CheckBox(QTableWidget *parent, int row, int column)
    : QCheckBox(parent),
    m_parent(parent),
    m_row(row),
    m_column(column)
{
    connect(this, &CheckBox::checkStateChanged, this,  &CheckBox::onCheckState);
}


void CheckBox::onCheckState(Qt::CheckState newState){

    if(!m_parent)
        return;

    emit checkStateChangedOnPos(newState, m_row, m_column);
}
