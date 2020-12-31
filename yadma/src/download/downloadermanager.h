#ifndef DOWNLOADERMANAGER_H_
#define DOWNLOADERMANAGER_H_

#include <vector>
#include "../util/yadmaptr.h"
#include "idownloadermanager.h"
#include "idownloadfactory.h"

#include "../database/idatabase.h"
#include "../settings/isettings.h"
#include "../settings/iparameterparser.h"
#include "../thread/thread.h"
#include "../thread/mutex.h"

//-----------------------------------------------------------------------------------
class CDownloaderManager: public IDownloaderManager
{
public:
    CDownloaderManager( yadmaptr<IDataBase> DataBase, yadmaptr<ISettings> Settings, yadmaptr<IParameterParser> Parameters,
                        yadmaptr<IDownloadFactory> DownloadFactory );
    virtual ~CDownloaderManager();
    bool Start();
    void Stop();
    void ClearFinished();
    std::string EnumStopped( TEnumStoppedInformation CallBack  );
    std::string EnumRunning( TEnumDownloadingInformation CallBack  );
    void AddDownload( const std::string& Url, const std::string& FileName = "", const std::string& ImageUrl = "", const std::string& Folder = "" );
    void RemoveDownload( int DownloadId );
    void StopDownload( int DownloadId );
    void ResumeDownload( int DownloadId );
    void ChangePosition( int DownloadId, bool Up );
    int GetListVersion();
    void Loop();

private:
    void RemoveDownloadFiles( const std::vector<std::pair<int,int> >& Downloads );
    bool FindDownloadFromId( int DownloadId, bool& RunningList, int& Status );
    int  GetRunningDownloads();
    void KeepRunning();
    void GetMoreInfo();
    void CheckInactiveDowns();
    void ReloadDownloadList();
    bool ResumeDownload( int DownloadId, int Status );
    void AddDownloadInfo( int DownloadId );
    void SaveDownloadOrders();


private:
    CMutex                                m_MtxDownloads;
    std::vector<yadmaptr<IDownload> >     m_RunningDownloads;
    std::vector<TDownloadInfo>            m_StoppedDownloads;
    yadmaptr<IDataBase>                 m_DataBase;
    yadmaptr<ISettings>                 m_Settings;
    yadmaptr<IParameterParser>              m_Parameters;
    yadmaptr<IDownloadFactory>            m_DownloadFactory;
    int                                    m_ListVersion;
    bool                                m_Running;
};

#endif /*DOWNLOADERMANAGER_H_*/
