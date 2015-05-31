#include "WebCrawler.h"

/* Global, as passing it as a function ptr will change it's prototype("this" pointer
    will be the first parameter), as is with all class member functions. */
size_t saveData(void* buf, size_t size, size_t nmemb, void* userp)
{
    WebCrawler* wc = NULL;

    if(userp != NULL)
        wc = (WebCrawler*) userp;

    wc->dataRecieved.append(static_cast<char*>(buf));
    
    return size * nmemb;
}

WebCrawler::WebCrawler()
{    
    dataRecieved = std::string();
}

WebCrawler::~WebCrawler()
{
    dataRecieved.clear();
    curl_easy_cleanup(curl);
}

bool WebCrawler::initialize()
{
    curl = curl_easy_init();
    
    if(!curl)
        return false;
        
    return true;
}

bool WebCrawler::tryGetData(const QueryData* qData, const std::string& baseURL, std::string& data)
{
    if(curl) 
    {
        std::string url = std::string(baseURL);
        url.append(qData->altDateOfTravelString);
        url.append("&fromCity=");
        url.append(qData->sourceCity);
        url.append("&toCity=");
        url.append(qData->destinationCity);
        url.append("&pickups=1");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, saveData);

        dataRecieved.clear();

        CURLcode res = curl_easy_perform(curl);

        if(res != CURLE_OK)
        {
            data.clear();
            data.append("Error: Cannot execute function curl_easy_perform(). Check if you are connected to the internet.");
            
            return false;
        }

        data.clear();
        data.append(dataRecieved);
    }
    else
    {
        data.clear();
        data.append("Error: Web Crawler not instantiated.");
        
        return false;
    }

    return true;
}
