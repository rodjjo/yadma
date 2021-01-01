#ifdef _DEBUG
#include <stdio.h>
#endif

#include "../util/pathutil.h"
#include "../util/utils.h"

#include "settings.h"

//----------------------------------------------------------------------------------------------------
CSettings::CSettings( yadmaptr<IDataBase> DataBase )
{
    m_DataBase = DataBase;

    if( m_DataBase->IsOpen() )
    {
        yadmaptr<IDBQuery> Query = m_DataBase->Query( "select * from TB_SETTINGS ");

        if( !Query->Next() ) {
            m_DataBase->Execute(
                "INSERT INTO TB_SETTINGS (WEB_SERVER_NAME, WEB_SERVER_PORT, DOWNLOAD_DIRECTORY, PROXY_ADDRESS, PROXY_PORT, PROXY_ACTIVATE, PROXY_HTTPS)" \
                "VALUES ('0.0.0.0', '8080', '', '', '', 'n', 'n')"
            );
        }

        Query = m_DataBase->Query( "select * from TB_SETTINGS ");
        if( Query->Next() )
        {
            for( size_t i = 0; i < Query->ColCount(); i++ )
            {
                m_Settings[ Query->GetColName( i ) ] = Query->AsString( i );
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------
CSettings::~CSettings()
{
}

//----------------------------------------------------------------------------------------------------
std::string CSettings::GetStrValue( const std::string& SettingName, const std::string& DefaultValue )
{
    TSettingMap::iterator it = m_Settings.find( SettingName );

    if( it != m_Settings.end() )
    {
        return it->second;
    }

    return DefaultValue;
}

//----------------------------------------------------------------------------------------------------
int CSettings::GetIntValue( const std::string& SettingName, int DefaultValue )
{
    bool Error = false;

    int RetVal = StrToInt( GetStrValue( SettingName, "" ), &Error );

    if( Error )
    {
        RetVal = DefaultValue;
    }

    return RetVal;
}

//----------------------------------------------------------------------------------------------------
void CSettings::SetStrValue( const std::string& SettingName, const std::string& Value )
{
    TSettingMap::iterator it = m_Settings.find( SettingName );

    if( it != m_Settings.end() )
    {
        it->second = Value;
        std::vector<std::string> Params;
        Params.push_back( Value );
        m_DataBase->Execute( "UPDATE TB_SETTINGS SET " + SettingName + " = ? ", Params );
    }
}

//----------------------------------------------------------------------------------------------------
void CSettings::SetIntValue( const std::string& SettingName, int Value )
{
    SetStrValue( SettingName, IntToStr( Value ) );
}
