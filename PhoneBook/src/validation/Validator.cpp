#include "Validator.h"
#include <regex>
#include <sstream>
#include <algorithm>
#include "../core/Contact.h"

bool Validator::validateName(const std::string& name) {
    if (name.empty()) return true; //отчество может быть пустым
    
    //буквы различных алфавитов, цифры, дефис, пробел
    //должно начинаться с буквы, не может начинаться/заканчиваться дефисом
    std::regex name_regex("^[A-Za-zА-Яа-я][A-Za-zА-Яа-я0-9\\s\\-]*[A-Za-zА-Яа-я0-9]$");
    return std::regex_match(name, name_regex);
}

bool Validator::validatePhone(const std::string& phone) {
    std::string normalized = normalizePhone(phone);
    
    //упрощённая проверка. просто считаем что номер валидный если он не пустой
    if (normalized.empty()) {
        return false;
    }
    
    //проверяем основные форматы
    if (normalized.length() == 12 && normalized.substr(0, 2) == "+7") {
        //проверяем что остальные символы цифры
        for (size_t i = 2; i < normalized.length(); i++) {
            if (!std::isdigit(normalized[i])) return false;
        }
        return true;
    }
    else if (normalized.length() == 11 && normalized[0] == '8') {
        //проверяем что остальные символы цифры
        for (size_t i = 1; i < normalized.length(); i++) {
            if (!std::isdigit(normalized[i])) return false;
        }
        return true;
    }
    
    //если не подходит под форматы, но не пустой считаем валидным
    return !normalized.empty();
}

bool Validator::validateEmail(const std::string& email) {
    std::string normalized = normalizeEmail(email);
    if (normalized.empty()) return false;
    
    //простая проверка есть @ и точка после неё
    size_t at_pos = normalized.find('@');
    if (at_pos == std::string::npos || at_pos == 0) {
        return false;
    }
    
    size_t dot_pos = normalized.find('.', at_pos);
    if (dot_pos == std::string::npos || dot_pos == at_pos + 1) {
        return false;
    }
    
    return true;
}

bool Validator::validateDate(const std::string& date) {
    if (date.empty()) return true; //дата рождения не обязательна
    
    //формат: YYYY-MM-DD
    std::regex date_regex("^\\d{4}-\\d{2}-\\d{2}$");
    if (!std::regex_match(date, date_regex)) {
        return false;
    }
    
    int year, month, day;
    char dash1, dash2;
    std::stringstream ss(date);
    ss >> year >> dash1 >> month >> dash2 >> day;
    
    //проверяем корректность
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;
    
    //проверяем количество дней в месяце
    int maxDays = getDaysInMonth(month, year);
    if (day > maxDays) return false;
    
    //проверяем что дата не в будущем (упрощённо)
    
    return true;
}

//нормализация данных
std::string Validator::normalizeName(const std::string& name) {
    return removeExtraSpaces(name);
}

std::string Validator::normalizePhone(const std::string& phone) {
    std::string result;
    for (char c : phone) {
        if (std::isdigit(c) || c == '+') {
            result += c;
        }
    }
    return result;
}

std::string Validator::normalizeEmail(const std::string& email) {
    std::string normalized = removeExtraSpaces(email);
    //удаляем пробелы вокруг @
    size_t at_pos = normalized.find('@');
    if (at_pos != std::string::npos) {
        std::string local = normalized.substr(0, at_pos);
        std::string domain = normalized.substr(at_pos + 1);
        //удаляем пробелы в local и domain частях
        local.erase(std::remove(local.begin(), local.end(), ' '), local.end());
        domain.erase(std::remove(domain.begin(), domain.end(), ' '), domain.end());
        normalized = local + "@" + domain;
    }
    return normalized;
}

std::string Validator::normalizeDate(const std::string& date) {
    return removeExtraSpaces(date);
}

bool Validator::validateContact(const Contact& contact) {
    return validateName(contact.getFirstName()) &&
           validateName(contact.getLastName()) &&
           validateName(contact.getMiddleName()) &&
           validateEmail(contact.getEmail()) &&
           validateDate(contact.getBirthDate()) &&
           !contact.getPhoneNumbers().empty() &&
           validatePhone(contact.getPhoneNumbers()[0]); // Первый номер обязателен
}

//вспомогательные методы
bool Validator::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int Validator::getDaysInMonth(int month, int year) {
    switch (month) {
        case 2: return isLeapYear(year) ? 29 : 28;
        case 4: case 6: case 9: case 11: return 30;
        default: return 31;
    }
}

std::string Validator::removeExtraSpaces(const std::string& str) {
    std::string result;
    bool lastWasSpace = true; //начинаем с true чтобы удалить начальные пробелы
    
    for (char c : str) {
        if (std::isspace(c)) {
            if (!lastWasSpace) {
                result += ' ';
                lastWasSpace = true;
            }
        } else {
            result += c;
            lastWasSpace = false;
        }
    }
    
    //удаляем конечный пробел
    if (!result.empty() && std::isspace(result.back())) {
        result.pop_back();
    }
    
    return result;
}