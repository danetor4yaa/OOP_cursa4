#include "ConsoleUI.h"
#include "../utils/StringUtils.h"
#include "../validation/Validator.h"
#include <iostream>
#include <limits>

ConsoleUI::ConsoleUI(const std::string& filename) 
    : storage(std::make_unique<FileStorage>(filename)), running(true) {
    
    //загрузка контактов из файла
    auto loadedContacts = storage->load();
    for (const auto& contact : loadedContacts) {
        phoneBook.addContact(contact);
    }
    
    std::cout << "Phone book loaded. Contacts: " 
              << phoneBook.getContactCount() << std::endl;
}

void ConsoleUI::run() {
    //главный цикл программы
    while (running) {
        showMainMenu();
        int choice = StringUtils::readInt("Choose action: ");
        handleMainMenuChoice(choice);
    }
}

void ConsoleUI::showMainMenu() {
    clearScreen();
    std::cout << "=== PHONE BOOK ===" << std::endl;
    std::cout << "1. Show all contacts" << std::endl;
    std::cout << "2. Add contact" << std::endl;
    std::cout << "3. Delete contact" << std::endl;
    std::cout << "4. Edit contact" << std::endl;
    std::cout << "5. Search contacts" << std::endl;
    std::cout << "6. Sort by first name" << std::endl;
    std::cout << "7. Sort by last name" << std::endl;
    std::cout << "8. Sort by email" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "==================" << std::endl;
}

void ConsoleUI::handleMainMenuChoice(int choice) {
    //обработка выбора в главном меню
    switch (choice) {
        case 1: showAllContacts(); break;
        case 2: addContact(); break;
        case 3: removeContact(); break;
        case 4: editContact(); break;
        case 5: handleSearch(); break;  //использование улучшенного поиска
        case 6: 
            phoneBook.sortByFirstName();
            std::cout << "Contacts sorted by first name!" << std::endl;
            showAllContacts();
            break;
        case 7: 
            phoneBook.sortByLastName();
            std::cout << "Contacts sorted by last name!" << std::endl;
            showAllContacts();
            break;
        case 8: 
            phoneBook.sortByEmail();
            std::cout << "Contacts sorted by email!" << std::endl;
            showAllContacts();
            break;
        case 0: 
            running = false;
            storage->save(phoneBook.getAllContacts());
            std::cout << "Data saved. Goodbye!" << std::endl;
            break;
        default:
            std::cout << "Invalid choice!" << std::endl;
            waitForEnter();
    }
}

void ConsoleUI::addContact() {
    clearScreen();
    std::cout << "=== ADD CONTACT ===" << std::endl;
    
    //создание нового контакта
    Contact contact = inputContact(false);

    //отладочная информация
    std::cout << "=== DEBUG ===" << std::endl;
    std::cout << "First name: '" << contact.getFirstName() << "'" << std::endl;
    std::cout << "Last name: '" << contact.getLastName() << "'" << std::endl;
    std::cout << "Email: '" << contact.getEmail() << "'" << std::endl;
    
    //добавление контакта в телефонную книгу
    if (phoneBook.addContact(contact)) {
        std::cout << "Contact added successfully!" << std::endl;
        storage->save(phoneBook.getAllContacts());
    } else {
        std::cout << "Error: Could not add contact. Check the data." << std::endl;
    }
    
    waitForEnter();
}

void ConsoleUI::removeContact() {
    clearScreen();
    std::cout << "=== DELETE CONTACT ===" << std::endl;
    
    //показ всех контактов с номерами
    const auto& contacts = phoneBook.getAllContacts();
    if (contacts.empty()) {
        std::cout << "Phone book is empty. Nothing to delete." << std::endl;
        waitForEnter();
        return;
    }
    
    std::cout << "All contacts:" << std::endl;
    printContactsWithNumbers(contacts);
    
    //запрос номера контакта для удаления
    int contactNumber = StringUtils::readInt("Enter contact number to delete (0 to cancel): ");
    
    if (contactNumber == 0) {
        std::cout << "Deletion cancelled." << std::endl;
        waitForEnter();
        return;
    }
    
    //проверка валидности номера
    if (contactNumber < 1 || contactNumber > static_cast<int>(contacts.size())) {
        std::cout << "Invalid contact number!" << std::endl;
        waitForEnter();
        return;
    }
    
    //email выбранного контакта
    const Contact& contactToDelete = contacts[contactNumber - 1];
    std::string email = contactToDelete.getEmail();
    
    //подтверждение удаления
    std::cout << "You are about to delete this contact:" << std::endl;
    printContact(contactToDelete);
    std::cout << std::endl;
    
    std::string confirmation = StringUtils::readLine("Are you sure? (yes/no): ");
    if (confirmation == "yes" || confirmation == "y") {
        if (phoneBook.removeContact(email)) {
            std::cout << "Contact deleted successfully!" << std::endl;
            storage->save(phoneBook.getAllContacts());
        } else {
            std::cout << "Error deleting contact." << std::endl;
        }
    } else {
        std::cout << "Deletion cancelled." << std::endl;
    }
    
    waitForEnter();
}

void ConsoleUI::editContact() {
    clearScreen();
    std::cout << "=== EDIT CONTACT ===" << std::endl;
    
    //все контакты с номерами
    const auto& contacts = phoneBook.getAllContacts();
    if (contacts.empty()) {
        std::cout << "Phone book is empty. Nothing to edit." << std::endl;
        waitForEnter();
        return;
    }
    
    std::cout << "All contacts:" << std::endl;
    printContactsWithNumbers(contacts);
    
    //номер контакта для редактирования
    int contactNumber = StringUtils::readInt("Enter contact number to edit (0 to cancel): ");
    
    if (contactNumber == 0) {
        std::cout << "Edit cancelled." << std::endl;
        waitForEnter();
        return;
    }
    
    //валидность номера
    if (contactNumber < 1 || contactNumber > static_cast<int>(contacts.size())) {
        std::cout << "Invalid contact number!" << std::endl;
        waitForEnter();
        return;
    }
    
    //выбранный контакт
    const Contact& contactToEdit = contacts[contactNumber - 1];
    std::string email = contactToEdit.getEmail();
    
    //текущие данные
    std::cout << "Current contact data:" << std::endl;
    printContact(contactToEdit);
    std::cout << std::endl;
    
    //новые данные
    std::cout << "Enter new data (leave empty to keep current value):" << std::endl;
    Contact newData = inputContactWithDefaults(contactToEdit);
    
    if (phoneBook.editContact(email, newData)) {
        std::cout << "Contact updated successfully!" << std::endl;
        storage->save(phoneBook.getAllContacts());
    } else {
        std::cout << "Error updating contact." << std::endl;
    }
    
    waitForEnter();
}

void ConsoleUI::searchContacts() {
    clearScreen();
    std::cout << "=== SEARCH CONTACTS ===" << std::endl;
    
    //поиск по всем полям
    std::string query = StringUtils::readLine("Enter search query: ");
    auto results = phoneBook.search(query);
    
    if (results.empty()) {
        std::cout << "No contacts found." << std::endl;
    } else {
        std::cout << "Found contacts: " << results.size() << std::endl;
        printContacts(results);
    }
    
    waitForEnter();
}

void ConsoleUI::showAllContacts() {
    clearScreen();
    std::cout << "=== ALL CONTACTS ===" << std::endl;
    
    //все контакты
    const auto& contacts = phoneBook.getAllContacts();
    if (contacts.empty()) {
        std::cout << "Phone book is empty." << std::endl;
    } else {
        std::cout << "Total contacts: " << contacts.size() << std::endl;
        printContacts(contacts);
    }
    
    waitForEnter();
}

void ConsoleUI::printContact(const Contact& contact) const {
    //информацич об одном контакте
    std::cout << "First Name: " << contact.getFirstName() << std::endl;
    std::cout << "Last Name: " << contact.getLastName() << std::endl;
    if (!contact.getMiddleName().empty()) {
        std::cout << "Middle Name: " << contact.getMiddleName() << std::endl;
    }
    if (!contact.getAddress().empty()) {
        std::cout << "Address: " << contact.getAddress() << std::endl;
    }
    if (!contact.getBirthDate().empty()) {
        std::cout << "Birth Date: " << contact.getBirthDate() << std::endl;
    }
    std::cout << "Email: " << contact.getEmail() << std::endl;
    
    //все номера телефонов
    auto phones = contact.getPhoneNumbers();
    std::cout << "Phones: ";
    for (size_t i = 0; i < phones.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << StringUtils::formatPhone(phones[i]);
    }
    std::cout << std::endl;
}

void ConsoleUI::printContacts(const std::vector<Contact>& contacts) const {
    //список контактов
    for (size_t i = 0; i < contacts.size(); ++i) {
        std::cout << "--- Contact #" << (i + 1) << " ---" << std::endl;
        printContact(contacts[i]);
        std::cout << std::endl;
    }
}

Contact ConsoleUI::inputContact(bool forEdit) {
    Contact contact;
    
    std::cout << "Enter contact data:" << std::endl;
    
    //обязательные поля
    contact.setFirstName(StringUtils::readLine("First Name*: "));
    contact.setLastName(StringUtils::readLine("Last Name*: "));
    if (!forEdit) {
        contact.setEmail(StringUtils::readLine("Email*: "));
    }
    
    //необязательные поля
    contact.setMiddleName(StringUtils::readLine("Middle Name: "));
    contact.setAddress(StringUtils::readLine("Address: "));
    contact.setBirthDate(StringUtils::readLine("Birth Date (YYYY-MM-DD): "));
    
    //телефонные номера
    std::cout << "Phone numbers*:" << std::endl;
    std::string phones = inputPhoneNumbers();
    auto phoneList = StringUtils::split(phones, ',');
    contact.setPhoneNumbers(phoneList);
    
    return contact;
}

std::string ConsoleUI::inputPhoneNumbers() {
    std::vector<std::string> phones;
    
    std::cout << "Enter phone numbers (one per line, empty line to finish):" << std::endl;
    
    while (true) {
        std::string phone = StringUtils::readLine("Phone: ");
        if (phone.empty()) {
            break;
        }
        
        if (Validator::validatePhone(phone)) {
            phones.push_back(phone);
            std::cout << "Number added." << std::endl;
        } else {
            std::cout << "Invalid phone format! Try again." << std::endl;
        }
    }
    
    return StringUtils::join(phones, ",");
}

void ConsoleUI::clearScreen() {
    //очистка экрана для разных операционных систем
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void ConsoleUI::waitForEnter() {
    //ожидание нажатия Enter
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void ConsoleUI::showSearchMenu() {
    clearScreen();
    std::cout << "=== SEARCH CONTACTS ===" << std::endl;
    std::cout << "1. Search by first name" << std::endl;
    std::cout << "2. Search by last name" << std::endl;
    std::cout << "3. Search by email" << std::endl;
    std::cout << "4. Search by phone" << std::endl;
    std::cout << "5. Search all fields" << std::endl;
    std::cout << "0. Back" << std::endl;
    std::cout << "=======================" << std::endl;
}

void ConsoleUI::handleSearch() {
    //меню поиска
    showSearchMenu();
    int choice = StringUtils::readInt("Choose search type: ");
    
    if (choice == 0) return;
    
    //поле для поиска
    std::string field;
    switch (choice) {
        case 1: field = "firstname"; break;
        case 2: field = "lastname"; break;
        case 3: field = "email"; break;
        case 4: field = "phone"; break;
        case 5: field = "all"; break;
        default: 
            std::cout << "Invalid choice!" << std::endl;
            waitForEnter();
            return;
    }
    
    //выполнение поиска
    std::string query = StringUtils::readLine("Enter search query: ");
    auto results = phoneBook.searchByField(field, query);
    
    //результаты
    if (results.empty()) {
        std::cout << "No contacts found." << std::endl;
    } else {
        std::cout << "Found contacts: " << results.size() << std::endl;
        printContacts(results);
    }
    
    waitForEnter();
}
void ConsoleUI::printContactsWithNumbers(const std::vector<Contact>& contacts) const {
    for (size_t i = 0; i < contacts.size(); ++i) {
        std::cout << "[" << (i + 1) << "] ";
        std::cout << contacts[i].getFirstName() << " " 
                  << contacts[i].getLastName() << " (" 
                  << contacts[i].getEmail() << ")" << std::endl;
    }
    std::cout << std::endl;
}

Contact ConsoleUI::inputContactWithDefaults(const Contact& defaultContact) {
    Contact contact;
    
    //текущий email (нельзя менять)
    contact.setEmail(defaultContact.getEmail());
    
    //запрос новых значений с подсказками текущих
    std::string input;
    
    //имя
    std::cout << "First Name [" << defaultContact.getFirstName() << "]: ";
    input = StringUtils::readLine("");
    contact.setFirstName(input.empty() ? defaultContact.getFirstName() : input);
    
    //фамилия
    std::cout << "Last Name [" << defaultContact.getLastName() << "]: ";
    input = StringUtils::readLine("");
    contact.setLastName(input.empty() ? defaultContact.getLastName() : input);
    
    //отчество
    std::cout << "Middle Name [" << defaultContact.getMiddleName() << "]: ";
    input = StringUtils::readLine("");
    contact.setMiddleName(input.empty() ? defaultContact.getMiddleName() : input);
    
    //адрес
    std::cout << "Address [" << defaultContact.getAddress() << "]: ";
    input = StringUtils::readLine("");
    contact.setAddress(input.empty() ? defaultContact.getAddress() : input);
    
    //дата рождения
    std::cout << "Birth Date [" << defaultContact.getBirthDate() << "]: ";
    input = StringUtils::readLine("");
    contact.setBirthDate(input.empty() ? defaultContact.getBirthDate() : input);
    
    //телефонные номера
    std::cout << "Phone numbers (current: ";
    auto currentPhones = defaultContact.getPhoneNumbers();
    for (size_t i = 0; i < currentPhones.size(); ++i) {
        if (i > 0) std::cout << ", ";
        std::cout << currentPhones[i];
    }
    std::cout << ")" << std::endl;
    
    std::string phones = inputPhoneNumbers();
    if (!phones.empty()) {
        auto phoneList = StringUtils::split(phones, ',');
        contact.setPhoneNumbers(phoneList);
    } else {
        //если не ввели новые номера, оставляются старые
        contact.setPhoneNumbers(currentPhones);
    }
    
    return contact;
}