#include <stdio.h>
#include "pagedefines.h"
#include "downloadspage.h"
#include "../util/utils.h"

#define DLINFO_ID 1
#define DLINFO_STATUS 2
#define DLINFO_URL 3
#define DLINFO_REDIRECTED 4
#define DLINFO_FILENAME 5
#define DLINFO_PICTUREURL 6
#define DLINFO_FILESIZE 7
#define DLINFO_DOWNLOADED 8
#define DLINFO_SPEED 9



//-----------------------------------------------------------------------------------------------------
CDownloadsPage::CDownloadsPage( yadmaptr<IDownloaderManager> DownloadManager )
{
	m_DownloadManager = DownloadManager;
}

//-----------------------------------------------------------------------------------------------------
CDownloadsPage::~CDownloadsPage()
{
}

//-----------------------------------------------------------------------------------------------------
void CDownloadsPage::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	mg_printf( conn, "%s", PAGE_HEADER  );
	
	if ( !GetVar( conn, "version" ).empty() )
	{
		mg_printf( conn, "%d", m_DownloadManager->GetListVersion() );
		return;
	}
	
	if( ProcessDownloadInclusion( conn ) )
	{
		return;
	}		
	
	if( ProcessDownloadStop( conn ) )
	{
		return;
	}
	
	if ( ProcessDownloadResume( conn ) ) 
	{	
		return;
	}
	
	if ( ProcessDownloadClearFinished(conn ) )
	{
		return;
	}
									
	if ( ProcessDownloadOrder( conn ) )
	{
		return;
	}
	
	if( ProcessDownloadDelete( conn ) )
	{
		return;
	}
	
	if ( ProcessRunningInfo( conn ) )
	{
		return;
	} 
	
	m_DownloadManager->EnumRunning( conn, &CDownloadsPage::EnumDownloadingInformation );
	m_DownloadManager->EnumStopped( conn, &CDownloadsPage::EnumStoppedInformation );
}

//-----------------------------------------------------------------------------------------------------
bool CDownloadsPage::ProcessRunningInfo( struct mg_connection* conn )
{
	if ( GetVar( conn, "runinfo" ) == "1" )
	{
		m_DownloadManager->EnumRunning( conn, &CDownloadsPage::EnumRunningInformation );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------
void CDownloadsPage::EnumStoppedInformation( void* UserData, const TDownloadInfo* Info )
{
	struct mg_connection* conn = (struct mg_connection*) UserData;

	mg_printf( conn, "%d:%d\n", DLINFO_ID, Info->Id );
	mg_printf( conn, "%d:%d\n", DLINFO_STATUS, Info->Status );
	mg_printf( conn, "%d:%s\n", DLINFO_URL, Info->Url.c_str() );
	mg_printf( conn, "%d:%s\n", DLINFO_REDIRECTED, Info->RedirectedUrl.c_str() );
	mg_printf( conn, "%d:%s\n", DLINFO_FILENAME, Info->FileName.c_str() );
	mg_printf( conn, "%d:%s\n", DLINFO_PICTUREURL, Info->PictureUrl.c_str() );
	mg_printf( conn, "%d:%d\n", DLINFO_FILESIZE, Info->FileSize );
	mg_printf( conn, "%d:%d\n\n", DLINFO_DOWNLOADED, Info->Downloaded );
}

//-----------------------------------------------------------------------------------------------------
void CDownloadsPage::EnumDownloadingInformation( void* UserData, yadmaptr<IDownload> DownloadInfo )
{
	struct mg_connection* conn = (struct mg_connection*) UserData;
		 
	mg_printf( conn, "%d:%d\n", DLINFO_ID, DownloadInfo->GetId() );
	mg_printf( conn, "%d:%d\n", DLINFO_STATUS, ( DownloadInfo->GetStatus() == DLS_DOWNLOADING ? 0 : 1 ) );
	mg_printf( conn, "%d:%s\n", DLINFO_URL, DownloadInfo->GetOriginalUrl().c_str() );
	mg_printf( conn, "%d:%s\n", DLINFO_REDIRECTED, DownloadInfo->GetRedirectedUrl().c_str() );
	mg_printf( conn, "%d:%s\n", DLINFO_FILENAME, DownloadInfo->GetFilePath().c_str() );
	mg_printf( conn, "%d:%s\n", DLINFO_PICTUREURL, DownloadInfo->GetPictureUrl().c_str() );
	mg_printf( conn, "%d:%d\n", DLINFO_FILESIZE, DownloadInfo->GetFileSize() );
	mg_printf( conn, "%d:%d\n", DLINFO_DOWNLOADED, DownloadInfo->GetDownloaded() );
	mg_printf( conn, "%d:%d\n\n", DLINFO_SPEED, DownloadInfo->GetDownloadSpeed() );
}

//-----------------------------------------------------------------------------------------------------
void CDownloadsPage::EnumRunningInformation( void* UserData, yadmaptr<IDownload> DownloadInfo )
{
	struct mg_connection* conn = (struct mg_connection*) UserData;
	mg_printf( conn, "%d:%d\n", DLINFO_ID, DownloadInfo->GetId() );
	mg_printf( conn, "%d:%d\n", DLINFO_FILESIZE, DownloadInfo->GetFileSize() );
	mg_printf( conn, "%d:%d\n", DLINFO_DOWNLOADED, DownloadInfo->GetDownloaded() );
	mg_printf( conn, "%d:%d\n\n", DLINFO_SPEED, DownloadInfo->GetDownloadSpeed() );
}

//-----------------------------------------------------------------------------------------------------
std::string  CDownloadsPage::GetVar( struct mg_connection* conn, const std::string& VarName )
{
	char* Var = mg_get_var( conn, VarName.c_str() );
	
	if ( Var )
	{
		std::string RetVal( Var );
		mg_free( Var );
		return RetVal;		
	} 
	
	return "";
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::RemoveAds( const std::string& Url )
{
	size_t p = 0;
	
	// http://propraganda.bla/?link=http://www.megaupload.com
	if ( ( p = Url.find(  "http://", 3 ) ) != std::string::npos )
	{
		return Url.substr( p );
	}	
	
	// http://propraganda.bla/?link=http://http%3A%2F%2Fwww%3Amegaupload%3Acom
	if (  ( p = Url.find(  "http%3A%2F%2F", 3 ) ) != std::string::npos  )
	{
		return UrlDecode( Url.substr( p ) );
	}
	
	// base 64
	// http://propraganda.bla/?link=xpptec3vad2as6trer23ew2rew
	if (  ( p = Url.find(  "aHR0cDov", 3 ) ) != std::string::npos  )
	{
		return Base64Decode( Url.substr( p ) );
	}
	
	// Inverted String
	// http://propaganda.com.br/?value=GFEDCBA=d?/moc.daolpuagem.www//:ptth
	if (  Url.find(  "//:ptth", 3 ) != std::string::npos  )
	{
		std::string Reverted;
		
		Reverted.reserve( Url.length() );
		
		for ( size_t i = Url.length(); i > 0; i-- )
		{
			Reverted.push_back( Url[ i - 1 ]  );  
		}

		if ( ( p = Reverted.rfind( "=" ) ) != std::string::npos );
		{
			return Reverted.substr( 0, p - 1 );
		}
		
		return Reverted;
	}
	
	return Url;
}

//-----------------------------------------------------------------------------------------------------
bool CDownloadsPage::ProcessDownloadInclusion( struct mg_connection* conn )
{
	std::string AddLink = GetVar( conn, "links" ); 
	 
	if( !AddLink.empty() )
	{
		size_t p = AddLink.find_first_of( "\n\r" );
		
		while( p != std::string::npos )
		{
			std::string Part = AddLink.substr( 0, p );
			
			if ( !Part.empty() )
			{
				m_DownloadManager->AddDownload( RemoveAds( Part ), "", GetVar( conn, "site" ), GetVar( conn, "folder" ) );
			}
			
			AddLink = AddLink.substr( p + 1 );
			
			p = AddLink.find_first_of( "\n\r" );
		}
		
		if ( !AddLink.empty() )
		{
			m_DownloadManager->AddDownload( RemoveAds( AddLink ), "", GetVar( conn, "site" ), GetVar( conn, "folder" ) );
		}
		
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------------------------------
bool CDownloadsPage::ProcessDownloadOrder( struct mg_connection* conn )
{
	int DownloadId = StrToInt( GetVar( conn, "order_up" ) );
	
	bool Up = DownloadId > 0;
	
	if ( !Up )
	{
		DownloadId = StrToInt( GetVar( conn, "order_down" ) );
	}
	
	if ( DownloadId > 0 )
	{
		m_DownloadManager->ChangePosition( DownloadId, Up );
		
		return true;
	} 
	
	return false;
}

//-----------------------------------------------------------------------------------------------------
bool CDownloadsPage::ProcessDownloadStop( struct mg_connection* conn )
{
	int StopId = StrToInt( GetVar( conn, "stop" ) );
	 
	if( StopId > 0 )
	{
		m_DownloadManager->StopDownload( StopId );
		return true;
	}
	
	return false;
}
//-----------------------------------------------------------------------------------------------------
bool CDownloadsPage::ProcessDownloadResume( struct mg_connection* conn )
{
	int ResumeId = StrToInt( GetVar( conn, "resume" ) );
	 
	if( ResumeId > 0 )
	{
		m_DownloadManager->ResumeDownload( ResumeId );
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------------------------------
bool CDownloadsPage::ProcessDownloadClearFinished( struct mg_connection* conn )
{
	int finishedid = StrToInt( GetVar( conn, "clear" ) );
	
	if( finishedid != 0 )
	{
		if ( finishedid == -1 )
		{
			m_DownloadManager->ClearFinished();
		}
		else
		{
			m_DownloadManager->RemoveDownload( finishedid );
		}
		return true;
	}
	
	return false;
}

//-----------------------------------------------------------------------------------------------------
bool CDownloadsPage::ProcessDownloadDelete( struct mg_connection* conn )
{
	int DeleteId = StrToInt( GetVar( conn, "delete" ) );
	 
	if( DeleteId > 0 )
	{
		m_DownloadManager->RemoveDownload( DeleteId );
		
		return true;
	}
	
	return false;
}


