#include "addreceiptwindowcontroller.h"

AddReceiptWindowController::AddReceiptWindowController(AddReceiptWindow& wnd, ReceiptsDataController& dataController, QObject *parent)
    : QObject{parent},
    m_addReceiptWindow(wnd),
    m_dataController(dataController)
{
    for(size_t i = 0; i != static_cast<size_t>(SubwidgetInfo::Type::ENUM_COUNT); i++) {
        auto&[state, text] = m_subwidgetsInfo[static_cast<SubwidgetInfo::Type>(i)];
        state = false;
        text = "";
    }

    //connect(wnd, &AddReceiptWindow::uploadButtonClicked, this, &AddReceiptWindowController::onUploadButtonClicked);

}

void AddReceiptWindowController::checkImageFile(QString filepath){

    if (!filepath.isEmpty()) {

        QFileInfo file(filepath);

        if (file.isFile() && file.exists() && file.isReadable()){
            subwidgetCheckSuccess(SubwidgetInfo::Type::RECEIPT_IMAGE, filepath);
        }
        else
            subwidgetCheckFailure(SubwidgetInfo::Type::RECEIPT_IMAGE, "Cannot open the file \"" + filepath + "\"\n");

    }
    else
        subwidgetCheckFailure(SubwidgetInfo::Type::RECEIPT_IMAGE, "No file selected.\n");



}

void AddReceiptWindowController::resetState(){

    for(auto& i : m_subwidgetsInfo){
        auto&[state, text] = i.second;
        state = false;
        text = "";
    }
}

const SubwidgetInfo::Container& AddReceiptWindowController::getSubwidgetsState() const{
    return m_subwidgetsInfo;
}

bool AddReceiptWindowController::isSubmitButtonEnable() const{

    for(auto& i : m_subwidgetsInfo){
        auto&[state, text] = i.second;
        if(!state)
            return false;
    }
    return true;
}

void AddReceiptWindowController::subwidgetCheckFailure(SubwidgetInfo::Type type, QString errorText){

    auto&[state, text] = m_subwidgetsInfo[type];
    state = false;
    text.clear();
    emit checkFailed(errorText);
}

void AddReceiptWindowController::subwidgetCheckSuccess(SubwidgetInfo::Type type, QString savedText){

    auto&[state, text] = m_subwidgetsInfo[type];
    state = true;
    text = savedText;
    emit checkSuccessed();
}


void AddReceiptWindowController::checkName(QString name){

    if(!name.isEmpty()){
        if(!m_dataController.isReceipt(name))
            subwidgetCheckSuccess(SubwidgetInfo::Type::RECEIPT_NAME, name);

        else subwidgetCheckFailure(SubwidgetInfo::Type::RECEIPT_NAME, "Receipt with this name already exists.\n");
    }
    else
        subwidgetCheckFailure(SubwidgetInfo::Type::RECEIPT_NAME);

}


std::vector<std::pair<QString, double>> AddReceiptWindowController::OCR_response_items(const QString& receiptName){
    std::vector<std::pair<std::string, float>> items_std = m_ocr.recognizeReceipt(receiptName.toStdString());
    std::vector<std::pair<QString, double>> items_qt;
    for(auto& i : items_std){
        items_qt.emplace_back(QString::fromStdString(i.first), i.second);
    }
    return items_qt;
};


void AddReceiptWindowController::sendInfoToCreate(){


    std::vector<std::pair<QString, double>> items_qt = OCR_response_items(m_subwidgetsInfo.at(SubwidgetInfo::Type::RECEIPT_IMAGE).second);;
    //std::vector<std::pair<QString, double>> items_qt = {{"milk", 23.45}, {"milk", 23.45}, {"milk", 23.45}, {"milk", 23.45}, {"milk", 23.45}};

    m_dataController.addReceipt(std::move(m_subwidgetsInfo), std::move(items_qt));
}


size_t AddReceiptWindowController::getReceiptListSize() { return m_dataController.getReceiptListSize(); }

