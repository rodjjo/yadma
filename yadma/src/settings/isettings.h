#ifndef ISETTINGS_H_
#define ISETTINGS_H_

#include <string>

class ISettings
{
public:
    virtual ~ISettings(){};
    virtual std::string GetStrValue( const std::string& SettingName, const std::string& DefaultValue ) = 0;
    virtual int GetIntValue( const std::string& SettingName, int DefaultValue ) = 0;
    virtual void SetStrValue( const std::string& SettingName, const std::string& Value ) = 0;
    virtual void SetIntValue( const std::string& SettingName, int Value ) = 0;
};

#endif /*ISETTINGS_H_*/
