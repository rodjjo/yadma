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
    std::pair<int, std::string> HandleRequest( void *ev_data );
    
private:
    yadmaptr<IDownloaderManager> m_DownloadManager;
};

#endif /*FINISHPAGE_H_*/
