#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <vector>

class StringUtils {
public:
    //работа со строками
    static std::string trim(const std::string& str);
    static std::string toLower(const std::string& str);
    static std::string toUpper(const std::string& str);
    static bool containsIgnoreCase(const std::string& str, const std::string& substr);
    
    //разделение и объединение строк
    static std::vector<std::string> split(const std::string& str, char delimiter);
    static std::string join(const std::vector<std::string>& strings, const std::string& delimiter);
    
    //ввод данных
    static std::string readLine(const std::string& prompt = "");
    static int readInt(const std::string& prompt = "");
    
    //форматирование
    static std::string formatPhone(const std::string& phone);
    static std::string formatName(const std::string& name);
};

#endif