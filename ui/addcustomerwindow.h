#ifndef ADDCUSTOMERWINDOW_H
#define ADDCUSTOMERWINDOW_H

#include <QDialog>

namespace Ui {
class AddCustomerWindow;
}

class AddCustomerWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AddCustomerWindow(QWidget *parent = nullptr);
    ~AddCustomerWindow();
    void reset();

private slots:
    void onOKButtonClicked();
    void onCancelButtonClicked();
    void onNameTextChanged();

signals:
    void nameCancelled();
    void nameAccepted(QString name);
private:
    Ui::AddCustomerWindow *m_ui;
};

#endif // ADDCUSTOMERWINDOW_H
