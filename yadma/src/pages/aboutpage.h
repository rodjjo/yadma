#ifndef ABOUTPAGE_H_
#define ABOUTPAGE_H_

#include "../webserver/iwebserverlistener.h"

class CAboutPage: public IWebServerListener
{
public:
	CAboutPage();
	virtual ~CAboutPage();
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
};	
	
#endif /*ABOUTPAGE_H_*/
