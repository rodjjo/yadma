#ifndef DOWNLOADSPAGE_H_
#define DOWNLOADSPAGE_H_

#include "../util/yadmaptr.h"
#include "../download/idownloadermanager.h"
#include "../webserver/iwebserverlistener.h"

class CDownloadsPage: public IWebServerListener
{
public:
	CDownloadsPage( yadmaptr<IDownloaderManager> DownloadManager );
	virtual ~CDownloadsPage();
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
	
private:
	static void EnumStoppedInformation( void* UserData, const TDownloadInfo* Info );
	static void EnumDownloadingInformation( void* UserData, yadmaptr<IDownload> DownloadInfo );
	static void EnumRunningInformation( void* UserData, yadmaptr<IDownload> DownloadInfo );
	
private:
	std::string RemoveAds( const std::string& Url );
	std::string GetVar( struct mg_connection* conn, const std::string& VarName );
	bool ProcessDownloadInclusion( struct mg_connection* conn );
	bool ProcessDownloadStop( struct mg_connection* conn );
	bool ProcessDownloadResume( struct mg_connection* conn );
	bool ProcessDownloadClearFinished( struct mg_connection* conn );
	bool ProcessDownloadDelete( struct mg_connection* conn );
	bool ProcessDownloadOrder( struct mg_connection* conn );
	bool ProcessRunningInfo( struct mg_connection* conn );
	
private:
	yadmaptr<IDownloaderManager> m_DownloadManager;
};

#endif /*DOWNLOADSPAGE_H_*/
