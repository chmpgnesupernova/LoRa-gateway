#include "json_to_html.h"

int main() {
    convert_json_to_html("../data/latest_messages.json", "../data/index.html");
    return 0;
}