#include "Utility.h"

const char* Utility::getCurrentDateStr(bool alternateString)
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    if(alternateString)
    {
        char* date = new char[11];

        sprintf(date, "%02d-%02d-%04d", ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);

        return date;
    }

    char weekday[WEEKDAYSIZE + 1];
    char monthstr[MONTHSTRSIZE + 1];

    //Date to WeekDay string
    if(strftime (weekday, WEEKDAYSIZE + 1, "%a", ltm) == 0)
    {
        std::cout << "Error: Could not convert date into weekday." << std::endl;
        return NULL;
    }

    //Month to Month string
    if(strftime (monthstr, MONTHSTRSIZE + 1, "%b", ltm) == 0)
    {
        std::cout << "Error: Could not convert month into month string." << std::endl;
        return NULL;
    }

    char* buffer = new char[50];
    sprintf(buffer, "%s, %d-%s-%d", weekday, ltm->tm_mday, monthstr, 1900 + ltm->tm_year);

    return buffer;
}

std::string Utility::trim(const std::string& str)
{
    if(str.empty())
        return str;

    std::size_t first = str.find_first_not_of(" \t\n");
    std::size_t last  = str.find_last_not_of(" \t\n");

    return str.substr(first, last-first+1);
}

std::string Utility::toLower(const std::string& str)
{
    size_t size = str.size();
    char* charArray = new char[size + 1];
    
    for(size_t i = 0; i < size; i++)
        charArray[i] = ::tolower(str[i]);
        
    charArray[size] = 0;
        
    return std::string(charArray);
}

