#ifndef OBJECTCONTAINER_H_
#define OBJECTCONTAINER_H_

#include "util/yadmaptr.h"
#include "webserver/iwebserver.h"
#include "settings/isettings.h"
#include "download/idownloadermanager.h"
#include "download/idownloadfactory.h"
#include "thread/iwaitsignal.h"
#include "database/idatabase.h"
#include "settings/iparameterparser.h"
#include "autenticator/iautenticator.h"

class CObjectContainer
{
public:
	static yadmaptr<IWebServer> GetWebServer();
	static yadmaptr<ISettings> GetSettings();
	static yadmaptr<IDownloaderManager> GetDownloaderManager();
	static yadmaptr<IWaitSignal> GetWaitFinish();
	static yadmaptr<IDataBase> GetDataBase();
	static yadmaptr<IParameterParser> GetParameterParser();
	static yadmaptr<IDownloadFactory> GetDownloadFactory();
	static yadmaptr<IAutenticator> GetAutenticator();
	
private:
	static yadmaptr<IWebServer> 			m_WebServer;
	static yadmaptr<ISettings> 				m_Settings;
	static yadmaptr<IDownloaderManager> 	m_DownloaderManager;
	static yadmaptr<IWaitSignal> 			m_WaitFinish;
	static yadmaptr<IDataBase> 				m_DataBase;
	static yadmaptr<IParameterParser> 		m_ParameterParser;
	static yadmaptr<IDownloadFactory>   	m_DownloadFactory;
	static yadmaptr<IAutenticator> 			m_Autenticator;
};

#endif /*OBJECTCONTAINER_H_*/
