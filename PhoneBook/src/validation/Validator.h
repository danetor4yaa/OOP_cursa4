#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <string>
#include <regex>

class Validator {
public:
    //основные методы валидации
    static bool validateName(const std::string& name);
    static bool validatePhone(const std::string& phone);
    static bool validateEmail(const std::string& email);
    static bool validateDate(const std::string& date);
    
    //нормализация данных
    static std::string normalizeName(const std::string& name);
    static std::string normalizePhone(const std::string& phone);
    static std::string normalizeEmail(const std::string& email);
    static std::string normalizeDate(const std::string& date);
    
    //валидация контакта целиком
    static bool validateContact(const class Contact& contact);

private:
    //вспомогательные методы
    static bool isLeapYear(int year);
    static int getDaysInMonth(int month, int year);
    static std::string removeExtraSpaces(const std::string& str);
};

#endif