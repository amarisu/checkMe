#ifndef OCRMODULE_REQUESTSENDER_H
#define OCRMODULE_REQUESTSENDER_H

#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

namespace OCRModule {
    class RequestSender {
        class RequestOptions {
        public:
            std::string scale;
            std::string isTable;
            std::string engine;
            std::string isOverlay;
            std::string orient;
            RequestOptions();
            void setOptions(bool scale, bool istable, std::string engine, bool overlay, bool orient);
        };

    private:
        std::string apikey;
        std::string post_url;
        bool error_occured;
    public:
        RequestOptions options;
        RequestSender();
        void setApi(std::string apikey, std::string post_url);
        nlohmann::json postRequest(const std::string& image_path);
        bool isError();
    };

} // OCRModule

#endif //OCRMODULE_REQUESTSENDER_H
