#include "console/ConsoleUI.h"
#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

int main() {
    
    try {
        ConsoleUI app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}