#ifndef IDOWNLOADERMANAGER_H_
#define IDOWNLOADERMANAGER_H_

#include <string>
#include "../util/yadmaptr.h"
#include "idownload.h"

#define DOWNLOAD_STATUS_RUNNING            1
#define DOWNLOAD_STATUS_STOPED          2
#define DOWNLOAD_STATUS_FINISHED        4

typedef struct
{
    int                Id;
    int             Status;
    std::string     Url;
    std::string     RedirectedUrl;
    std::string     FileName;
    std::string     PictureUrl;
    int64_t             FileSize;
    int64_t            Downloaded;
}TDownloadInfo;

typedef std::string (*TEnumStoppedInformation)( const TDownloadInfo* Info );

typedef std::string (*TEnumDownloadingInformation)( yadmaptr<IDownload> DownloadInfo );

//-----------------------------------------------------------------------
class IDownloaderManager
{
public:
    virtual ~IDownloaderManager(){};
    virtual bool Start() = 0;
    virtual void Stop() = 0;
    virtual void AddDownload( const std::string& Url, const std::string& FileName = "",  const std::string& ImageUrl = "", const std::string& Folder = "" ) = 0;
    virtual void RemoveDownload( int DownloadId ) = 0;
    virtual void StopDownload( int DownloadId ) = 0;
    virtual void ResumeDownload( int DownloadId ) = 0;
    virtual void ChangePosition( int DownloadId, bool Up ) = 0;
    virtual    void ClearFinished() = 0;
    virtual std::string EnumStopped( TEnumStoppedInformation CallBack  ) = 0;
    virtual std::string EnumRunning( TEnumDownloadingInformation CallBack  ) = 0;
    virtual    int  GetListVersion() = 0;
    virtual void Loop() = 0;
};

#endif /*IDOWNLOADERMANAGER_H_*/
