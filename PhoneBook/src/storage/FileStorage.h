#ifndef FILESTORAGE_H
#define FILESTORAGE_H

#include <string>
#include <vector>

class Contact;

class FileStorage {
private:
    std::string filename;

public:
    FileStorage(const std::string& filename);
    
    //основные операции
    bool save(const std::vector<Contact>& contacts);
    std::vector<Contact> load();
    
    //утилиты
    bool backupExists() const;
    bool createBackup();
    bool restoreFromBackup();
    
    std::string getFilename() const { return filename; }
    void setFilename(const std::string& newFilename) { filename = newFilename; }
};

#endif