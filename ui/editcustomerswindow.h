#ifndef EDITCUSTOMERSWINDOW_H
#define EDITCUSTOMERSWINDOW_H

#include <QDialog>
#include "mark.h"
#include "addcustomerwindow.h"
#include "qmessagebox.h"


namespace Ui {
class EditCustomersWindow;
}

class EditCustomersWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EditCustomersWindow(QWidget *parent = nullptr);
    ~EditCustomersWindow();
    void fill(const std::unordered_map<QString, std::unordered_map<char,bool>>& customers);

protected:
    void hideEvent(QHideEvent* event) override;
    void showEvent(QShowEvent* event) override;

signals:
    void sendInfo(std::vector<QString> customers);    // если нет в таблице - добавить, если есть - удалить.
    //void displayed();

private slots:
    void onDeleteButtonClicked();
    void onSaveDeleteButtonClicked();
    void onSaveButtonClicked();
    void onAddButtonClicked();
    void atNameCancelled();
    void atNameAccepted(QString text);

private:
    void addName(QString name);
    void destructAllNames();


signals:

public:
    bool isValid;

private:
    Ui::EditCustomersWindow *m_ui;
    AddCustomerWindow* m_addCustomerWindow;
    std::vector<QString> m_editedCustomers;
    std::vector<QPushButton*> m_currentCustomers;
    QMessageBox msg;
    static char tablePos;

};

#endif // EDITCUSTOMERSWINDOW_H
