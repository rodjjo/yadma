#include <unistd.h>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include "../util/utils.h"

#include "autenticator.h"
 
//--------------------------------------------------------------
CAutenticator::CAutenticator( yadmaptr<IDataBase> DataBase )
{
	m_DataBase = DataBase;
	
	Update();
}

//--------------------------------------------------------------
void CAutenticator::Update()
{
	CMutexLocker Locker( m_mtxAutenticator );
	
	m_Infos.clear();
	
	yadmaptr<IDBQuery> Query = m_DataBase->Query( "SELECT "
  													"ID_SERVER, URL, USERNAME, PASSWORD, LOGINURL, LOGINUSERVAR, LOGINPASSVAR, COOKIES, PREFIX, ADDITIONALVARS "
												  "FROM TB_SERVERS" );
	while( Query->Next() )
	{
		TAutenticationInfo Info = { 0, };
		
		Info.id_server 	= Query->AsInteger( "ID_SERVER" );
		strncpy( Info.Server, Query->AsString( "URL" ).c_str(), sizeof(Info.Server) );
		strncpy( Info.UserName, Query->AsString( "USERNAME" ).c_str(), sizeof(Info.UserName) );
		strncpy( Info.Password, Query->AsString( "PASSWORD" ).c_str(), sizeof(Info.Password) );
		strncpy( Info.LoginUrl, Query->AsString( "LOGINURL" ).c_str(), sizeof(Info.LoginUrl) );
		strncpy( Info.UserNameVar, Query->AsString( "LOGINUSERVAR" ).c_str(), sizeof(Info.UserNameVar) );
		strncpy( Info.PasswordVar, Query->AsString( "LOGINPASSVAR" ).c_str(), sizeof(Info.PasswordVar) );
		strncpy( Info.Token, Query->AsString( "LOGINVALIDATE" ).c_str(), sizeof(Info.Token) );
		strncpy( Info.LastCookie, Query->AsString( "COOKIES" ).c_str(), sizeof(Info.LastCookie) );
		strncpy( Info.Prefix, Query->AsString( "PREFIX" ).c_str(), sizeof(Info.Prefix) );
		strncpy( Info.Additional, Query->AsString( "ADDITIONALVARS" ).c_str(), sizeof(Info.Additional) );
			
		m_Infos.push_back( Info );	
	}	
}

bool CAutenticator::Compatible( const std::string& Server, const std::string& Url )
{
	std::string Temp = Server;
	
	size_t p = 0;
	
	while ( !Temp.empty() )
	{
		if( ( p = Temp.find("|") ) == std::string::npos )
		{
			return Url.find( Temp.c_str() ) != std::string::npos;
		}
		
		if ( Url.find( Temp.substr( 0, p ).c_str() ) != std::string::npos )
		{
			return true;
		}
		
		Temp = Temp.substr( p + 1 );	
	}
	
	return false;
}

//--------------------------------------------------------------
std::string CAutenticator::GetAutenticationCookie( const std::string& Url, std::string* Prefix  )
{
	CMutexLocker Locker( m_mtxAutenticator );
	
	for ( std::list<TAutenticationInfo>::iterator it = m_Infos.begin(); it != m_Infos.end(); it++ )
	{
		if(  Compatible( it->Server, Url )  )
		{
			if(  Prefix != (std::string*)0 )
			{
				*Prefix = it->Prefix;
			}
			return NewAutentication( it );
		}
	}
	
	
	return "";
}

//--------------------------------------------------------------
size_t CAutenticator::WriteNull( void *Buffer, size_t ElementSize, size_t ElementCount, void *UserData )
{
	return ElementCount * ElementSize;	
}

//--------------------------------------------------------------
std::string CAutenticator::GetLastCookie( const std::string& Url, std::string* Prefix  )
{
	for ( std::list<TAutenticationInfo>::iterator it = m_Infos.begin(); it != m_Infos.end(); it++ )
	{
		if( Compatible( it->Server, Url ) )
		{
			if(  Prefix != (std::string*)0 )
			{
				*Prefix = it->Prefix;
			}
			return it->LastCookie;
		}
	}
	return "";
}

//--------------------------------------------------------------
std::string CAutenticator::NewAutentication( std::list<TAutenticationInfo>::iterator Info )
{
	std::string CapturedCookies;

	CURL* Curl = curl_easy_init();

    if ( Curl )
    {
    	char PostData[ 400 ] = "";
    	
    	if ( Info->Additional[0] == '\0' )
    	{
    		sprintf( PostData, "%s=%s&%s=%s", Info->UserNameVar, Info->UserName, Info->PasswordVar, Info->Password );
    	}
    	else
    	{
    		sprintf( PostData, "%s=%s&%s=%s&%s", Info->UserNameVar, Info->UserName, Info->PasswordVar, Info->Password, Info->Additional );
    	}
    	
		curl_easy_setopt( Curl, CURLOPT_COOKIEFILE, "" ); // Apenas para iniciar o sistema de  cookies
		curl_easy_setopt( Curl, CURLOPT_COOKIELIST, "" );
		curl_easy_setopt( Curl, CURLOPT_FOLLOWLOCATION, 1L );
    	curl_easy_setopt( Curl, CURLOPT_MAXREDIRS, 8L );
    	curl_easy_setopt( Curl, CURLOPT_SSL_VERIFYPEER, 0L);
    	curl_easy_setopt( Curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, CAutenticator::WriteNull );
        curl_easy_setopt( Curl, CURLOPT_NOSIGNAL, 1L ); 				//para nao ter problemas com threads
        curl_easy_setopt( Curl, CURLOPT_CONNECTTIMEOUT, 20L );
        curl_easy_setopt( Curl, CURLOPT_LOW_SPEED_LIMIT, 1000L );
        curl_easy_setopt( Curl, CURLOPT_LOW_SPEED_TIME, 10L );
        curl_easy_setopt( Curl, CURLOPT_POST, 1L );
        curl_easy_setopt( Curl, CURLOPT_POSTFIELDS, PostData ); 
        curl_easy_setopt( Curl, CURLOPT_URL, Info->LoginUrl );
            
        curl_easy_perform( Curl );
        
        struct curl_slist* Cookies = NULL;
        
        curl_easy_getinfo( Curl, CURLINFO_COOKIELIST, &Cookies );
                
        struct curl_slist* nc = Cookies;
        
        while( nc ) 
        {
        	CapturedCookies += nc->data;
        	CapturedCookies += "\n";
        	
        	nc = nc->next;
        }
       
        curl_slist_free_all( Cookies );
        
        curl_easy_cleanup( Curl ); 	
    }
    
	if ( !CapturedCookies.empty() )
	{
		strncpy( Info->LastCookie, CapturedCookies.c_str(), sizeof(Info->LastCookie) );
		
		std::vector<std::string> Params;
		
		Params.push_back( Info->LastCookie );
		Params.push_back( IntToStr( Info->id_server ) );
	
		m_DataBase->Execute( "UPDATE TB_SERVERS SET "
								"COOKIES = ?"
							" WHERE ID_SERVER = ?" , Params );
		
	}
		
	return CapturedCookies;
}
