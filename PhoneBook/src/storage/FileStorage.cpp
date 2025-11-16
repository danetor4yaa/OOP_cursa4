#include "FileStorage.h"
#include "../core/Contact.h"
#include <fstream>
#include <iostream>

FileStorage::FileStorage(const std::string& filename) 
    : filename(filename) {}

bool FileStorage::save(const std::vector<Contact>& contacts) {
    //backup перед сохранением
    createBackup();
    
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл для записи: " << filename << std::endl;
        return false;
    }
    
    for (const auto& contact : contacts) {
        file << contact.toString() << std::endl;
    }
    
    file.close();
    return true;
}

std::vector<Contact> FileStorage::load() {
    std::vector<Contact> contacts;
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cout << "Файл не найден, будет создан новый: " << filename << std::endl;
        return contacts;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            try {
                Contact contact = Contact::fromString(line);
                contacts.push_back(contact);
            } catch (const std::exception& e) {
                std::cerr << "Ошибка при чтении контакта: " << e.what() << std::endl;
            }
        }
    }
    
    file.close();
    return contacts;
}

bool FileStorage::backupExists() const {
    std::string backupFile = filename + ".backup";
    std::ifstream file(backupFile);
    return file.good();
}

bool FileStorage::createBackup() {
    std::ifstream source(filename, std::ios::binary);
    if (!source) {
        return true;
    }
    
    std::string backupFile = filename + ".backup";
    std::ofstream dest(backupFile, std::ios::binary);
    
    if (!dest) {
        std::cerr << "Ошибка при создании бэкапа" << std::endl;
        return false;
    }
    
    dest << source.rdbuf();
    return source && dest;
}

bool FileStorage::restoreFromBackup() {
    std::string backupFile = filename + ".backup";
    std::ifstream source(backupFile, std::ios::binary);
    if (!source) {
        return false;
    }
    
    std::ofstream dest(filename, std::ios::binary);
    if (!dest) {
        return false;
    }
    
    dest << source.rdbuf();
    return source && dest;
}