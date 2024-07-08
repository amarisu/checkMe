#ifndef ADDRECEIPTWINDOWCONTROLLER_H
#define ADDRECEIPTWINDOWCONTROLLER_H

#include <QObject>
#include <QFileDialog>
#include "subwidgettype.h"
#include "receiptsdatacontroller.h"
#include "OCR.h"


namespace CheckMe{
class AddReceiptWindowController;
}

class AddReceiptWindow;

class AddReceiptWindowController : public QObject
{

    Q_OBJECT


public:
    AddReceiptWindowController(AddReceiptWindow& wnd, ReceiptsDataController& dataController, QObject *parent = nullptr);

    void checkImageFile(QString filepath);
    void checkName(QString name);
    bool isSubmitButtonEnable() const;
    const SubwidgetInfo::Container& getSubwidgetsState() const;
    void sendInfoToCreate();
    void resetState();

                                                            size_t getReceiptListSize();        // debug's

signals:
    void checkFailed(QString errorText = "");
    void checkSuccessed();

    void sendReceiptInfo(std::unordered_map<SubwidgetInfo::Type, std::pair<bool, QString>> info);

private:
    void subwidgetCheckFailure(SubwidgetInfo::Type type, QString errorText = "");
    void subwidgetCheckSuccess(SubwidgetInfo::Type type, QString savedText);
    std::vector<std::pair<QString, double>> OCR_response_items(const QString& receiptName);

private:
    AddReceiptWindow& m_addReceiptWindow;
    ReceiptsDataController& m_dataController;

    SubwidgetInfo::Container m_subwidgetsInfo;
    OCRModule::OCR m_ocr;
};

#endif // ADDRECEIPTWINDOWCONTROLLER_H
