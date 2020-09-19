#ifndef IDOWNLOADERMANAGER_H_
#define IDOWNLOADERMANAGER_H_

#include <string>
#include "../util/yadmaptr.h"
#include "idownload.h"

#define DOWNLOAD_STATUS_RUNNING			1
#define DOWNLOAD_STATUS_STOPED  		2
#define DOWNLOAD_STATUS_FINISHED		4

typedef struct
{
	int				Id;
	int 			Status;
	std::string 	Url;
	std::string     RedirectedUrl;
	std::string 	FileName;
	std::string 	PictureUrl;
	size_t 			FileSize;
	size_t			Downloaded;
}TDownloadInfo;

typedef void(*TEnumStoppedInformation)( void* UserData, const TDownloadInfo* Info );
											
typedef void(*TEnumDownloadingInformation)( void* UserData, yadmaptr<IDownload> DownloadInfo ); 

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
	virtual	void ClearFinished() = 0;
	virtual void EnumStopped( void* UserData, TEnumStoppedInformation CallBack  ) = 0;
	virtual void EnumRunning( void* UserData, TEnumDownloadingInformation CallBack  ) = 0;
	virtual	int  GetListVersion() = 0;
	virtual void Loop() = 0;
};

#endif /*IDOWNLOADERMANAGER_H_*/
