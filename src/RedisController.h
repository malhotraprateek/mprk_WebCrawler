#ifndef __CODE_H__
#define __CODE_H__

#include "Utility.h"

#include <hiredis.h>

class RedisController
{
public:
    RedisController();
    ~RedisController();
    
    bool connectToRedis(const char* _hostname, int _port);
    bool tryGetData(const QueryData* qData, /*OUT Param*/std::string& data);
    bool writeData(const QueryData* qData, const std::string& data, std::string& errorString);

private:
    redisContext* mpContext;
};

#endif
