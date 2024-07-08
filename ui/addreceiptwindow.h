#ifndef ADD_RECEIPT_WINDOW_H
#define ADD_RECEIPT_WINDOW_H

#include <QDialog>
#include <QMessageBox>
#include "addreceiptwindowcontroller.h"


namespace Ui {
class AddReceiptWindow;
}


class AddReceiptWindow : public QDialog {
    Q_OBJECT
public:
    AddReceiptWindow(ReceiptsDataController& dataController, QWidget *parent = nullptr);
    ~AddReceiptWindow();

    void reset();
    void show();
    void hide();

    void reject() override;
    void accept() override; // for submit maybe


private slots:
    void onUploadImageButtonClicked();
    void onSubmitButtonClicked();

    void onNameFilled();
    void atFieldCheckSuccess();
    void atFieldCheckError(QString errorText);

private:
    QString getReceiptImagePath() const;

private:
    Ui::AddReceiptWindow* m_ui;

    QMessageBox m_errMsg;

    const char* c_explorerWindowTitle;
    const char* c_imageTypes;

    AddReceiptWindowController m_addReceiptWindowControl;
};

#endif
