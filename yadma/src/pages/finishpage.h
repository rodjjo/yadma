#ifndef FINISHPAGE_H_
#define FINISHPAGE_H_

#include "../util/yadmaptr.h"
#include "../download/idownloadermanager.h" 
#include "../webserver/iwebserverlistener.h"

class CFinishPage: public IWebServerListener
{
public:
	CFinishPage( yadmaptr<IDownloaderManager> DownloadManager );
	virtual ~CFinishPage();
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
	
private:
	yadmaptr<IDownloaderManager> m_DownloadManager;
};

#endif /*FINISHPAGE_H_*/
