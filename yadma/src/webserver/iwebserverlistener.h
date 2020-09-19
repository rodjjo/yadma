#ifndef IWEBSERVERLISTENER_H_
#define IWEBSERVERLISTENER_H_

#include <stdio.h>
#include <mongoose/mongoose.h>

#define PAGE_HEADER "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"

//-----------------------------------------------------------------------------------------------------------------------
class IWebServerListener
{
public:
	virtual ~IWebServerListener(){};
	virtual void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info ) = 0;
};

#endif /*IWEBSERVERLISTENER_H_*/
