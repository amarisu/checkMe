#ifndef OCRMODULE_OCR_H
#define OCRMODULE_OCR_H

#include "RequestSender.h"
#include "ResponseHandler.h"

#include <string>
#include <vector>
#include <utility>

namespace OCRModule {

    class OCR {
    private:
        std::string apikey;
        std::string api_url;
        int status;
        OCRModule::RequestSender sender;
        OCRModule::ResponseHandler handler;

    public:
        OCR();
        int getStatus();
        std::vector<std::pair<std::string, float>> recognizeReceipt(const std::string& image_path);
    };

} // OCRModule

#endif //OCRMODULE_OCR_H
