#ifndef TOPPAGE_H_
#define TOPPAGE_H_

#include "../webserver/iwebserverlistener.h"

class CTopPage: public IWebServerListener
{
public:
	CTopPage( );
	virtual ~CTopPage();
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
};

#endif /*TOPPAGE_H_*/
