#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <iostream>
#include <vector>
#include <string>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WEEKDAYSIZE 3
#define MONTHSTRSIZE 3

struct QueryData
{
    const char* sourceCity;
    const char* destinationCity;
    const char* minDateOfTravel;
    const char* altDateOfTravelString;
    const char* returnDate;             //Optional Parameter
};

class Utility
{
public:
    static const char* getCurrentDateStr(bool alternateString);
    static std::string trim(const std::string& str);
    static std::string toLower(const std::string& str);
};

#endif
