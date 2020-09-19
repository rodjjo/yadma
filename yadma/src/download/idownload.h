#ifndef IDOWNLOAD_H_
#define IDOWNLOAD_H_

#include <string>

typedef enum {
		DLS_DOWNLOADING,
		DLS_FINISHED,
		DLS_PENDING,
		DLS_GETTING_INFO,
		DLS_ERROR,
		DLS_BAD
	}TDownloadStatus;
	
//-----------------------------------------------------------------------
class IDownload
{
public:
	virtual ~IDownload(){};
	virtual int GetId() = 0;
	virtual bool InitializeInfo() = 0;
	virtual std::string GetOriginalUrl() = 0;
	virtual std::string GetRedirectedUrl() = 0;
	virtual std::string GetFilePath() = 0;
	virtual std::string GetPictureUrl() = 0;
	virtual std::string GetFolder() = 0;
	virtual int 		GetFileSize() = 0;
	virtual int			GetDownloaded() = 0;
	virtual int			GetDownloadSpeed() = 0;
	virtual int			GetStatus() = 0;	
	virtual void		KeepRunning() = 0;
};

#endif /*IDOWNLOAD_H_*/
