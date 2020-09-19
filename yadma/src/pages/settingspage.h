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
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
	
private:
	void UpdateSetting( const std::string& InputName, const std::string& ConfigName, struct mg_connection* conn );
	std::string GetSettingValue( const std::string& SettingName );
	bool SaveSettings(struct mg_connection* conn );
	
private:
	yadmaptr<ISettings> m_Settings;
};


#endif /*SETTINGSPAGE_H_*/
