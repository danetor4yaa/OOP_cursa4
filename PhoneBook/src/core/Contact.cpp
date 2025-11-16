#include "Contact.h"
#include <sstream>

Contact::Contact() = default;

Contact::Contact(const std::string& first, const std::string& last, 
                 const std::string& mail, const std::string& phone)
    : firstName(first), lastName(last), email(mail) {
    phoneNumbers.push_back(phone);
}

//геттеры
std::string Contact::getFirstName() const { return firstName; }
std::string Contact::getLastName() const { return lastName; }
std::string Contact::getMiddleName() const { return middleName; }
std::string Contact::getAddress() const { return address; }
std::string Contact::getBirthDate() const { return birthDate; }
std::string Contact::getEmail() const { return email; }
std::vector<std::string> Contact::getPhoneNumbers() const { return phoneNumbers; }

//сеттеры
void Contact::setFirstName(const std::string& name) { firstName = name; }
void Contact::setLastName(const std::string& name) { lastName = name; }
void Contact::setMiddleName(const std::string& name) { middleName = name; }
void Contact::setAddress(const std::string& addr) { address = addr; }
void Contact::setBirthDate(const std::string& date) { birthDate = date; }
void Contact::setEmail(const std::string& mail) { email = mail; }
void Contact::setPhoneNumbers(const std::vector<std::string>& phones) { phoneNumbers = phones; }
void Contact::addPhoneNumber(const std::string& phone) { phoneNumbers.push_back(phone); }

bool Contact::isValid() const {
    //базовая проверка - потом вызов Validator
    return !firstName.empty() && !lastName.empty() && !email.empty() && !phoneNumbers.empty();
}

std::string Contact::toString() const {
    std::stringstream ss;
    ss << firstName << "|" << lastName << "|" << middleName << "|"
       << address << "|" << birthDate << "|" << email << "|";
    
    for (size_t i = 0; i < phoneNumbers.size(); ++i) {
        if (i > 0) ss << ",";
        ss << phoneNumbers[i];
    }
    
    return ss.str();
}

Contact Contact::fromString(const std::string& data) {
    std::stringstream ss(data);
    std::string token;
    Contact contact;
    
    //парсинг основных полей
    std::getline(ss, token, '|'); contact.setFirstName(token);
    std::getline(ss, token, '|'); contact.setLastName(token);
    std::getline(ss, token, '|'); contact.setMiddleName(token);
    std::getline(ss, token, '|'); contact.setAddress(token);
    std::getline(ss, token, '|'); contact.setBirthDate(token);
    std::getline(ss, token, '|'); contact.setEmail(token);
    
    //парсинг телефонов
    std::string phones;
    std::getline(ss, phones);
    if (!phones.empty()) {
        std::stringstream phoneStream(phones);
        std::string phone;
        while (std::getline(phoneStream, phone, ',')) {
            contact.addPhoneNumber(phone);
        }
    }
    
    return contact;
}

bool Contact::contains(const std::string& query) const {
    std::string lowerQuery;
    for (char c : query) lowerQuery += std::tolower(c);
    
    auto containsIgnoreCase = [&](const std::string& str) {
        std::string lowerStr;
        for (char c : str) lowerStr += std::tolower(c);
        return lowerStr.find(lowerQuery) != std::string::npos;
    };
    
    return containsIgnoreCase(firstName) || containsIgnoreCase(lastName) ||
           containsIgnoreCase(middleName) || containsIgnoreCase(email) ||
           containsIgnoreCase(address);
}