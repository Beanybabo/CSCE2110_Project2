#ifndef STRINGUTILS_H
#define STRINGUTILS_H
 
#include <string>
#include <vector>
#include <cctype>
 
// removes spaces, tabs, and \r from both ends. Inner spaces are kept
inline std::string trim(const std::string& text) {
    const std::string whitespace = " \t\r\n";
    size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1);
}
 
// returns a copy of the text with every ASCII letter in uppercase
inline std::string toUpper(std::string text) {
    for (size_t i = 0; i < text.size(); i++) {
        text[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(text[i])));
    }
    return text;
}
 
// returns true if both strings match when capitalization is ignored
inline bool equalsIgnoreCase(const std::string& a, const std::string& b) {
    return toUpper(a) == toUpper(b);
}
 
// split a line on the given delimiter. "a|b|" gives 3 fields: a, b, ""
inline std::vector<std::string> splitFields(const std::string& line, char delimiter) {
    std::vector<std::string> fields;
    size_t start = 0;
    while (true) {
        size_t pos = line.find(delimiter, start);
        if (pos == std::string::npos) {
            fields.push_back(line.substr(start));
            break;
        }
        fields.push_back(line.substr(start, pos - start));
        start = pos + 1;
    }
    return fields;
}
 
#endif