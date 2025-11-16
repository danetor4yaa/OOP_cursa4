#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include "Contact.h"
#include <vector>
#include <memory>

class PhoneBook {
private:
    std::vector<Contact> contacts;

public:
    //основные операции
    bool addContact(const Contact& contact);
    bool removeContact(const std::string& email);
    bool editContact(const std::string& email, const Contact& newData);
    
    //поиск и сортировка
    std::vector<Contact> search(const std::string& query) const;
    std::vector<Contact> searchByField(const std::string& field, const std::string& query) const;
    void sortByFirstName();
    void sortByLastName();
    void sortByEmail();
    
    //геттеры
    const std::vector<Contact>& getAllContacts() const;
    Contact* findContactByEmail(const std::string& email);
    
    //утилиты
    size_t getContactCount() const;
    void clear();
};

#endif