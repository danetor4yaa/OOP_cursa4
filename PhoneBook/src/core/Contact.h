#ifndef CONTACT_H
#define CONTACT_H

#include <string>
#include <vector>

class Contact {
private:
    std::string firstName;
    std::string lastName;
    std::string middleName;
    std::string address;
    std::string birthDate;
    std::string email;
    std::vector<std::string> phoneNumbers;

public:
    //конструкторы
    Contact();
    Contact(const std::string& first, const std::string& last, 
            const std::string& email, const std::string& phone);
    
    //геттеры
    std::string getFirstName() const;
    std::string getLastName() const;
    std::string getMiddleName() const;
    std::string getAddress() const;
    std::string getBirthDate() const;
    std::string getEmail() const;
    std::vector<std::string> getPhoneNumbers() const;
    
    //сеттеры
    void setFirstName(const std::string& name);
    void setLastName(const std::string& name);
    void setMiddleName(const std::string& name);
    void setAddress(const std::string& addr);
    void setBirthDate(const std::string& date);
    void setEmail(const std::string& mail);
    void setPhoneNumbers(const std::vector<std::string>& phones);
    void addPhoneNumber(const std::string& phone);
    
    //валидация
    bool isValid() const;
    
    //сериализация
    std::string toString() const;
    static Contact fromString(const std::string& data);
    
    //поиск
    bool contains(const std::string& query) const;
};

#endif