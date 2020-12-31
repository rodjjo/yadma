#ifndef DOWNLOADSPAGE_H_
#define DOWNLOADSPAGE_H_

#include <string>
#include <utility>
#include "../util/yadmaptr.h"
#include "../download/idownloadermanager.h"
#include "../webserver/iwebserverlistener.h"

class CDownloadsPage: public IWebServerListener
{
public:
    CDownloadsPage( yadmaptr<IDownloaderManager> DownloadManager );
    virtual ~CDownloadsPage();
    std::pair<int, std::string> HandleRequest( void *ev_data );

private:
    static std::string EnumStoppedInformation( const TDownloadInfo* Info );
    static std::string EnumDownloadingInformation( yadmaptr<IDownload> DownloadInfo );
    static std::string EnumRunningInformation( yadmaptr<IDownload> DownloadInfo );

private:
    std::string RemoveAds( const std::string& Url );
    std::string ProcessDownloadInclusion( void *ev_data );
    std::string ProcessDownloadStop( void *ev_data );
    std::string ProcessDownloadResume( void *ev_data );
    std::string ProcessDownloadClearFinished( void *ev_data );
    std::string ProcessDownloadDelete( void *ev_data );
    std::string ProcessDownloadOrder( void *ev_data );
    std::string ProcessRunningInfo( void *ev_data );

private:
    yadmaptr<IDownloaderManager> m_DownloadManager;
};

#endif /*DOWNLOADSPAGE_H_*/
