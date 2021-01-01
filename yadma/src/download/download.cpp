#include <unistd.h>
#include <stdio.h>
#include <vector>

#include "../thread/mutex.h"
#include "getdownloadinfo.h"
#include "download.h"
#include "../util/utils.h"
#include "../util/pathutil.h"

//--------------------------------------------------------------------------------
CDownload::CDownload( int DownloadId, yadmaptr<ISettings> Settings, yadmaptr<IDataBase> DataBase, yadmaptr<IAutenticator> Autenticator )
{
    m_Autenticator      = Autenticator;
    m_DataBase            = DataBase;
    m_Settings             = Settings;
    m_Id                 = DownloadId;
    m_Status             = DLS_PENDING;
    m_File                = NULL;
    m_Curl                = NULL;
    m_StatusVerified     = false;
    m_InfoNeeded        = false;
    m_InfoRetrived        = false;
    m_Errors            = 0;
    m_FileSize            = 0;
    m_Downloaded        = 0;


    LoadInfo();
}

//--------------------------------------------------------------------------------
bool CDownload::InitializeInfo()
{
    if ( ( !m_InfoRetrived ) && m_FilePath.empty() && ( m_Status == DLS_PENDING ) )
    {
        m_InfoRetrived = true;

        m_Status = DLS_GETTING_INFO;

        KeepRunning();

        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------
CDownload::~CDownload()
{
    CThread::Stop();

    Dispose();
}

//--------------------------------------------------------------------------------
void CDownload::Dispose()
{
    CMutexLocker Locker( m_mtxSpeed );

    if( m_Downloaded > 0 && ( m_Downloaded >= m_FileSize ) )
    {
        m_Status = DLS_FINISHED;
    }
    else
    {
        if ( m_Errors++ > 10 )
        {
            m_Status = DLS_BAD;
        }
    }

    if ( m_Status ==  DLS_DOWNLOADING )
    {
        m_Status = DLS_PENDING;
    }

    SaveInfo();

    if( m_File )
    {
        fclose( m_File );
        m_File = NULL;
    }

    if( m_Curl )
    {
        curl_easy_cleanup( m_Curl );
        m_Curl = NULL;
    }
}


//--------------------------------------------------------------------------------
void CDownload::IncrementDownloaded( int Bytes )
{
    m_mtxSpeed.Lock();

    m_Downloaded += Bytes;

    m_mtxSpeed.Unlock();
}

//--------------------------------------------------------------------------------
int CDownload::GetId()
{
    return m_Id;
}

//--------------------------------------------------------------------------------
std::string CDownload::GetOriginalUrl()
{
    return m_OriginalUrl;
}

//--------------------------------------------------------------------------------
std::string CDownload::GetRedirectedUrl()
{
    return m_RedirectedUrl;
}

//--------------------------------------------------------------------------------
std::string CDownload::GetFilePath()
{
    return m_FilePath;
}

//--------------------------------------------------------------------------------
std::string CDownload::GetPictureUrl()
{
    return m_PictureUrl;
}

//--------------------------------------------------------------------------------
std::string CDownload::GetFolder()
{
    return m_Folder;
}

//--------------------------------------------------------------------------------
int64_t CDownload::GetFileSize()
{
    return m_FileSize;
}

//--------------------------------------------------------------------------------
int64_t CDownload::GetDownloaded()
{
    m_mtxSpeed.Lock();

    int64_t RetVal = m_Downloaded;

    m_mtxSpeed.Unlock();

    return RetVal;
}

//--------------------------------------------------------------------------------
int CDownload::GetDownloadSpeed()
{
    m_mtxSpeed.Lock();

    int Speed = 0;

    if( m_Curl )
    {
        double dSpeed = 0;

        if( curl_easy_getinfo( m_Curl, CURLINFO_SPEED_DOWNLOAD, &dSpeed ) == CURLE_OK )
        {
            Speed = (size_t)dSpeed;
        }
    }

    m_mtxSpeed.Unlock();

    return Speed;
}

//--------------------------------------------------------------------------------
int CDownload::GetStatus()
{
    return m_Status;
}

//--------------------------------------------------------------------------------
void CDownload::KeepRunning()
{
    if( !IsRunning() )
    {
        if( ( m_Status == DLS_PENDING ) || ( m_Status == DLS_GETTING_INFO ) )
        {
            m_InfoNeeded = ( m_Status == DLS_GETTING_INFO );

            if ( !CThread::IsRunning() )
            {
                if( !m_InfoNeeded )
                {
                    m_Status = DLS_DOWNLOADING;
                }

                usleep( 100000 );

                CThread::Start();

                usleep( 100000 );
            }
        }
    }

}

//--------------------------------------------------------------------------------
bool CDownload::PrepareCurl()
{
    m_Curl = curl_easy_init();

    if ( m_Curl )
    {
        if( !m_Cookie.empty() )
        {
            curl_easy_setopt( m_Curl, CURLOPT_COOKIEFILE, "" ); // Apenas para iniciar o sistema de  cookies
            curl_easy_setopt( m_Curl, CURLOPT_COOKIELIST, m_Cookie.c_str() );
        }

        curl_easy_setopt( m_Curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt( m_Curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt( m_Curl, CURLOPT_WRITEFUNCTION, CDownload::WriteData );
        curl_easy_setopt( m_Curl, CURLOPT_WRITEDATA, this );
        curl_easy_setopt( m_Curl, CURLOPT_NOSIGNAL, 1L );                 //para nao ter problemas com threads
        curl_easy_setopt( m_Curl, CURLOPT_CONNECTTIMEOUT, 20L );
        curl_easy_setopt( m_Curl, CURLOPT_LOW_SPEED_LIMIT, 1000L );
        curl_easy_setopt( m_Curl, CURLOPT_LOW_SPEED_TIME, 20L );

        curl_easy_setopt( m_Curl, CURLOPT_FOLLOWLOCATION, 1L );
        curl_easy_setopt( m_Curl, CURLOPT_MAXREDIRS, 8L );

        if( m_Prefix.empty() )
        {
            curl_easy_setopt( m_Curl, CURLOPT_URL, m_OriginalUrl.c_str() );
        }else
        {
            curl_easy_setopt( m_Curl, CURLOPT_URL, m_RedirectedUrl.c_str() );
        }

        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------
void CDownload::Run()
{
    if ( m_InfoNeeded )
    {
        m_InfoNeeded = false;

        GetDownloadInformation();

        m_Status = DLS_PENDING;

        return;
    }

    m_Curl = NULL;
    m_File = NULL;

    if( GetDownloadInformation() )
    {
        m_Errors = 0;

        m_File = fopen( m_FilePath.c_str(), "ab+" );

        if( m_File != NULL )
        {
            fseek( m_File, 0, SEEK_END );
            m_Downloaded = ftell( m_File );

            if ( PrepareCurl() )
            {

                struct curl_slist *headers = NULL;

                if ( m_Downloaded > 0 )
                {
                    char Range[100] = "";
                    sprintf( Range, "Range: bytes=%lu-", (unsigned long) m_Downloaded );
                    headers = curl_slist_append(headers, Range );
                    curl_easy_setopt( m_Curl, CURLOPT_HTTPHEADER, headers );
                }

                curl_easy_perform( m_Curl );

                if ( headers )
                {
                    curl_slist_free_all( headers );
                }
            }
        }
        else
        {
            m_Errors = 1000; // nao acessou arquivo, nao deixar continuar
        }

    }

    Dispose();
}


//--------------------------------------------------------------------------------
size_t CDownload::WriteData( void *Buffer, size_t ElementSize, size_t ElementCount, void *UserData )
{
    CDownload* Download = (CDownload*) UserData;

    if( Download->StopRequested() || ElementSize != 1 )
    {
        return 0;
    }

    if( !Download->m_StatusVerified  )
    {
        Download->m_StatusVerified = true;

        long Status = 0;

        if ( curl_easy_getinfo( Download->m_Curl, CURLINFO_RESPONSE_CODE, &Status ) != CURLE_OK )
        {
            return 0;
        }
        else if( Status != ( Download->m_Downloaded > 0 ? 206 : 200 )  )
        {
            return 0;
        }

        double DownSize = 0;

        if( curl_easy_getinfo( Download->m_Curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &DownSize ) != CURLE_OK )
        {
            return  0;
        }
        else if ( DownSize != (double)Download->m_FileSize )
        {
            if( Download->m_FileSize == 0 )
            {
                Download->m_FileSize = (size_t) DownSize;
            }
            else
            {
                if( ( Download->m_Downloaded + (size_t) DownSize )  != Download->m_FileSize )
                {
                    return 0;
                }
            }
        }
    }

    size_t Count = fwrite( Buffer, ElementSize, ElementCount, Download->m_File );

    fflush( Download->m_File );

    Download->IncrementDownloaded( (int)(ElementCount * ElementSize) );

    return Count;
}

//--------------------------------------------------------------------------------
bool CDownload::GetDownloadInformation()
{
    LoadInfo();

    std::string Cookie = m_Cookie;
    std::string Final;
    std::string FileName;
    int64_t FileSize = 0;

    bool Result = GetDownloadInfo( m_Prefix + m_OriginalUrl, Cookie, Final, FileName, FileSize, !m_Prefix.empty() );

    if ( !Result )
    {
        Cookie = m_Autenticator->GetAutenticationCookie( m_OriginalUrl, &m_Prefix );

        if ( !Cookie.empty() )
        {
            Result = GetDownloadInfo( m_Prefix + m_OriginalUrl, Cookie, Final, FileName, FileSize , !m_Prefix.empty());
        }
    }

    if( Result )
    {
        if( m_FileSize == 0 || ( FileSize == m_FileSize ))
        {

            if( !m_Cookie.empty() )
            {
                m_Cookie = Cookie;
            }

            m_FileSize = FileSize;

            m_RedirectedUrl = Final;

            if( m_FilePath.empty() )
            {
                std::string Directory =  m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" );

                if( !m_Folder.empty() )
                {
                    Directory += "/" +  m_Folder;
                }

                if ( !CreateDirectory( Directory ) )
                {
                    Directory = m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" );
                }

                m_FilePath =  MakeExclusiveName( Directory + "/" + FileName );
            }

            SaveInfo();

            fflush( stdout );

            return true;
        }
    }

    fflush( stdout );

    return false;
}

//--------------------------------------------------------------------------------
void CDownload::LoadInfo()
{
    yadmaptr<IDBQuery> Query = m_DataBase->Query( "SELECT * FROM TB_DOWNLOADS WHERE ID_DOWNLOAD = " + IntToStr( m_Id ) );

    if( Query->Next() )
    {
        m_Prefix        = "";
        m_Downloaded     = Query->AsInteger( "DOWNLOADED" );
        m_FilePath         = Query->AsString( "FILENAME" );
        m_FileSize         = Query->AsInteger( "FILESIZE" );
        m_OriginalUrl     = Query->AsString( "URL" );
        m_Cookie        = Query->AsString( "COOKIE" );
        m_Folder        = Query->AsString( "FOLDER" );
        m_RedirectedUrl = Query->AsString( "REDIRECTEDURL" );
        m_PictureUrl    = Query->AsString( "PICTURE_URL" );

        if ( m_Cookie.empty() )
        {
            m_Cookie = m_Autenticator->GetLastCookie( m_OriginalUrl, &m_Prefix );

            if( m_Cookie.empty() )
            {
                m_Cookie = m_Autenticator->GetAutenticationCookie( m_OriginalUrl, &m_Prefix );
            }
        }


    }
}

//--------------------------------------------------------------------------------
void CDownload::SaveInfo()
{
    std::vector<std::string> Params;

    Params.push_back( IntToStr( m_Downloaded ) );
    Params.push_back( m_FilePath );
    Params.push_back( IntToStr( m_FileSize ) );
    Params.push_back( m_OriginalUrl );
    Params.push_back( m_RedirectedUrl );
    Params.push_back( m_Cookie );
    m_DataBase->Execute( "UPDATE TB_DOWNLOADS SET "
                            "DOWNLOADED = ?,"
                            "FILENAME = ?,"
                            "FILESIZE = ?,"
                            "URL = ?,"
                            "REDIRECTEDURL = ?,"
                            "COOKIE = ?"
                            " WHERE ID_DOWNLOAD = " + IntToStr( m_Id ) , Params );
}
