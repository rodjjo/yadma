#include <stdio.h>
#include <sstream>
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
std::pair<int, std::string> CDownloadsPage::HandleRequest( void *ev_data )
{
     if ( !get_var( "version", ev_data ).empty() )
    {
        char data[32] = { 0, };
        sprintf(data, "%d", m_DownloadManager->GetListVersion());
        return std::make_pair(200, data);
    }

     std::string response = ProcessDownloadInclusion( ev_data ) ;

    if ( response.empty() )
    {
        response = ProcessDownloadStop( ev_data );
    }

    if ( response.empty() )
    {
        response = ProcessDownloadResume( ev_data );
    }

    if ( response.empty() )
    {
        response =ProcessDownloadClearFinished( ev_data );
    }

    if ( response.empty() )
    {
        response = ProcessDownloadOrder( ev_data );
    }

    if ( response.empty() )
    {
        response = ProcessDownloadDelete( ev_data );
    }

    if ( response.empty() )
    {
        response = ProcessRunningInfo( ev_data );
    }

    if (response.empty()) {
        response = m_DownloadManager->EnumRunning( &CDownloadsPage::EnumDownloadingInformation );
    }

    if (response.empty()) {
        response = m_DownloadManager->EnumStopped( &CDownloadsPage::EnumStoppedInformation );
    }

    return std::make_pair(200, response);
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::ProcessRunningInfo( void *ev_data )
{
    if ( get_var( "runinfo", ev_data ) == "1" )
    {
        return m_DownloadManager->EnumRunning( &CDownloadsPage::EnumRunningInformation );
    }
    return std::string();
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::EnumStoppedInformation( const TDownloadInfo* Info )
{
    std::stringstream ss;


    ss << DLINFO_ID << ":" << Info->Id << "\n";
    ss << DLINFO_STATUS << ":" << Info->Status << "\n";
    ss << DLINFO_URL << ":" << Info->Url << "\n";
    ss << DLINFO_REDIRECTED << ":" << Info->RedirectedUrl << "\n";
    ss << DLINFO_FILENAME << ":" << Info->FileName << "\n";
    ss << DLINFO_PICTUREURL << ":" << Info->PictureUrl << "\n";
    ss << DLINFO_FILESIZE << ":" << IntToStr(Info->FileSize) << "\n";
    ss << DLINFO_DOWNLOADED << ":" << IntToStr(Info->Downloaded) << "\n\n";

    return ss.str();
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::EnumDownloadingInformation( yadmaptr<IDownload> DownloadInfo )
{
    std::stringstream ss;

    ss << DLINFO_ID << ":" << DownloadInfo->GetId() << "\n";
    ss << DLINFO_STATUS << ":" << ( DownloadInfo->GetStatus() == DLS_DOWNLOADING ? 0 : 1 ) << "\n";
    ss << DLINFO_URL << ":" << DownloadInfo->GetOriginalUrl() << "\n";
    ss << DLINFO_REDIRECTED << ":" << DownloadInfo->GetRedirectedUrl() << "\n";
    ss << DLINFO_FILENAME << ":" << DownloadInfo->GetFilePath() << "\n";
    ss << DLINFO_PICTUREURL << ":" << DownloadInfo->GetPictureUrl() << "\n";
    ss << DLINFO_FILESIZE << ":" << IntToStr(DownloadInfo->GetFileSize()) << "\n";
    ss << DLINFO_DOWNLOADED << ":" << IntToStr(DownloadInfo->GetDownloaded()) << "\n";
    ss << DLINFO_SPEED << ":" << IntToStr(DownloadInfo->GetDownloadSpeed()) << "\n\n";

    return ss.str();
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::EnumRunningInformation( yadmaptr<IDownload> DownloadInfo )
{
    std::stringstream ss;

    ss << DLINFO_ID << ":" << DownloadInfo->GetId() << "\n";
    ss << DLINFO_FILESIZE << ":" << IntToStr(DownloadInfo->GetFileSize()) << "\n";
    ss << DLINFO_DOWNLOADED << ":" << IntToStr(DownloadInfo->GetDownloaded()) << "\n";
    ss << DLINFO_SPEED << ":" << IntToStr(DownloadInfo->GetDownloadSpeed()) << "\n\n";

    return ss.str();
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
std::string CDownloadsPage::ProcessDownloadInclusion( void *ev_data )
{
    std::string AddLink = get_var( "links", ev_data );

    if( !AddLink.empty() )
    {
        size_t p = AddLink.find_first_of( "\n\r" );

        while( p != std::string::npos )
        {
            std::string Part = AddLink.substr( 0, p );

            if ( !Part.empty() )
            {
                m_DownloadManager->AddDownload( RemoveAds( Part ), "", get_var( "site", ev_data ), get_var( "folder", ev_data ) );
            }

            AddLink = AddLink.substr( p + 1 );

            p = AddLink.find_first_of( "\n\r" );
        }

        if ( !AddLink.empty() )
        {
            m_DownloadManager->AddDownload( RemoveAds( AddLink ), "", get_var( "site", ev_data ), get_var( "folder", ev_data ) );
        }

        return "ok";
    }

    return std::string();
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::ProcessDownloadOrder( void *ev_data )
{
    int DownloadId = StrToInt( get_var( "order_up", ev_data ) );

    bool Up = DownloadId > 0;

    if ( !Up )
    {
        DownloadId = StrToInt( get_var( "order_down", ev_data ) );
    }

    if ( DownloadId > 0 )
    {
        m_DownloadManager->ChangePosition( DownloadId, Up );

        return "ok";
    }

    return std::string();
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::ProcessDownloadStop( void *ev_data )
{
    int StopId = StrToInt( get_var( "stop", ev_data ) );

    if( StopId > 0 )
    {
        m_DownloadManager->StopDownload( StopId );
        return "ok";
    }

    return std::string();
}
//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::ProcessDownloadResume( void *ev_data )
{
    int ResumeId = StrToInt( get_var( "resume", ev_data ) );

    if( ResumeId > 0 )
    {
        m_DownloadManager->ResumeDownload( ResumeId );
        return "ok";
    }

    return std::string();
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::ProcessDownloadClearFinished( void *ev_data )
{
    int finishedid = StrToInt( get_var( "clear", ev_data ) );

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
        return "ok";
    }

    return std::string();
}

//-----------------------------------------------------------------------------------------------------
std::string CDownloadsPage::ProcessDownloadDelete( void *ev_data )
{
    int DeleteId = StrToInt( get_var( "delete", ev_data ) );

    if( DeleteId > 0 )
    {
        m_DownloadManager->RemoveDownload( DeleteId );

        return "ok";
    }

    return std::string();
}
