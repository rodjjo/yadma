#include <stdio.h>
#include <fstream>
#include <unistd.h>
#include "../util/utils.h"
#include "downloadermanager.h"

//-------------------------------------------------------------------------------
CDownloaderManager::CDownloaderManager( yadmaptr<IDataBase> DataBase, yadmaptr<ISettings> Settings, yadmaptr<IParameterParser> Parameters,
						yadmaptr<IDownloadFactory> DownloadFactory )
{
	m_DataBase 			= DataBase;
	m_Settings 			= Settings; 
	m_Parameters 		= Parameters;
	m_DownloadFactory 	= DownloadFactory;
	m_ListVersion		= 0;
	m_Running 			= false;
}

//-------------------------------------------------------------------------------
CDownloaderManager::~CDownloaderManager()
{
	Stop();
}

//-------------------------------------------------------------------------------
bool CDownloaderManager::ResumeDownload( int DownloadId, int Status )
{
	if( Status == DOWNLOAD_STATUS_RUNNING )
	{
		for( size_t i = 0; i < m_RunningDownloads.size(); i++ )
		{
			if( m_RunningDownloads[i]->GetId() == DownloadId )
			{
				return true;
			}
		}
		
		m_RunningDownloads.push_back( m_DownloadFactory->CreateDownload( DownloadId ) );
		
		return true;
	}
	
	return false;	
}

//-------------------------------------------------------------------------------
int CDownloaderManager::GetListVersion()
{
	return m_ListVersion;	
}

//-------------------------------------------------------------------------------
void CDownloaderManager::AddDownloadInfo( int DownloadId )
{
	yadmaptr<IDBQuery> Query = m_DataBase->Query( "SELECT * FROM TB_DOWNLOADS WHERE ID_DOWNLOAD = " + IntToStr( DownloadId ) );
	
	if( Query->Next() )
	{
		TDownloadInfo 		Info;
		Info.Id 			= DownloadId;
		Info.Downloaded		= Query->AsInteger( "DOWNLOADED" );
		Info.FileName		= Query->AsString( "FILENAME" );
		Info.FileSize		= Query->AsInteger( "FILESIZE" );
		Info.Status			= Query->AsInteger( "STATUS" );
		Info.Url			= Query->AsString( "URL" );
		Info.RedirectedUrl	= Query->AsString( "REDIRECTEDURL" );
		Info.PictureUrl 	= Query->AsString( "PICTURE_URL" );
		
		m_StoppedDownloads.push_back( Info );
		
		m_ListVersion++;
	}	
}

//-------------------------------------------------------------------------------
void CDownloaderManager::RemoveDownloadFiles( const std::vector<std::pair<int,int> >& Downloads )
{
	bool Found = false;
	
	for( size_t i = 0; i < m_StoppedDownloads.size(); i++ )
	{
		Found = false;
	
		for( size_t i2 = 0; i2 < Downloads.size(); i2++ )
		{
			if( Downloads[i2].first == m_StoppedDownloads[i].Id )
			{
				Found = true;
				break;
			}		
		}
		
		if( (!Found) && ( m_StoppedDownloads[i].Status != DOWNLOAD_STATUS_FINISHED ) )
		{
			std::remove( m_StoppedDownloads[i].FileName.c_str() );
		}
	}
	
	for( size_t i = m_RunningDownloads.size(); i > 0; i-- )
	{
		Found = false;
	
		for( size_t i2 = 0; i2 < Downloads.size(); i2++ )
		{
			if( Downloads[i2].first == m_RunningDownloads[i-1]->GetId() )
			{
				Found = true;
				break;
			}		
		}
		
		if( !Found )
		{
			std::string FileName = m_RunningDownloads[i-1]->GetFilePath(); 
			
			m_RunningDownloads.erase( m_RunningDownloads.begin() + (i-1) );
			
			std::remove( FileName.c_str() );
			
			m_ListVersion++;
		}
	}
}

//-------------------------------------------------------------------------------
void  CDownloaderManager::ReloadDownloadList()
{
	yadmaptr<IDBQuery> Query = m_DataBase->Query( "SELECT ID_DOWNLOAD, STATUS FROM TB_DOWNLOADS ORDER BY \"ORDER\", ID_DOWNLOAD" );
	
	std::vector<std::pair<int,int> > QueryValues;
	
	while( Query->Next() )
	{
		QueryValues.push_back( std::pair<int,int>( Query->AsInteger( "ID_DOWNLOAD" ), Query->AsInteger( "STATUS" ) ) );	
	}	
	
	RemoveDownloadFiles( QueryValues );
					
	m_StoppedDownloads.clear();
	
	for( size_t i = 0; i < QueryValues.size(); i++  )
	{
		if( !ResumeDownload( QueryValues[i].first, QueryValues[i].second ) )
		{
			AddDownloadInfo( QueryValues[i].first );			
		}
	}
	
	m_ListVersion++;
}

//-------------------------------------------------------------------------------
bool CDownloaderManager::Start()
{
	m_Running = true;
	return true;
}

//-------------------------------------------------------------------------------
void CDownloaderManager::Stop()
{
	m_Running = false;
}

//-------------------------------------------------------------------------------
void CDownloaderManager::ClearFinished()
{
	CMutexLocker Locker( m_MtxDownloads );
	
	m_DataBase->Execute( "DELETE FROM TB_DOWNLOADS WHERE STATUS = " + IntToStr( DOWNLOAD_STATUS_FINISHED ) );
	
	ReloadDownloadList();
}

//-------------------------------------------------------------------------------
void CDownloaderManager::Loop()
{
	ReloadDownloadList();

	while( m_Running )
	{
		usleep( 1000000 ); 
		
		CMutexLocker Locker( m_MtxDownloads );
	
		KeepRunning();
	
		CheckInactiveDowns();
	}
}

//-------------------------------------------------------------------------------
void CDownloaderManager::AddDownload( const std::string& Url, const std::string& FileName, const std::string& ImageUrl, const std::string& Folder  )
{
	CMutexLocker Locker( m_MtxDownloads );
	
	std::vector<std::string> Params;
	
	Params.push_back( Url );
	Params.push_back( FileName );
	Params.push_back( ImageUrl );
	Params.push_back( Folder );
	Params.push_back( IntToStr( DOWNLOAD_STATUS_RUNNING ) );

	m_DataBase->Execute( "insert into TB_DOWNLOADS(URL,FILENAME,PICTURE_URL,FOLDER,STATUS)values(?,?,?,?,?)", Params );
	
	ReloadDownloadList();
}

//-------------------------------------------------------------------------------
bool CDownloaderManager::FindDownloadFromId( int DownloadId, bool& RunningList, int& Status )
{
	RunningList = false;

	for ( size_t i = 0; i < m_StoppedDownloads.size(); i++ )
	{
		if( m_StoppedDownloads[i].Id == DownloadId )
		{
			Status = m_StoppedDownloads[i].Status;
			return true;
		}
	}
	
	for ( size_t i = 0; i < m_RunningDownloads.size(); i++ )
	{
		if( m_RunningDownloads[i]->GetId() == DownloadId )
		{
			Status = DOWNLOAD_STATUS_RUNNING;
			RunningList = true;
			return true;
		}
	}	
	
	return false;
}

//-------------------------------------------------------------------------------
void CDownloaderManager::RemoveDownload( int DownloadId )
{
	CMutexLocker Locker( m_MtxDownloads );

	bool Running 	= false;
	int  Status		= DOWNLOAD_STATUS_FINISHED;
	
	if( FindDownloadFromId( DownloadId, Running, Status ) )
	{
		m_DataBase->Execute( "DELETE FROM TB_DOWNLOADS WHERE ID_DOWNLOAD = " + IntToStr( DownloadId ) );
		
		ReloadDownloadList();
	}
}

//-------------------------------------------------------------------------------
void CDownloaderManager::StopDownload( int DownloadId )
{
	CMutexLocker Locker( m_MtxDownloads );
	
	for( size_t i = 0; i < m_RunningDownloads.size(); i++ )
	{
		if( m_RunningDownloads[i]->GetId() == DownloadId )
		{
			m_DataBase->Execute( "update TB_DOWNLOADS SET STATUS = 2 WHERE ID_DOWNLOAD = " + IntToStr( DownloadId ) );
			
			m_RunningDownloads.erase( m_RunningDownloads.begin() + i );
			
			ReloadDownloadList();
			
			return;
		}
	}
}

//-------------------------------------------------------------------------------
void CDownloaderManager::ChangePosition( int DownloadId, bool Up )
{
	CMutexLocker Locker( m_MtxDownloads );
	
	for( size_t i = 0; i < m_RunningDownloads.size(); i++ )
	{
		if( m_RunningDownloads[i]->GetId() == DownloadId )
		{
			if ( ( Up && ( i == 0 ) ) || ( ( i + 1 == m_RunningDownloads.size() ) && !Up ))
			{
				return;
			}		
			
			yadmaptr<IDownload> temp = 	m_RunningDownloads[ (int)i + (Up ? -1 : 1) ];
			m_RunningDownloads[ (int)i + (Up ? -1 : 1) ] = m_RunningDownloads[ i ];
			m_RunningDownloads[ i ] = temp;
			
			SaveDownloadOrders();
						
			return;
		}	
	}
}

//-------------------------------------------------------------------------------
void CDownloaderManager::SaveDownloadOrders()
{
	int Order = 0;
	for( size_t i = 0; i < m_RunningDownloads.size(); i++ )
	{
		m_DataBase->Execute( "update TB_DOWNLOADS SET \"ORDER\" = " + IntToStr( Order ) + " WHERE ID_DOWNLOAD = " + IntToStr( m_RunningDownloads[i]->GetId() ) );
		Order++;
	}
	 
	for ( size_t i = 0; i < m_StoppedDownloads.size(); i++ )
	{
		m_DataBase->Execute( "update TB_DOWNLOADS SET \"ORDER\" = " + IntToStr( Order ) + " WHERE ID_DOWNLOAD = " + IntToStr( m_StoppedDownloads[i].Id ) );
		Order++;
	}
}
 
//-------------------------------------------------------------------------------
void CDownloaderManager::ResumeDownload( int DownloadId )
{
	CMutexLocker Locker( m_MtxDownloads );

	bool Running 	= false;
	int  Status		= DOWNLOAD_STATUS_FINISHED;
	
	if( FindDownloadFromId( DownloadId, Running, Status ) )
	{
		if( ( Running == false ) && ( Status != DOWNLOAD_STATUS_FINISHED ) )
		{
			m_DataBase->Execute( "update TB_DOWNLOADS SET STATUS = 1 WHERE ID_DOWNLOAD = " + IntToStr( DownloadId ) );
			
			ReloadDownloadList();
		}
	}
}

//-------------------------------------------------------------------------------
void CDownloaderManager::EnumStopped( void* UserData, TEnumStoppedInformation CallBack  )
{
	CMutexLocker Locker( m_MtxDownloads );
	
	for( size_t i = 0; i < m_StoppedDownloads.size(); i++ )
	{
		CallBack( UserData, &m_StoppedDownloads[i] );
	}
}

//-------------------------------------------------------------------------------
void CDownloaderManager::EnumRunning( void* UserData, TEnumDownloadingInformation CallBack  )
{
	CMutexLocker Locker( m_MtxDownloads );
	
	for( size_t i = 0; i < m_RunningDownloads.size(); i++ )
	{
		CallBack( UserData, m_RunningDownloads[i] );
	}
}

//-------------------------------------------------------------------------------
int  CDownloaderManager::GetRunningDownloads()
{
	int RunningDownloads = 0;
	
	for( size_t i = 0; i < m_RunningDownloads.size(); i++ )
	{
		if( m_RunningDownloads[i]->GetStatus() == DLS_DOWNLOADING )
		{
			RunningDownloads++;
		}	
	}
	
	return RunningDownloads;
}

//-------------------------------------------------------------------------------
void  CDownloaderManager::KeepRunning()
{
	int RunningDownloads = GetRunningDownloads();
	
	int MaxDowns = m_Settings->GetIntValue( "DOWNLOAD_MAX_ACTIVE", 1 );
	
	if ( MaxDowns < 1 )
	{
		MaxDowns =  1;
	}
	
	int Status = 0;
	
	bool CanGetMoreInfo = true;
	
	for( size_t i = 0; i < m_RunningDownloads.size(); i++ )
	{
		Status = m_RunningDownloads[i]->GetStatus();
		
		if( Status  == DLS_PENDING )
		{
			if( MaxDowns > RunningDownloads )
			{
				RunningDownloads++;
				m_RunningDownloads[i]->KeepRunning();
				CanGetMoreInfo = false;
			}
		}else if ( Status == DLS_GETTING_INFO )
		{
			CanGetMoreInfo = false;
		}	
	}
	
	if ( CanGetMoreInfo )
	{
		GetMoreInfo();
	}
}

//-------------------------------------------------------------------------------
void CDownloaderManager::GetMoreInfo()
{
	for( size_t i = 0; i < m_RunningDownloads.size();  i++ )
	{
		if( m_RunningDownloads[i]->InitializeInfo() )
		{
			return;
		}
	}	
}

//-------------------------------------------------------------------------------
void CDownloaderManager::CheckInactiveDowns()
{
	for( size_t i = m_RunningDownloads.size(); i > 0; i-- )
	{
		if( m_RunningDownloads[ i - 1 ]->GetStatus() == DLS_FINISHED ||  
			m_RunningDownloads[ i - 1 ]->GetStatus() == DLS_BAD )
		{
			int Status = ( m_RunningDownloads[ i - 1 ]->GetStatus() == DLS_FINISHED ) ? DOWNLOAD_STATUS_FINISHED : DOWNLOAD_STATUS_STOPED;
			 
			m_DataBase->Execute( "UPDATE TB_DOWNLOADS SET STATUS=" + IntToStr( Status ) + 
									 " WHERE ID_DOWNLOAD = " + IntToStr( m_RunningDownloads[ i - 1 ]->GetId()  ));
									 
			TDownloadInfo Info;
			Info.Downloaded 	= m_RunningDownloads[ i - 1 ]->GetDownloaded();
			Info.FileSize 		= m_RunningDownloads[ i - 1 ]->GetFileSize();
			Info.FileName 		= m_RunningDownloads[ i - 1 ]->GetFilePath();
			Info.Id 			= m_RunningDownloads[ i - 1 ]->GetId();
			Info.PictureUrl 	= m_RunningDownloads[ i - 1 ]->GetPictureUrl();
			Info.RedirectedUrl  = m_RunningDownloads[ i - 1 ]->GetRedirectedUrl();
			Info.Status 		= ( m_RunningDownloads[ i - 1 ]->GetStatus() == DLS_FINISHED ? DOWNLOAD_STATUS_FINISHED : DOWNLOAD_STATUS_STOPED );
			Info.Url			= m_RunningDownloads[ i - 1 ]->GetOriginalUrl();
			
			m_StoppedDownloads.push_back( Info );
			m_RunningDownloads.erase( m_RunningDownloads.begin() + ( i - 1) );
			
			m_ListVersion++;
		}
	}
}


