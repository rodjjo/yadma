#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <stdio.h>
#include <map>
#include <pthread.h>
#include "../util/yadmaptr.h"
#include <mongoose/mongoose.h>

#include "iwebserver.h"
#include "../settings/isettings.h"



//-------------------------------------------------------------------------
class CWebServer: public IWebServer
{
	typedef std::map<std::string, yadmaptr<IWebServerListener> > TListenerMap;
public:
	CWebServer( yadmaptr<ISettings> Settings );
	virtual ~CWebServer();
	void AddListener( const std::string& Resource, yadmaptr<IWebServerListener> Listener );
	bool Start();
	void Stop();
	
private:
	void RegisterCallBacks();
	static void WebServerCallBack( struct mg_connection *conn, const struct mg_request_info *request_info, void *user_data );
	
private:
	TListenerMap 												m_Listeners;
	yadmaptr<ISettings> 										m_Settings;
	struct mg_context* 											m_context; 
	pthread_cond_t												m_StopCondition;
	pthread_mutex_t												m_StopMutex;
};

#endif /*WEBSERVER_H_*/
