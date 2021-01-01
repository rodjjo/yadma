#ifndef IWEBSERVER_H_
#define IWEBSERVER_H_

#include <string>
#include "../thread/thread.h"
#include "../util/yadmaptr.h"
#include "iwebserverlistener.h"

//------------------------------------------------------------------------------------
class IWebServer: public CThread
{
public:
    virtual ~IWebServer(){};
    virtual void AddListener( const std::string& resource, yadmaptr<IWebServerListener> Listener ) = 0;
};

#endif /*IWEBSERVER_H_*/
