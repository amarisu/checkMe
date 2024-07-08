#include "ResponseHandler.h"

namespace OCRModule {
    std::vector<std::pair<std::string, float>> ResponseHandler::processResponse(nlohmann::json response) {
        if (response["OCRExitCode"] != 1) {
            error_occured = true;
            return {};
        }
        fillObjectList(response);
        findPrices();
        filterObjects();
        sortObjects();
        return getPairs();
    }

    void ResponseHandler::fillObjectList(nlohmann::json response) {
        auto lines = response["ParsedResults"][0]["TextOverlay"]["Lines"];
        float x_of_total = 1000;

        for (auto& element : lines) {
            auto name = (std::string)element["LineText"];
            auto left_coord = (float)element["Words"][0]["Left"];
            auto right_coord = (float)element["Words"][0]["Width"] + left_coord;
            auto top_coord = (float)element["Words"][0]["Top"];
            recognized_objects.emplace_back(name, left_coord, right_coord, top_coord);
            if ((name.find("Итог") != std::string::npos || name.find("ИТОГ") != std::string::npos) && right_coord < x_of_total) {
                y_of_total = top_coord;
                x_of_total = right_coord;
            }

        }
        //std::cout << "ИТОГО - y: " << y_of_total << "\n";
    }

    void ResponseHandler::findPrices() {
        std::vector<RecognizedObject> detected_floats;
        float max_right_coord = 0;

        // Заполнение вектора всех считанных флоатов
        for (auto& element : recognized_objects) {
            std::string text = element.text;
            //std::cout << "Text: " << element.text << " Coord: " << element.right << "\n";
            if (isFloatNumber(text)) {
                text.erase(std::remove_if(text.begin(), text.end(), [](unsigned char x) { return std::isspace(x) || x == '='; }),
                           text.end());
                detected_floats.emplace_back(text, element.left, element.right, element.top);
                if (element.right > max_right_coord)
                    max_right_coord = element.right;
                //std::cout << "Float: " << element.text << " Coord: " << element.right << "\n";
            }
        }

        // Выбор самых "правых"
        std::vector<RecognizedObject> right_column;
        for (auto& element : detected_floats){
            if (std::abs(element.right - max_right_coord) < max_right_coord*0.05) {
                //std::cout << "Float: " << element.text << " Coord: " << element.right << " Top: " << element.top << "\n";
                right_column.push_back(element);
            }
        }

        float total = 0;
        // Если в чеке было обнаружено слово ИТОГО, то делаем проверку:
        if (y_of_total != 0){
            for (auto& element : right_column){
                if (abs(element.top - y_of_total) < 15){
                    // Нашли флоат, соответствующий строке ИТОГО. Если несколько подходит, берем последний ненулевой (нижний)
                    if (stof(element.text) != 0)
                        total = stof(element.text);
                }
            }
        }

        if (total == 0)
            error_occured = true;

        std::vector<RecognizedObject> prices;
        float sum = 0;
        // Подсчитываем сумму
        for (auto& element : right_column){
            float number = stof(element.text);
            sum += number;
            if (total != 0 && sum > total + 1) {
                sum -= number;
                break;
            }
            prices.push_back(element);
            //std::cout << "Float was added: " <<  number << "\n";
        }
        //std::cout << "Sum: " << sum << "\n";

        // Сохранение результатов.
        ResponseHandler::prices = prices;

        // Проверка на ошибку:
        if (std::abs(total - sum) > total*0.05)
            ResponseHandler::error_occured = true;
    }

    void ResponseHandler::filterObjects() {
        std::vector<RecognizedObject> filtered_lines;
        for (auto& element : recognized_objects) {
            if (element.top < y_of_total && element.text.size() > 5 && element.text.find("скидк") == std::string::npos && element.text.find("СКИДК") == std::string::npos)
                filtered_lines.push_back(element);
        }
        recognized_objects = filtered_lines;
    }


    bool compare_top(const RecognizedObject& left, const RecognizedObject& right) {
        return left.top < right.top;
    }

    void ResponseHandler::sortObjects() {
        std::vector<RecognizedObject> longest_lines = recognized_objects;
        std::sort(longest_lines.begin(), longest_lines.end(), compare_top);
        products = longest_lines;
    }

    int ResponseHandler::getScore(const std::string& text){
        int score = 0;
        for (auto& element : text){
            if (isdigit(element))
                score--;
            else
                score++;
        }
        return score;
    }

    std::vector<std::pair<std::string, float>> ResponseHandler::getPairs(){
        std::vector<std::pair<std::string, float>> pairs;
        float last_top_coord = 0;
        float cur_top_coord = 0;
        for (int i = 0; i<prices.size(); i++){
            std::string name = "";
            float score = 100;
            float line_distance = prices[1].top - prices[0].top;

            for (auto& element : products) {
                if (prices[i].top - element.top < line_distance && prices[i].top - element.top > 0){
                    float new_score = abs(prices[i].top - line_distance/2 - element.top) - getScore(element.text);
                    if (new_score < score && element.top != last_top_coord) {
                        name = element.text;
                        score = new_score;
                        cur_top_coord = element.top;
                    }
                }
            }

            last_top_coord = cur_top_coord;
            if (name.empty())
                error_occured = true;
            pairs.emplace_back(name, stof(prices[i].text));
        }
        return pairs;
    }

    /*void ResponseHandler::testPrintMap() {
        std::ofstream outputFile("output.txt");

        for (auto& element : recognized_objects) {
            auto name = element.text;
            auto left_coord = element.left;
            auto right_coord = element.right;
            auto top_coord = element.top;
            outputFile << name << "       " << std::to_string(left_coord) << " " << std::to_string(right_coord) << " " << std::to_string(top_coord) << '\n';
        }

        outputFile.close();
    }*/


    bool ResponseHandler::isError() {
        return error_occured;
    }

    static bool isFloatNumber(const std::string& string){
        std::string::const_iterator it = string.begin();
        bool decimalPoint = false;
        while(it != string.end()){
            if(*it == '.' || *it == ','){
                if(!decimalPoint) decimalPoint = true;
                else break;
            }else if(!(std::isdigit(*it) || std::isspace(*it) || *it == '=')){
                break;
            }
            ++it;
        }
        return !string.empty() && it == string.end() && decimalPoint;
    }
} // OCRModule