#ifndef DOWNLOAD_H_
#define DOWNLOAD_H_

#include <curl/curl.h>
#include "../util/yadmaptr.h"
#include "idownload.h"
#include "../autenticator/iautenticator.h"
#include "../settings/isettings.h"
#include "../database/idatabase.h"
#include "../thread/thread.h"
#include "../thread/mutex.h"

class CDownload: public IDownload, public CThread
{
public:
	CDownload( int DownloadId, yadmaptr<ISettings> Settings, yadmaptr<IDataBase> DataBase, yadmaptr<IAutenticator> Autenticator );
	virtual ~CDownload();
	int GetId();
	std::string GetOriginalUrl();
	std::string GetRedirectedUrl();
	std::string GetFilePath();
	std::string GetPictureUrl();
	std::string GetFolder();
	int GetFileSize();
	int GetDownloaded();
	int GetDownloadSpeed();
	int GetStatus();	
	void KeepRunning();
	bool InitializeInfo();
	
private:
	void IncrementDownloaded( int Bytes );
	static size_t WriteData( void *Buffer, size_t ElementSize, size_t ElementCount, void *UserData );
	bool PrepareCurl();
	bool GetDownloadInformation();
	void Dispose();
	void LoadInfo();
	void SaveInfo();
	
protected:
	void Run();
	
private:
	bool					m_InfoRetrived;
	bool					m_InfoNeeded;
	bool					m_StatusVerified;
    CURL*                   m_Curl;
	FILE*					m_File;
	int 					m_Id;
	int 					m_Status;
	std::string   			m_Prefix;
	std::string				m_FilePath;
	std::string				m_OriginalUrl;
	std::string				m_RedirectedUrl;
	std::string				m_PictureUrl;
	std::string				m_Cookie;
	std::string 			m_Folder;
	size_t					m_FileSize;
	size_t					m_Downloaded;
	yadmaptr<ISettings> 	m_Settings;
	yadmaptr<IDataBase>  	m_DataBase;
	yadmaptr<IAutenticator> m_Autenticator;
	CMutex					m_mtxSpeed;
	int						m_Errors;
};
	

#endif /*DOWNLOAD_H_*/
