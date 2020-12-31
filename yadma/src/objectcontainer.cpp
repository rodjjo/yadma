#include <stdio.h>

#include "webserver/webserver.h"
#include "settings/settings.h"
#include "thread/waitsignal.h"
#include "util/pathutil.h"
#include "database/database.h"
#include "settings/parameterparser.h"
#include "download/downloadermanager.h"
#include "download/downloadfactory.h"
#include "autenticator/autenticator.h"

#include "pages/toppage.h"
#include "pages/downloadspage.h"
#include "pages/finishpage.h"
#include "pages/settingspage.h"
#include "pages/aboutpage.h"
#include "pages/accountspages.h"
#include "pages/directoriespages.h"
#include "objectcontainer.h"


yadmaptr<IWebServer>             CObjectContainer::m_WebServer;
yadmaptr<ISettings>             CObjectContainer::m_Settings;
yadmaptr<IDownloaderManager>     CObjectContainer::m_DownloaderManager;
yadmaptr<IWaitSignal>             CObjectContainer::m_WaitFinish;
yadmaptr<IDataBase>             CObjectContainer::m_DataBase;
yadmaptr<IParameterParser>         CObjectContainer::m_ParameterParser;
yadmaptr<IDownloadFactory>       CObjectContainer::m_DownloadFactory;
yadmaptr<IAutenticator>         CObjectContainer::m_Autenticator;

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<IWebServer> CObjectContainer::GetWebServer()
{
    if( !m_WebServer )
    {
        m_WebServer = yadmaptr<IWebServer>( new CWebServer( GetSettings() ) );
        m_WebServer->AddListener( "/yadma/top.html", yadmaptr<IWebServerListener>( new CTopPage() ) );
        m_WebServer->AddListener( "/yadma/downloads.html", yadmaptr<IWebServerListener>( new CDownloadsPage( GetDownloaderManager() ) ) );
        m_WebServer->AddListener( "/yadma/settings.html", yadmaptr<IWebServerListener>( new CSettingsPage( GetSettings() ) ) );
        m_WebServer->AddListener( "/yadma/accounts.html", yadmaptr<IWebServerListener>( new CAccountsPages( GetDataBase(), GetAutenticator() ) ) );
        m_WebServer->AddListener( "/stop.html", yadmaptr<IWebServerListener>( new CFinishPage( GetDownloaderManager() ) ) );
        m_WebServer->AddListener( "/yadma/about.html", yadmaptr<IWebServerListener>( new CAboutPage() ) );
        m_WebServer->AddListener( "/yadma/dirs.html", yadmaptr<IWebServerListener>( new CDirectoriesPages( GetSettings() ) ) );
    }

    return m_WebServer;
}

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<ISettings> CObjectContainer::GetSettings()
{
    return     m_Settings ? m_Settings : ( m_Settings = yadmaptr<ISettings>( new CSettings( GetDataBase() ) ) );
}

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<IDownloaderManager> CObjectContainer::GetDownloaderManager()
{
    return  m_DownloaderManager ? m_DownloaderManager : ( m_DownloaderManager = yadmaptr<IDownloaderManager>(
                                    new CDownloaderManager( GetDataBase(), GetSettings(), GetParameterParser(), GetDownloadFactory() ) ) );
}

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<IWaitSignal> CObjectContainer::GetWaitFinish()
{
    return m_WaitFinish ? m_WaitFinish : ( m_WaitFinish = yadmaptr<IWaitSignal>( new CWaitSignal() ) );
}

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<IDataBase> CObjectContainer::GetDataBase()
{
    return m_DataBase ? m_DataBase : ( m_DataBase = yadmaptr<IDataBase>( new CDataBase( GetAppDirectory() + "../data/yadmadowns.sqlite3" ) ) );
}

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<IParameterParser> CObjectContainer::GetParameterParser()
{
    return m_ParameterParser ? m_ParameterParser : ( m_ParameterParser = yadmaptr<IParameterParser>( new CParameterParser() ) );
}

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<IDownloadFactory> CObjectContainer::GetDownloadFactory()
{
    return m_DownloadFactory ? m_DownloadFactory : ( m_DownloadFactory = yadmaptr<IDownloadFactory>( new CDownloadFactory( GetSettings(), GetDataBase(), GetAutenticator() ) ) );
}

//-----------------------------------------------------------------------------------------------------------------
yadmaptr<IAutenticator> CObjectContainer::GetAutenticator()
{
    return m_Autenticator ? m_Autenticator : ( m_Autenticator = yadmaptr<IAutenticator>( new CAutenticator( GetDataBase() ) ) );
}
