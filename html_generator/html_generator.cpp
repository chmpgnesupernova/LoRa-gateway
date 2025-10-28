#include "json_to_html.h"

int main() {
    convert_json_to_html("../data/latest_messages.json", "../data/latest_messages.html");
    return 0;
}