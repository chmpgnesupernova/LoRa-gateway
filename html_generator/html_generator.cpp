#include "json_to_html.h"
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

int main() {

    while (true) {
        convert_json_to_html("../data/latest_messages.json", "../data/index.html");
        // 30초 마다 한번씩 html 업데이트
        std::this_thread::sleep_for(std::chrono::seconds(30));
    }
}