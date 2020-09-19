#ifndef DIRECTORIESPAGES_H_
#define DIRECTORIESPAGES_H_

#include "../util/yadmaptr.h"
#include "../webserver/iwebserverlistener.h"
#include "../settings/isettings.h"

class CDirectoriesPages: public IWebServerListener
{
public:
	CDirectoriesPages( yadmaptr<ISettings> Settings );
	virtual ~CDirectoriesPages();
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
	
private:
	std::string GetVar( struct mg_connection* conn, const std::string& VarName );
	bool ProcessDirList( struct mg_connection* conn );
	void SendDirInfo( struct mg_connection* conn, const char* BaseDir, void* Entry );
	bool ProcessDirInclusion( struct mg_connection* conn );
	bool ProcessDirExclusion( struct mg_connection* conn );
	
private:
	yadmaptr<ISettings> m_Settings;
};

#endif /*DIRECTORIESPAGES_H_*/
