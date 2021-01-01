#include <string.h>
#include <sstream>

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
std::pair<int, std::string> CSettingsPage::HandleRequest( void *ev_data )
{
    std::string response;

    if( is_post(ev_data) )
    {
        response = SaveSettings( ev_data);
    }
    else
    {
        std::stringstream ss;

        ss << SETTINGS_WEB_SERVER_NAME << ":" << GetSettingValue( "WEB_SERVER_NAME" ) << "\n";
        ss << SETTINGS_WEB_SERVER_PORT << ":" << GetSettingValue( "WEB_SERVER_PORT" ) << "\n";
        ss << SETTINGS_DOWNLOAD_DIRECTORY << ":" << GetSettingValue( "DOWNLOAD_DIRECTORY" ) << "\n";
        ss << SETTINGS_DOWNLOAD_MAX_ACTIVE << ":" << GetSettingValue( "DOWNLOAD_MAX_ACTIVE" ) << "\n";
        ss << SETTINGS_PROXY_ADDRESS << ":" << GetSettingValue( "PROXY_ADDRESS" ) << "\n";
        ss << SETTINGS_PROXY_PORT << ":" << GetSettingValue( "PROXY_PORT" ) << "\n";
        ss << SETTINGS_PROXY_ACTIVATE << ":" << m_Settings->GetStrValue("PROXY_ACTIVATE", "N" ) << "\n";
        ss << SETTINGS_PROXY_HTTPS << ":" << m_Settings->GetStrValue("PROXY_HTTPS", "N" ) << "\n";

        response = ss.str();
    }

    return std::make_pair(200, response);
}

//-----------------------------------------------------------------------------------------------------
std::string CSettingsPage::SaveSettings( void *ev_data )
{
    UpdateSetting( "server_name", "WEB_SERVER_NAME", ev_data );
    UpdateSetting( "server_port", "WEB_SERVER_PORT", ev_data );
    UpdateSetting( "down_dir", "DOWNLOAD_DIRECTORY", ev_data );
    UpdateSetting( "down_max", "DOWNLOAD_MAX_ACTIVE", ev_data );
    UpdateSetting( "proxy_addr", "PROXY_ADDRESS", ev_data );
    UpdateSetting( "proxy_port", "PROXY_PORT", ev_data );
    UpdateSetting( "proxy_activate", "PROXY_ACTIVATE", ev_data );
    UpdateSetting( "proxy_https", "PROXY_HTTPS", ev_data );

    return "ok";
}

//-----------------------------------------------------------------------------------------------------
std::string CSettingsPage::GetSettingValue( const std::string& SettingName )
{
    return HtmlEncode(   ReplaceAll( m_Settings->GetStrValue( SettingName, "" ), "'", "''" ) );
}

//-----------------------------------------------------------------------------------------------------
void CSettingsPage::UpdateSetting( const std::string& InputName, const std::string& ConfigName, void *ev_data )
{
    std::string Var = get_var(InputName.c_str(), ev_data );
    if( !Var.empty() )
    {
        m_Settings->SetStrValue( ConfigName, Var );
    }
}
