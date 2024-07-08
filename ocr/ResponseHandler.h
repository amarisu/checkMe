#ifndef OCRMODULE_RESPONSEHANDLER_H
#define OCRMODULE_RESPONSEHANDLER_H

#include <nlohmann/json.hpp>
#include <string>
#include <utility>
#include <fstream>
#include <algorithm>

namespace OCRModule {
    class RecognizedObject {
    public:
        std::string text;
        float left;
        float right;
        float top;
        RecognizedObject(std::string text, float left, float right, float top): text(text), left(left), right(right), top(top){};
    };

    class ResponseHandler {
    private:
        std::vector<RecognizedObject> recognized_objects;
        std::vector<RecognizedObject> products;
        std::vector<RecognizedObject> prices;

        bool error_occured;
        float y_of_total;

        void fillObjectList(nlohmann::json response);
        void filterObjects();
        void sortObjects();
        int getScore(const std::string& text);
        void findPrices();
        //void testPrintMap();
        std::vector<std::pair<std::string, float>> getPairs();

    public:
        ResponseHandler(): error_occured(false), y_of_total(0){};
        std::vector<std::pair<std::string, float>> processResponse(nlohmann::json response);
        bool isError();
    };

    static bool isFloatNumber(const std::string& string);
} // OCRModule

#endif //OCRMODULE_RESPONSEHANDLER_H
