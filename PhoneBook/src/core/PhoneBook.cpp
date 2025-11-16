#include "PhoneBook.h"
#include <algorithm>
#include "../utils/StringUtils.h"

bool PhoneBook::addContact(const Contact& contact) {
    if (!contact.isValid()) {
        return false;
    }
    
    //уникальность email
    for (const auto& existingContact : contacts) {
        if (existingContact.getEmail() == contact.getEmail()) {
            return false;
        }
    }
    
    contacts.push_back(contact);
    return true;
}

bool PhoneBook::removeContact(const std::string& email) {
    auto it = std::remove_if(contacts.begin(), contacts.end(),
        [&](const Contact& contact) { return contact.getEmail() == email; });
    
    if (it != contacts.end()) {
        contacts.erase(it, contacts.end());
        return true;
    }
    return false;
}

bool PhoneBook::editContact(const std::string& email, const Contact& newData) {
    for (auto& contact : contacts) {
        if (contact.getEmail() == email) {
            contact = newData;
            return true;
        }
    }
    return false;
}

std::vector<Contact> PhoneBook::search(const std::string& query) const {
    std::vector<Contact> results;
    for (const auto& contact : contacts) {
        if (contact.contains(query)) {
            results.push_back(contact);
        }
    }
    return results;
}

void PhoneBook::sortByFirstName() {
    std::sort(contacts.begin(), contacts.end(),
        [](const Contact& a, const Contact& b) {
            return a.getFirstName() < b.getFirstName();
        });
}

void PhoneBook::sortByLastName() {
    std::sort(contacts.begin(), contacts.end(),
        [](const Contact& a, const Contact& b) {
            return a.getLastName() < b.getLastName();
        });
}

void PhoneBook::sortByEmail() {
    std::sort(contacts.begin(), contacts.end(),
        [](const Contact& a, const Contact& b) {
            return a.getEmail() < b.getEmail();
        });
}

const std::vector<Contact>& PhoneBook::getAllContacts() const {
    return contacts;
}

Contact* PhoneBook::findContactByEmail(const std::string& email) {
    for (auto& contact : contacts) {
        if (contact.getEmail() == email) {
            return &contact;
        }
    }
    return nullptr;
}

size_t PhoneBook::getContactCount() const {
    return contacts.size();
}

void PhoneBook::clear() {
    contacts.clear();
}

std::vector<Contact> PhoneBook::searchByField(const std::string& field, const std::string& query) const {
    std::vector<Contact> results;
    
    for (const auto& contact : contacts) {
        if (field == "firstname") {
            if (StringUtils::containsIgnoreCase(contact.getFirstName(), query)) {
                results.push_back(contact);
            }
        }
        else if (field == "lastname") {
            if (StringUtils::containsIgnoreCase(contact.getLastName(), query)) {
                results.push_back(contact);
            }
        }
        else if (field == "email") {
            if (StringUtils::containsIgnoreCase(contact.getEmail(), query)) {
                results.push_back(contact);
            }
        }
        else if (field == "phone") {
            auto phones = contact.getPhoneNumbers();
            for (const auto& phone : phones) {
                if (StringUtils::containsIgnoreCase(phone, query)) {
                    results.push_back(contact);
                    break;
                }
            }
        }
        else {
            //поиск по всем полям
            if (contact.contains(query)) {
                results.push_back(contact);
            }
        }
    }
    
    return results;
}