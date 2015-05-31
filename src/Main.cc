#include "RedisController.h"
#include "WebCrawler.h"

int main(int argc, char **argv)
{
    std::string cities = std::string("");
    std::string city1 = std::string(), city2 = std::string();

    std::cout << "City Pair: ";

    std::getline(std::cin, cities);
    
    size_t separatorPos = cities.find_first_of("-,;:");
    
    if(separatorPos == std::string::npos)
        separatorPos = cities.find_first_of(" \t");

    //Separating the city names
    city1 = cities.substr(0, separatorPos);
    city2 = cities.substr(separatorPos + 1);

    //Trimming the 2 city names for common whitespace values
    std::string fromCity = Utility::trim(city1);
    std::string toCity = Utility::trim(city2);
    
    fromCity = Utility::toLower(fromCity);
    toCity = Utility::toLower(toCity);
    

    QueryData* qData = new QueryData();
    qData->sourceCity = fromCity.c_str();
    qData->destinationCity = toCity.c_str();
    qData->minDateOfTravel = Utility::getCurrentDateStr(false);
    qData->altDateOfTravelString = Utility::getCurrentDateStr(true);
    qData->returnDate = NULL;


    std::string data = std::string();

    RedisController rc = RedisController();
    if(!rc.connectToRedis(NULL, 0))
    {
        std::cout << "Error: Cannot connect to Redis Server." << std::endl;
        return 0;
    }
    
    if(rc.tryGetData(qData, data))
    {
        //Printout the data if it is available on Redis
        std::cout << data << std::endl;    
        return 0; 
    }

    //Continue on if data isn't available on Redis

    WebCrawler wc = WebCrawler();
    if(!wc.initialize())
    {
        std::cout << "Error: Unable to initialize Web Crawler." << std::endl;
        return 0;
    }

    if(!wc.tryGetData(qData, "http://www.travelyaari.com/api/search/?mode=oneway&departDate=", data))
    {
        std::cout << data << std::endl;
        return 0;
    }
    
    //Continue on if the data is recieved from the Web Crawler
    
    std::string errorString = std::string();
    
    if(!rc.writeData(qData, data, errorString))
        std::cout << errorString << std::endl;
    
    //Show user the recieved data regardless of whether the data has been entered into Redis or not
    std::cout << data << std::endl;
    
    delete qData;
    return 0;
}

