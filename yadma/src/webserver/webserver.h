#ifndef WEBSERVER_H_
#define WEBSERVER_H_

#include <stdio.h>
#include <map>
#include <pthread.h>
#include "../util/yadmaptr.h"
#include "mongoose.hpp"
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

protected:
    void Run() override;

private:
    static void WebServerCallBack( struct mg_connection *c, int ev, void *ev_data, void *fn_data );
    void HandleEvent(struct mg_connection *c, int ev, void *ev_data);

private:
    TListenerMap m_Listeners;
    yadmaptr<ISettings> m_Settings;
    pthread_cond_t m_StopCondition;
    pthread_mutex_t m_StopMutex;
    struct mg_mgr   m_mgr;
};

#endif /*WEBSERVER_H_*/
