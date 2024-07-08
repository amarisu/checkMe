#ifndef RECEIPTWINDOW_H
#define RECEIPTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QScrollBar>
#include "qtablewidget.h"
#include "editcustomerswindow.h"
#include "receiptcontroller.h"
#include "checkbox.h"

namespace Ui {
class ReceiptWindow;
}

class ReceiptWindow : public QDialog
{
    Q_OBJECT

public:
    ReceiptWindow(Receipt& receipt, QWidget *parent = nullptr);
    ~ReceiptWindow();

private:
    void tableToEmpty();
    void tableToNormal();
    void clearFields();
    void fillFields();

signals:
    void hidden();
    void displayed();


private slots:
    void onEditCustomersButtonClicked();
    void onEditCustomersSendInfo(std::vector<QString> editedCustomers);
    void onMarksChanged(bool state);
    void tableUpdate();
    void updateTotals();
    void onCheckBoxChanged(Qt::CheckState newState, int row, int column);


protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

private:
    //const Receipt* const m_receipt;   //

    ReceiptController m_receiptController;
    Ui::ReceiptWindow* m_ui;
    QLabel* m_emptyTableText;
    EditCustomersWindow* m_editCustomersWindow;
    QPixmap m_receiptImage;

    const int c_align_horizontal;
    const int c_align_vertical;
    std::vector<std::vector<CheckBox*>> m_marks;
    std::vector<QLabel*> m_itemNames;
    std::vector<QLabel*> m_itemPrices;
    std::vector<QTableWidgetItem*> m_customersNames;
    std::vector<QLabel*> m_totalSums_local;
    QLabel* m_totalSum_global;
    QTableWidgetItem* m_prices_label;
    QTableWidgetItem* m_items_label;


};

#endif // RECEIPTWINDOW_H



