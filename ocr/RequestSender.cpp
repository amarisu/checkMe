#include "RequestSender.h"

namespace OCRModule {

    RequestSender::RequestOptions::RequestOptions() {
        RequestOptions::scale = "true";
        RequestOptions::isTable = "true";
        RequestOptions::engine = "2";
        RequestOptions::isOverlay = "false";
        RequestOptions::orient = "true";
    }

    void RequestSender::RequestOptions::setOptions(bool scale, bool istable, std::string engine, bool overlay, bool orient) {
        RequestOptions::scale = scale ? "true" : "false";
        RequestOptions::isTable = istable ? "true" : "false";
        RequestOptions::engine = engine;
        RequestOptions::isOverlay = overlay ? "true" : "false";
        RequestOptions::orient = orient ? "true" : "false";
    }

    RequestSender::RequestSender() {
        /* Конструктор класса - инициализирует поля apikey и post_url.
         * Создает объект класса RequestOptions, таким образом выставляя опции по умолчанию.
         */
        RequestSender::apikey = "";
        RequestSender::post_url = "";
        RequestSender::error_occured = false;
        RequestSender::options = RequestSender::RequestOptions();
    }

    void RequestSender::setApi(std::string apikey, std::string post_url) {
        RequestSender::apikey = apikey;
        RequestSender::post_url = post_url;
    }

    nlohmann::json RequestSender::postRequest(const std::string& image_path) {
        /* Делает пост-запрос к OCR_api, передавая на распознавание изображение в виде base64Image.
         * Все параметры запроса берутся из полей класса RequestSender.
         * Возвращает ответ сервера в виде json-объекта (словаря).
         */
        error_occured = false;
        auto r = cpr::Post(cpr::Url{RequestSender::post_url},
                           cpr::Multipart{ {"apikey", RequestSender::apikey},
                                           {"file",  cpr::File(image_path)},
                                           {"OCREngine", RequestSender::options.engine},
                                           {"isTable", RequestSender::options.isTable},
                                           {"isOverlayRequired", RequestSender::options.isOverlay},
                                           {"scale", RequestSender::options.scale},
                                           {"detectOrientation", RequestSender::options.orient}});
        //std::cout << "Response getted.\n";
        if (r.status_code == 0 or r.status_code >= 400) {
            error_occured = true;
            return nlohmann::json();
        }

        return nlohmann::json::parse(r.text); // JSON ответ от сервера.
    }

    bool RequestSender::isError() {
        return error_occured;
    }


} // OCRModule