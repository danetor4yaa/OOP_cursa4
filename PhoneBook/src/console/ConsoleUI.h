#ifndef CONSOLEUI_H
#define CONSOLEUI_H

#include "../core/PhoneBook.h"
#include "../storage/FileStorage.h"
#include "../utils/StringUtils.h"
#include <memory>

class ConsoleUI {
private:
    PhoneBook phoneBook;
    std::unique_ptr<FileStorage> storage;
    bool running;

public:
    ConsoleUI(const std::string& filename = "data/contacts.txt");
    
    //основной цикл
    void run();
    
private:
    //методы меню
    void showMainMenu();
    void handleMainMenuChoice(int choice);
    
    //операции с контактами
    void addContact();
    void removeContact();
    void editContact();
    void searchContacts();
    void showAllContacts();
    void showSearchMenu();
    void handleSearch();
    
    //вспомогательные методы
    void printContact(const Contact& contact) const;
    void printContacts(const std::vector<Contact>& contacts) const;
    Contact inputContact(bool forEdit = false);
    std::string inputPhoneNumbers();
    void printContactsWithNumbers(const std::vector<Contact>& contacts) const;
Contact inputContactWithDefaults(const Contact& defaultContact);
    
    //утилиты
    void clearScreen();
    void waitForEnter();
};

#endif