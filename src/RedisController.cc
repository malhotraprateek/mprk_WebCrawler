#include "RedisController.h"

RedisController::RedisController()
{
    mpContext = NULL;
}

RedisController::~RedisController()
{        
    if(mpContext != NULL)
        redisFree(mpContext);
}

bool RedisController::connectToRedis(const char* _hostname, int _port)
{
    const char* hostname = NULL;
    int port = 0;
    
    if(_hostname == NULL)
        hostname = "127.0.0.1";
    else
        hostname = _hostname;
        
    if(_port <= 0)
        port = 6379;
    else
        port = _port;

    struct timeval timeout = { 1, 500000 };
    mpContext = redisConnectWithTimeout(hostname, port, timeout);

    if (mpContext == NULL || mpContext->err) 
    {
        if (mpContext) 
        {
            std::cout << "Connection error: " << mpContext->errstr << std::endl;
            redisFree(mpContext);
        } 
        else
            std::cout << "Connection error: can't allocate redis context" << std::endl;

        return false;
    }
    
    return true;
}

bool RedisController::tryGetData(const QueryData* qData, std::string& data)
{
    if(mpContext == NULL)
    {
        data.clear();
        data.append("Error: Not connected to any Redis Server.");
        
        return false;
    }

    //Finding pair in Redis cache server
    std::string redisCmd = std::string("hgetall busQuery:");

    redisCmd.append(qData->sourceCity);
    redisCmd.append(":");
    redisCmd.append(qData->destinationCity);

    redisReply* reply = static_cast<redisReply*>(redisCommand(mpContext, redisCmd.c_str()));

    if ( reply->type == REDIS_REPLY_ERROR )
    {
        data.clear();
        data.append("Redis Error: ");
        data.append(reply->str);
        
        freeReplyObject(reply);
        return false;
    }
    else if(reply->elements > 0)
    {
        data.clear();
        
        //Accumulate the data
        for(size_t i = 0; i < reply->elements; ++i)
            data.append(reply->element[i]->str);
    }
    else if(reply->elements == 0)
    {
        //Data not available
        
        data.clear();
        
        freeReplyObject(reply);
        return false;
    }
    else
    {
        data.clear();
        data.append("Error: Unknown Error. Aborting...");
        
        freeReplyObject(reply);
        return false;
    }
    
    freeReplyObject(reply);
    return true;
}

bool RedisController::writeData(const QueryData* qData, const std::string& data, std::string& errorString)
{
    std::string redisCmd = std::string("");

    redisCmd.append("hset busQuery:");
    redisCmd.append(qData->sourceCity);
    redisCmd.append(":");
    redisCmd.append(qData->destinationCity);

    redisCmd.append(" Options %b");
    
    redisReply* reply = static_cast<redisReply*>(redisCommand(mpContext, redisCmd.c_str(), data.c_str(), data.size()));

    if ( reply->type == REDIS_REPLY_ERROR )
    {
        errorString.clear();
        errorString.append("Redis Error: ");
        errorString.append(reply->str);
        
        freeReplyObject(reply);
        return false;
    }

    freeReplyObject(reply);

    errorString.clear();
    return true;
}
