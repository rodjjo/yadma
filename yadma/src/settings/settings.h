#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <map>
#include <string>
#include "../util/yadmaptr.h"

#include "../database/idatabase.h"
#include "isettings.h"

class CSettings: public ISettings
{
public:
    typedef std::map<std::string,std::string> TSettingMap;
    CSettings( yadmaptr<IDataBase> DataBase );
    virtual ~CSettings();
    std::string GetStrValue( const std::string& SettingName, const std::string& DefaultValue );
    int GetIntValue( const std::string& SettingName, int DefaultValue );
    void SetStrValue( const std::string& SettingName, const std::string& Value );
    void SetIntValue( const std::string& SettingName, int Value );
    
private:
    yadmaptr<IDataBase>                     m_DataBase;
    TSettingMap                             m_Settings;
};

#endif /*SETTINGS_H_*/
