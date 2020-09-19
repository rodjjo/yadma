#include <stdio.h>
#include <string.h>
#include "accountspages.h"
#include "../util/utils.h"

#define  ACCOUNT_ID_SERVER 1
#define  ACCOUNT_URL 2
#define  ACCOUNT_USERNAME 3
#define  ACCOUNT_PASSWORD 4
#define  ACCOUNT_LOGINURL 5
#define  ACCOUNT_LOGINUSERVAR 6 
#define  ACCOUNT_LOGINPASSVAR 7
#define  ACCOUNT_PREFIX 8
#define  ACCOUNT_ADDITIONAL 9

//--------------------------------------------------------------------------------------------------------
CAccountsPages::CAccountsPages( yadmaptr<IDataBase> DataBase, yadmaptr<IAutenticator> Autenticator )
{
	m_DataBase = DataBase;
	m_Autenticator = Autenticator;
}

//--------------------------------------------------------------------------------------------------------
CAccountsPages::~CAccountsPages()
{
	
}

//--------------------------------------------------------------------------------------------------------
std::string CAccountsPages::GetParam( struct mg_connection* conn, const std::string& ParamName, bool& Found )
{
	Found = false;
	
	std::string RetVal;
	
	char* Var = mg_get_var( conn, ParamName.c_str() );
	
	if( Var )
	{
		RetVal = Var;
		
		mg_free( Var );
		
		Found = true;
	}
	
	return RetVal;
}

//--------------------------------------------------------------------------------------------------------
bool CAccountsPages::SaveAccount(struct mg_connection* conn )
{
	bool found = false;
	
	int Id_Server = StrToInt( GetParam( conn, "id", found ) );
	
	if ( found && ( Id_Server < 1 ) )
	{
		return false;
	}
	 
	std::string Url, UserName, Password, LoginUrl, 
				LoginUserVar, LoginPassVar, Prefix, AditionalVars;
	
	Url = GetParam( conn, "url", found );
	if ( !found ) return false;
	
	UserName = GetParam( conn, "username", found );
	if ( !found ) return false;
	 
	Password = GetParam( conn, "password", found );
	if ( !found ) return false;
	 
	LoginUrl  = GetParam( conn, "loginurl", found );
	if ( !found ) return false;
	 
	LoginUserVar = GetParam( conn, "uservar", found );
	if ( !found ) return false;
	 
	LoginPassVar = GetParam( conn, "passvar", found );
	if ( !found ) return false;
	
	Prefix = GetParam( conn, "prefix", found );
	if ( !found ) return false;
	
	AditionalVars = GetParam( conn, "additional", found );
	if ( !found ) return false;
	
	std::vector<std::string> Params;
	
	Params.push_back( Url );
	Params.push_back( UserName );
	Params.push_back( Password );
	Params.push_back( LoginUrl );
	Params.push_back( LoginUserVar );
	Params.push_back( LoginPassVar );
	Params.push_back( Prefix );
	Params.push_back( AditionalVars );
	
	if ( Id_Server > 0 )
	{
		Params.push_back( IntToStr( Id_Server ) );	
		
		m_DataBase->Execute( "UPDATE "
  								"TB_SERVERS SET "
  								"URL = ?,"
  								"USERNAME = ?,"
  								"PASSWORD = ?,"
  								"LOGINURL = ?,"
  								"LOGINUSERVAR = ?,"
  								"LOGINPASSVAR = ?, "
  								"PREFIX = ?, "
  								"ADDITIONALVARS = ?"
							 " WHERE "
  								"ID_SERVER = ?", Params );
	}
	else
	{
		m_DataBase->Execute( "INSERT INTO TB_SERVERS( URL, USERNAME, PASSWORD, LOGINURL, LOGINUSERVAR, LOGINPASSVAR, PREFIX, ADDITIONALVARS )"
							 "VALUES(?, ?, ?, ?, ?, ?, ?, ? )", Params );	
	}
	
	m_Autenticator->Update();
	
	return true;
}

//--------------------------------------------------------------------------------------------------------
void CAccountsPages::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	bool ParamFound = false;
		
	if( GetParam( conn, "delete", ParamFound ) == "1" )
	{
		int IdServer = StrToInt( GetParam( conn, "id", ParamFound ) );
		
		if ( ParamFound )
		{
			m_DataBase->Execute( ( std::string( "DELETE FROM TB_SERVERS WHERE ID_SERVER = " ) + IntToStr( IdServer ) ).c_str() );
		}
		
		m_Autenticator->Update();
	}
	else if( strcmp( request_info->request_method, "POST" ) == 0 ) 
	{
		SaveAccount( conn );
	}
	else
	{
		yadmaptr<IDBQuery> Query = m_DataBase->Query( "SELECT * FROM TB_SERVERS ORDER BY URL" );
	
		while( Query->Next() )
		{
			mg_printf( conn, "%d:%s\n", ACCOUNT_ID_SERVER, 	Query->AsString( "ID_SERVER" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_URL, 		Query->AsString( "URL" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_USERNAME, 	Query->AsString( "USERNAME" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_PASSWORD, 	Query->AsString( "PASSWORD" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_LOGINURL, 	Query->AsString( "LOGINURL" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_LOGINUSERVAR, 	Query->AsString( "LOGINUSERVAR" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_LOGINPASSVAR, 	Query->AsString( "LOGINPASSVAR" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_PREFIX, 		Query->AsString( "PREFIX" ).c_str() );
			mg_printf( conn, "%d:%s\n", ACCOUNT_ADDITIONAL, 	Query->AsString( "ADDITIONALVARS" ).c_str() );
		}
	}	
	
}
