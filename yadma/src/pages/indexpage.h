#ifndef INDEXPAGE_H_
#define INDEXPAGE_H_

#include "../webserver/iwebserverlistener.h"

class CIndexPage: public IWebServerListener
{
public:
	CIndexPage( );
	virtual ~CIndexPage();
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
};

#endif /*INDEXPAGE_H_*/
