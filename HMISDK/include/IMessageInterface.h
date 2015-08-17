#ifndef IMESSAGEINTERFACE_H
#define IMESSAGEINTERFACE_H

#include "json/json.h"
#include <string>


class IMessageInterface
{
public:
    virtual void onRequest(Json::Value) = 0;
    virtual void onNotification(Json::Value) = 0;
    virtual void onResult(Json::Value) = 0;
    virtual void onRawData(void * p, int iLength) = 0;
    virtual void onError(std::string error) = 0;
};

class INetworkStatus
{
public:
    virtual void onConnected() = 0;
    virtual void onNetworkBroken() = 0;
};

#endif // IMESSAGEINTERFACE_H
