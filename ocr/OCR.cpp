#include "OCR.h"

#define SUCCESS 0;
#define REQUEST_ERROR 1;
#define PROCESS_ERROR 2;


namespace OCRModule {
    OCR::OCR(): sender(), handler() {
         /* По-хорошему эти данные должны быть защищены, но пока что не важно.
         * (В худшем случае, утащив apikey, злоумышленники смогут потратить мои обращения к апи)
         */
        OCR::apikey = "K85398354788957";
        OCR::api_url = "https://api.ocr.space/parse/image";
        OCR::status = SUCCESS;
        sender.setApi(apikey, api_url);
    }

    std::vector<std::pair<std::string, float>> OCR::recognizeReceipt(const std::string& image_path) {
        status = SUCCESS;

        nlohmann::json response = sender.postRequest(image_path);
        if (sender.isError()) {
            status = REQUEST_ERROR; // Critical error (no response from API)
            return {};
        }

        auto receipt = handler.processResponse(response);
        if (handler.isError())
            status = PROCESS_ERROR; // Non-critical error (may be positions with empty "name")

        return receipt;
    }

    int OCR::getStatus() {
        // Возвращает статус последнего запроса (0 - успех, 1 - ошибка запроса, 2 - ошибка обработки)
        return status;
    }

} // OCRModule
