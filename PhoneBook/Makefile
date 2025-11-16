CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Isrc\core -Isrc\validation -Isrc\storage -Isrc\utils -Isrc\console
SRCDIR = src
SOURCES = $(SRCDIR)\main.cpp \
          $(SRCDIR)\core\Contact.cpp \
          $(SRCDIR)\core\PhoneBook.cpp \
          $(SRCDIR)\validation\Validator.cpp \
          $(SRCDIR)\storage\FileStorage.cpp \
          $(SRCDIR)\utils\StringUtils.cpp \
          $(SRCDIR)\console\ConsoleUI.cpp
TARGET = phonebook.exe

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	del $(TARGET)

run: $(TARGET)
	.\$(TARGET)

.PHONY: clean run