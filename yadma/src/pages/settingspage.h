#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include "../util/yadmaptr.h"
#include "../settings/isettings.h"
#include "../webserver/iwebserverlistener.h"

class CSettingsPage: public IWebServerListener
{
public:
    CSettingsPage( yadmaptr<ISettings> Settings );
    virtual ~CSettingsPage();
    std::pair<int, std::string> HandleRequest( void *ev_data );;

private:
    void UpdateSetting( const std::string& InputName, const std::string& ConfigName, void *ev_data );
    std::string GetSettingValue( const std::string& SettingName );
    std::string SaveSettings( void *ev_data );

private:
    yadmaptr<ISettings> m_Settings;
};


#endif /*SETTINGSPAGE_H_*/
