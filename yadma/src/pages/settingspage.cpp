#include <string.h>

#include "pagedefines.h"
#include "../util/utils.h"
#include "settingspage.h"

#define SETTINGS_WEB_SERVER_NAME 1
#define SETTINGS_WEB_SERVER_PORT 2
#define SETTINGS_DOWNLOAD_DIRECTORY 3
#define SETTINGS_DOWNLOAD_MAX_ACTIVE 4
#define SETTINGS_PROXY_ADDRESS 5
#define SETTINGS_PROXY_PORT 6
#define SETTINGS_PROXY_ACTIVATE 7
#define SETTINGS_PROXY_HTTPS 8

//-----------------------------------------------------------------------------------------------------
CSettingsPage::CSettingsPage( yadmaptr<ISettings> Settings )
{
	m_Settings = Settings;
}

//-----------------------------------------------------------------------------------------------------
CSettingsPage::~CSettingsPage()
{
}

//-----------------------------------------------------------------------------------------------------
void CSettingsPage::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	bool Error = false;
	
	if( strcmp( request_info->request_method, "POST" ) == 0 ) 
	{
		Error = !SaveSettings( conn );
	}
	else
	{
		mg_printf( conn, "%s", PAGE_HEADER );
	
		mg_printf( conn, "%d:%s\n", SETTINGS_WEB_SERVER_NAME, GetSettingValue( "WEB_SERVER_NAME" ).c_str() );
		mg_printf( conn, "%d:%s\n", SETTINGS_WEB_SERVER_PORT, GetSettingValue( "WEB_SERVER_PORT" ).c_str() );
		mg_printf( conn, "%d:%s\n", SETTINGS_DOWNLOAD_DIRECTORY, GetSettingValue( "DOWNLOAD_DIRECTORY" ).c_str() );
		mg_printf( conn, "%d:%s\n", SETTINGS_DOWNLOAD_MAX_ACTIVE, GetSettingValue( "DOWNLOAD_MAX_ACTIVE" ).c_str() );
		mg_printf( conn, "%d:%s\n", SETTINGS_PROXY_ADDRESS, GetSettingValue( "PROXY_ADDRESS" ).c_str() );
		mg_printf( conn, "%d:%s\n", SETTINGS_PROXY_PORT, GetSettingValue( "PROXY_PORT" ).c_str() );
		mg_printf( conn, "%d:%s\n", SETTINGS_PROXY_ACTIVATE, m_Settings->GetStrValue("PROXY_ACTIVATE", "N" ).c_str() );
		mg_printf( conn, "%d:%s\n", SETTINGS_PROXY_HTTPS, m_Settings->GetStrValue("PROXY_HTTPS", "N" ).c_str() );
	}
}

//-----------------------------------------------------------------------------------------------------
bool CSettingsPage::SaveSettings( struct mg_connection* conn )
{
	UpdateSetting( "server_name", "WEB_SERVER_NAME", conn );
	UpdateSetting( "server_port", "WEB_SERVER_PORT", conn );
	UpdateSetting( "down_dir", "DOWNLOAD_DIRECTORY", conn );
	UpdateSetting( "down_max", "DOWNLOAD_MAX_ACTIVE", conn );
	UpdateSetting( "proxy_addr", "PROXY_ADDRESS", conn );
	UpdateSetting( "proxy_port", "PROXY_PORT", conn );
	UpdateSetting( "proxy_activate", "PROXY_ACTIVATE", conn );
	UpdateSetting( "proxy_https", "PROXY_HTTPS", conn );
	
	return true;
}

//-----------------------------------------------------------------------------------------------------
std::string CSettingsPage::GetSettingValue( const std::string& SettingName )
{
	return HtmlEncode(   ReplaceAll( m_Settings->GetStrValue( SettingName, "" ), "'", "''" ) );
}

//-----------------------------------------------------------------------------------------------------
void CSettingsPage::UpdateSetting( const std::string& InputName, const std::string& ConfigName, struct mg_connection* conn )
{
	char* Var = mg_get_var( conn, InputName.c_str() );
	if( Var )
	{
		m_Settings->SetStrValue( ConfigName, Var );
		mg_free( Var );
	}
}

