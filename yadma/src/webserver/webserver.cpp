#include <stdio.h>
#include <iostream> 
#include <mongoose/mongoose.h>
#include "../pages/pagedefines.h"
#include "../util/pathutil.h"
#include "webserver.h"

//----------------------------------------------------------------------------------
CWebServer::CWebServer( yadmaptr<ISettings> Settings  )
{
	m_Settings 		= Settings;
	m_context   	= NULL;
}

//----------------------------------------------------------------------------------
CWebServer::~CWebServer()
{
	Stop();
}

//----------------------------------------------------------------------------------
bool CWebServer::Start()
{
	if( m_context == NULL )
	{
		m_context = mg_start();	
		
		if( m_context )
		{
			if( mg_set_option( m_context, "root", (GetAppDirectory() + "../www" ).c_str() ) == 1 )
			{
				RegisterCallBacks();
				
				if( mg_set_option( m_context, "ports", m_Settings->GetStrValue( "WEB_SERVER_PORT", "8080" ).c_str() ) == 1 )
				{
					return true;
				}
				else
				{
					fprintf( stderr, "Error: listening port %s", m_Settings->GetStrValue( "WEB_SERVER_PORT", "8080" ).c_str() );
				}
			}
			else
			{
				fprintf( stderr, "Error: serving directory %s", (GetAppDirectory() + "../www/").c_str() ); 
			}
			
			mg_stop( m_context );
			m_context = NULL;	
		}
		else
		{
			fprintf( stderr, "Error: WebServer  context not initialized...." );	
		}
	}
	
	return false;
}

//----------------------------------------------------------------------------------
void CWebServer::Stop()
{
	if( m_context != NULL )
	{
		mg_stop( m_context );
		m_context = NULL;	
	}
}

//----------------------------------------------------------------------------------
void CWebServer::AddListener( const std::string& Resource, yadmaptr<IWebServerListener> Listener )
{
	if( m_context )
	{
		throw "WebServer: Impossivel adicionar listener, servidor rodando.";
	}
	
	if( m_Listeners.find( Resource ) != m_Listeners.end() )
	{
		throw "WebServer: Listener ja incluido para este recurso."; 
	}
	
	m_Listeners[ Resource ] = Listener; 
}

//----------------------------------------------------------------------------------
void  CWebServer::RegisterCallBacks()
{
	for( TListenerMap::iterator it = m_Listeners.begin(); it != m_Listeners.end(); it++ )
	{
		mg_set_uri_callback( m_context, it->first.c_str(), &CWebServer::WebServerCallBack, (void *) &it->second );
	}
}

//----------------------------------------------------------------------------------
void CWebServer::WebServerCallBack( struct mg_connection *conn, const struct mg_request_info *request_info, void *user_data )
{
 	yadmaptr<IWebServerListener>* Listener = ( yadmaptr<IWebServerListener>* ) user_data;
 	(*Listener)->HandleRequest( conn, request_info );
}
