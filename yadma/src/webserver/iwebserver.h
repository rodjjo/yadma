#ifndef IWEBSERVER_H_
#define IWEBSERVER_H_

#include <string>
#include "../util/yadmaptr.h"
#include "iwebserverlistener.h"

//------------------------------------------------------------------------------------
class IWebServer
{
public:
	virtual ~IWebServer(){};
	virtual void AddListener( const std::string& resource, yadmaptr<IWebServerListener> Listener ) = 0;
	virtual bool Start() = 0;
	virtual void Stop() = 0;
};

#endif /*IWEBSERVER_H_*/
