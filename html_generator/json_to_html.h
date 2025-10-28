#ifndef JSON_TO_HTML_H
#define JSON_TO_HTML_H

#include <iostream>
#include <string>
#include <fstream>
#include "../include/json.hpp"

// convert json to html

bool convert_json_to_html(const std::string& _json_filename, const std::string& _html_filename) {
    std::ifstream json_file(_json_filename);
    if (!json_file.is_open()) {
        std::cerr << "[GENERATOR] Error, JSON file not found at " << _json_filename << std::endl;
        return false;
    }

    nlohmann::json j;
    try {
        json_file >> j;
    } 
    catch (const std::exception& e) {
        std::cerr << "[GENERATOR] Error, parsing JSON: " << e.what() << std::endl;
        return false;
    }

    // write html file
    std::ofstream html_file(_html_filename);
    if (!html_file.is_open()) {
        std::cerr << "[GENERATOR] Error, Could not open HTML file for writing at " << _html_filename << std::endl;
        return false;
    }

    // HTML
    html_file << "<!DOCTYPE html>\n";
    html_file << "<html>\n<head>\n<title>JSON Data to HTML</title>\n";

    html_file << "<style>table, th, td { border: 1px solid black; border-collapse: collapse; padding: 8px; }</style>\n";
    html_file << "</head>\n<body>\n";
    html_file << "<h1>Data from JSON</h1>\n";
    html_file << "<table>\n";

    // 데이터 처리: JSON이 배열인지 확인
    if (j.is_array() && !j.empty()) {
        // 첫 번째 객체의 키를 사용하여 테이블 헤더(<th>) 생성
        html_file << "  <thead>\n  <tr>\n";
        // .items()를 사용하여 key-value 쌍을 순회
        for (auto const& [key, val] : j[0].items()) {
            html_file << "    <th>" << key << "</th>\n";
        }
        html_file << "  </tr>\n  </thead>\n";

        // 테이블 본문(<tbody>) 및 데이터 행(<tr>) 생성
        html_file << "  <tbody>\n";
        for (const auto& item : j) {
            html_file << "  <tr>\n";
            // 객체의 모든 값을 순서대로 출력
            for (auto const& [key, val] : item.items()) {
                std::string value_str;
                if (val.is_string()) {
                    value_str = val.get<std::string>();
                } else if (val.is_number()) {
                    value_str = std::to_string(val.get<double>()); // 숫자는 double로 변환 후 문자열로
                } else if (val.is_boolean()) {
                    value_str = val.get<bool>() ? "true" : "false";
                } else if (val.is_null()) {
                    value_str = "NULL";
                } else {
                    // 배열이나 객체 내부에 또 다른 구조가 있을 경우 dump()로 JSON 문자열로 변환
                    value_str = val.dump();
                }
                html_file << "    <td>" << value_str << "</td>\n";
            }
            html_file << "  </tr>\n";
        }
        html_file << "  </tbody>\n";

    } else {
        html_file << "<tr><td>JSON is not a valid array or is empty.</td></tr>\n";
    }
    
    // 5. HTML 구조 마무리
    html_file << "</table>\n";
    html_file << "</body>\n</html>\n";

    html_file.close();
    std::cout << "[GENERATOR] Success, HTML file saved to " << _html_filename << std::endl;
    return true;
}

#endif