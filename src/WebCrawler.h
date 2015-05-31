#ifndef __WEBCRAWLER_H__
#define __WEBCRAWLER_H__

#include "Utility.h"

#include <curl/curl.h>

class WebCrawler
{
public:
    WebCrawler();
    ~WebCrawler();
    
    bool initialize();
    bool tryGetData(const QueryData* qData, const std::string& url, std::string& data);
    
    friend size_t saveData(void* buf, size_t size, size_t nmemb, void* userp);
    
private:
    CURL* curl;
    std::string dataRecieved;
};

#endif
