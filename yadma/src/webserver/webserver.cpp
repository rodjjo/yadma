#include <stdio.h>
#include <iostream>
#include "../pages/pagedefines.h"
#include "../util/pathutil.h"
#include "webserver.h"


//----------------------------------------------------------------------------------
CWebServer::CWebServer( yadmaptr<ISettings> Settings  )
{
    m_Settings = Settings;
}

//----------------------------------------------------------------------------------
CWebServer::~CWebServer()
{
    Stop();
}

//----------------------------------------------------------------------------------
void CWebServer::HandleEvent(struct mg_connection *c, int ev, void *ev_data)
{
    if (ev != MG_EV_HTTP_MSG) {
        return;
    }

    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    for( TListenerMap::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++ ) {
        if (mg_http_match_uri(hm, it->first.c_str())) {
            auto r = it->second->HandleRequest(ev_data);
            mg_http_reply(c, r.first, "", "%s", r.second.c_str() );
            return;
        }
    }
    if (mg_http_match_uri(hm, "/") || mg_http_match_uri(hm, "/index.html") || mg_http_match_uri(hm, "/index.htm")) {
        mg_http_serve_file(c, hm, (GetAppDirectory() + "../www/index.htm" ).c_str(), "text/html");
        return;
    }
    mg_http_serve_dir(c, hm, (GetAppDirectory() + "../www" ).c_str());
}

//----------------------------------------------------------------------------------
void CWebServer::Run() {
    auto s_listen_on = std::string("https://0.0.0.0:") + m_Settings->GetStrValue( "WEB_SERVER_PORT", "8080" );
    mg_mgr_init(&m_mgr);
    mg_http_listen(&m_mgr, s_listen_on.c_str(), CWebServer::WebServerCallBack, this);
    while (!StopRequested()) {
        mg_mgr_poll(&m_mgr, 100);
    }
    mg_mgr_free(&m_mgr);
}


//----------------------------------------------------------------------------------
void CWebServer::AddListener( const std::string& Resource, yadmaptr<IWebServerListener> Listener )
{
    if( m_Listeners.find( Resource ) != m_Listeners.end() )
    {
        throw "WebServer: Listener ja incluido para este recurso.";
    }

    m_Listeners[ Resource ] = Listener;
}

//----------------------------------------------------------------------------------
void CWebServer::WebServerCallBack( struct mg_connection *c, int ev, void *ev_data, void *fn_data )
{
    ((CWebServer *) fn_data)->HandleEvent(c, ev, ev_data);
}
