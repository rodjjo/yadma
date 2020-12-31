#include <stdio.h>
#include <string.h>
#include <sstream>
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
std::string CAccountsPages::GetParam( void *ev_data, const char* ParamName, bool& Found )
{
    Found = false;
    
    std::string RetVal = get_var( ParamName, ev_data );
    
    Found = RetVal.size() != 0;
    
    return RetVal;
}

//--------------------------------------------------------------------------------------------------------
bool CAccountsPages::SaveAccount( void *ev_data )
{
    bool found = false;
    
    int Id_Server = StrToInt( GetParam( ev_data, "id", found ) );
    
    if ( found && ( Id_Server < 1 ) )
    {
        return false;
    }
     
    std::string Url, UserName, Password, LoginUrl, 
                LoginUserVar, LoginPassVar, Prefix, AditionalVars;
    
    Url = GetParam( ev_data, "url", found );
    if ( !found ) return false;
    
    UserName = GetParam( ev_data, "username", found );
    if ( !found ) return false;
     
    Password = GetParam( ev_data, "password", found );
    if ( !found ) return false;
     
    LoginUrl  = GetParam( ev_data, "loginurl", found );
    if ( !found ) return false;
     
    LoginUserVar = GetParam( ev_data, "uservar", found );
    if ( !found ) return false;
     
    LoginPassVar = GetParam( ev_data, "passvar", found );
    if ( !found ) return false;
    
    Prefix = GetParam( ev_data, "prefix", found );
    if ( !found ) return false;
    
    AditionalVars = GetParam( ev_data, "additional", found );
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
std::pair<int,std::string> CAccountsPages::HandleRequest( void *ev_data )
{
    bool ParamFound = false;
    std::string response;
        
    if( GetParam( ev_data, "delete", ParamFound ) == "1" )
    {
        int IdServer = StrToInt( GetParam( ev_data, "id", ParamFound ) );
        
        if ( ParamFound )
        {
            m_DataBase->Execute( ( std::string( "DELETE FROM TB_SERVERS WHERE ID_SERVER = " ) + IntToStr( IdServer ) ).c_str() );
        }
        
        m_Autenticator->Update();
    }
    else if( is_post(ev_data) ) 
    {
        SaveAccount( ev_data );
    }
    else
    {
        yadmaptr<IDBQuery> Query = m_DataBase->Query( "SELECT * FROM TB_SERVERS ORDER BY URL" );
    
        while( Query->Next() )
        {
            std::stringstream ss;
            ss << ACCOUNT_ID_SERVER << ":"  << Query->AsString( "ID_SERVER" ) << "\n";
            ss << ACCOUNT_URL << ":"  << Query->AsString( "URL" ) << "\n";

            ss << ACCOUNT_URL << ":"  << Query->AsString( "URL" ) << "\n";
            ss << ACCOUNT_USERNAME << ":"  << Query->AsString( "USERNAME" ) << "\n";
            ss << ACCOUNT_PASSWORD << ":"  << Query->AsString( "PASSWORD" ) << "\n";
            ss << ACCOUNT_LOGINURL << ":"  << Query->AsString( "LOGINURL" ) << "\n";
            ss << ACCOUNT_LOGINUSERVAR << ":"  << Query->AsString( "LOGINUSERVAR" ) << "\n";
            ss << ACCOUNT_LOGINPASSVAR << ":"  << Query->AsString( "LOGINPASSVAR" ) << "\n";
            ss << ACCOUNT_PREFIX << ":"  << Query->AsString( "PREFIX" ) << "\n";
            ss << ACCOUNT_ADDITIONAL << ":"  << Query->AsString( "ADDITIONALVARS" ) << "\n";
            
            response = ss.str();
        }
    }    
    return std::make_pair(200, response);
}
