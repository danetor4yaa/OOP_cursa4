#include "StringUtils.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

std::string StringUtils::trim(const std::string& str) {
    if (str.empty()) return "";
    
    size_t start = 0;
    size_t end = str.length() - 1;
    
    //находим первый непробельный символ
    while (start <= end && std::isspace(str[start])) {
        start++;
    }
    
    //находим последний нпробельный символ
    while (end >= start && std::isspace(str[end])) {
        end--;
    }
    
    //если все символы пробельные
    if (start > end) return "";
    
    return str.substr(start, end - start + 1);
}

std::string StringUtils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StringUtils::toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

bool StringUtils::containsIgnoreCase(const std::string& str, const std::string& substr) {
    std::string lowerStr = toLower(str);
    std::string lowerSub = toLower(substr);
    return lowerStr.find(lowerSub) != std::string::npos;
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(trim(token));
        }
    }
    
    return tokens;
}

std::string StringUtils::join(const std::vector<std::string>& strings, const std::string& delimiter) {
    std::string result;
    for (size_t i = 0; i < strings.size(); ++i) {
        if (i > 0) result += delimiter;
        result += strings[i];
    }
    return result;
}

std::string StringUtils::readLine(const std::string& prompt) {
    if (!prompt.empty()) {
        std::cout << prompt;
    }
    
    std::string input;
    std::getline(std::cin, input);
    return trim(input);
}

int StringUtils::readInt(const std::string& prompt) {
    while (true) {
        std::string input = readLine(prompt);
        try {
            return std::stoi(input);
        } catch (const std::exception&) {
            std::cout << "Ошибка: введите корректное число!" << std::endl;
        }
    }
}

std::string StringUtils::formatPhone(const std::string& phone) {
    if (phone.empty()) return "";
    
    std::string digits;
    for (char c : phone) {
        if (std::isdigit(c) || c == '+') {
            digits += c;
        }
    }
    
    //форматируем в красивый вид
    if (digits.length() == 11 && digits[0] == '8') {
        return "+7 (" + digits.substr(1, 3) + ") " + digits.substr(4, 3) + "-" + 
               digits.substr(7, 2) + "-" + digits.substr(9, 2);
    }
    else if (digits.length() == 12 && digits.substr(0, 2) == "+7") {
        return "+7 (" + digits.substr(2, 3) + ") " + digits.substr(5, 3) + "-" + 
               digits.substr(8, 2) + "-" + digits.substr(10, 2);
    }
    
    return phone; //если формат не распознан
}

std::string StringUtils::formatName(const std::string& name) {
    if (name.empty()) return "";
    
    std::string result = name;
    //первую букву заглавной
    if (!result.empty()) {
        result[0] = std::toupper(result[0]);
    }
    return result;
}