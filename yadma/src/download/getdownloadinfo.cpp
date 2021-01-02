#include <algorithm>
#include <curl/curl.h>
#include "../util/utils.h"
#include "../objectcontainer.h"
#include "getdownloadinfo.h"


//------------------------------------------------------------------------------
typedef struct
{
    std::string        LastUrl;
    std::string        FileName;
    int64_t          FileSize;
    bool            Sucess;
} TRedirectionResult;

//------------------------------------------------------------------------------
std::string UrlToFileName(  const std::string& Url )
{
    std::string RetVal;

    size_t Pos = Url.rfind( '/' );

    if ( Pos != std::string::npos )
    {
        RetVal = Url.substr( Pos + 1 );
    }

    if ( RetVal.empty() || RetVal.find( '?' ) != std::string::npos )
    {
        return "index.html";
    }

    return RetVal;
}


//------------------------------------------------------------------------------
size_t ProcessHeader( void *ptr, size_t size, size_t nmemb, void *UserData )
{
    if ( size != 1 || nmemb < 1 )
    {
        return nmemb * size;
    }

    char* BufferAsChar = (char*)ptr;

    char SavedChar =  BufferAsChar[ nmemb - 1 ];

    BufferAsChar[ nmemb - 1 ] = '\0';

    std::string Data( BufferAsChar );
    BufferAsChar[ nmemb - 1 ] = SavedChar;

    Data.append( 1, SavedChar );

    if ( StrToLower( Data ).find( "content-disposition" ) != std::string::npos )
    {
        *((std::string*)UserData) = Data;
    }

    return size * nmemb;
}

//------------------------------------------------------------------------------
size_t Write_null( void *Buffer, size_t ElementSize, size_t ElementCount, void *UserData )
{
    return 0; // cancelar, s� pegar cabecalhos
}

//------------------------------------------------------------------------------
TRedirectionResult* RedirectToUrl( const std::string& Url, std::string& Cookie )
{
    if ( Url.empty() )
    {
        return NULL;
    }

    CURL* curl = curl_easy_init();

    if ( curl == NULL )
    {
        return NULL;
    }

    std::string LastContentDisp;

    if( !Cookie.empty() )
    {
        curl_easy_setopt( curl, CURLOPT_COOKIEFILE, ""); // Apenas para iniciar o sistema de  cookies
        curl_easy_setopt( curl, CURLOPT_COOKIELIST, Cookie.c_str() );
    }

    curl_easy_setopt( curl, CURLOPT_CONNECTTIMEOUT, 10L );
    curl_easy_setopt( curl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt( curl, CURLOPT_TIMEOUT, 30L );
    curl_easy_setopt( curl, CURLOPT_FOLLOWLOCATION, 1L );
    curl_easy_setopt( curl, CURLOPT_MAXREDIRS, 8L );
    curl_easy_setopt( curl, CURLOPT_NOSIGNAL, 1L );                 //para nao ter problemas com threads
    curl_easy_setopt( curl, CURLOPT_URL, Url.c_str() );
    curl_easy_setopt( curl, CURLOPT_HEADERFUNCTION, ProcessHeader );
    curl_easy_setopt( curl, CURLOPT_WRITEHEADER,  &LastContentDisp ); //se eu nao "setasse" CURLOPT_HEADERFUNCTION teria que ser FILE*
    curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, Write_null ); // write_null retorna que escreveu zero, causar CURLE_WRITE_ERROR interromper transferencia

    CURLcode Code = curl_easy_perform( curl );

    if ( Code == CURLE_OK || Code == CURLE_WRITE_ERROR )
    {
        long Code = 0;
        if ( curl_easy_getinfo( curl, CURLINFO_RESPONSE_CODE, &Code ) == CURLE_OK )
        {
            long RedirectCount = 0;

            curl_easy_getinfo( curl, CURLINFO_REDIRECT_COUNT, &RedirectCount );

            if ( Code == 200 )
            {
                TRedirectionResult* Res = new TRedirectionResult;
                Res->FileSize = (size_t)-1;
                Res->FileName.clear();
                Res->LastUrl.clear();

                char *url = NULL;
                if( curl_easy_getinfo( curl, CURLINFO_EFFECTIVE_URL, &url ) == CURLE_OK )
                {
                    Res->LastUrl = url;
                }

                double DownSize = 0;
                if( curl_easy_getinfo( curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &DownSize ) == CURLE_OK )
                {
                    if ( DownSize != 0 )
                    {
                        Res->Sucess = DownSize != -1;
                        Res->FileSize = (size_t)DownSize;
                    }
                }

                if ( LastContentDisp.empty() )
                {
                    Res->FileName = UrlToFileName( Res->LastUrl );
                }
                else
                {
                    //TODO: COLOCAR EXPRESSAO REGULAR
                    size_t Pos = StrToLower( LastContentDisp ).find( "filename=\"" );
                    if ( Pos != std::string::npos )
                    {
                        Res->Sucess = true;
                        //TODO: COLOCAR EXPRESSAO REGULAR

                        Res->FileName = LastContentDisp.substr( Pos + 10 );

                        Pos =  Res->FileName.find( "\"" );

                        if ( Pos != std::string::npos )
                        {
                            Res->FileName = Res->FileName.substr( 0, Pos );
                        }
                    }

                    if( Res->FileName.empty() )
                    {
                        Res->FileName = UrlToFileName( Res->LastUrl );
                    }

                    if ( Res->FileName.find(".html") == Res->FileName.size() - 4)
                    {
                        Res->Sucess = false;
                    }

                }

                curl_easy_cleanup( curl );

                return Res;
            }
        }

    }

    curl_easy_cleanup( curl );

    return NULL;
}

//------------------------------------------------------------------------------
size_t WriteInfo( void *Buffer, size_t ElementSize, size_t ElementCount, void *UserData )
{
    std::string* Data = (std::string*)UserData;

    if ( Data->size() + ElementCount > 5000 )
    {
        if ( Data->size() < 5000 )
        {
            if ( 5000 - Data->size() < ElementCount )
            {
                ElementCount = 5000 - Data->size();
            }
            (*Data) += std::string( (char*) Buffer, ElementCount );
        }

        return 0;
    }else
    {
        (*Data) += std::string( (char*) Buffer, ElementCount );
    }

    return ElementCount;
}

//------------------------------------------------------------------------------
std::string GetUrlFromContent( const std::string& Url, const std::string& Cookie )
{
    CURL* Curl = curl_easy_init();

    if ( Curl )
    {
        std::string Content;

        if( !Cookie.empty() )
        {
            curl_easy_setopt( Curl, CURLOPT_COOKIEFILE, ""); // Apenas para iniciar o sistema de  cookies
            curl_easy_setopt( Curl, CURLOPT_COOKIELIST, Cookie.c_str() );
        }

        curl_easy_setopt( Curl, CURLOPT_NOSIGNAL, 1L );                 //para nao ter problemas com threads
        curl_easy_setopt( Curl, CURLOPT_FOLLOWLOCATION, 1L );
        curl_easy_setopt( Curl, CURLOPT_MAXREDIRS, 8L );
        curl_easy_setopt( Curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt( Curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt( Curl, CURLOPT_WRITEFUNCTION, WriteInfo );
        curl_easy_setopt( Curl, CURLOPT_WRITEDATA, &Content );
        curl_easy_setopt( Curl, CURLOPT_CONNECTTIMEOUT, 20L );
        curl_easy_setopt( Curl, CURLOPT_LOW_SPEED_LIMIT, 1000L );
        curl_easy_setopt( Curl, CURLOPT_LOW_SPEED_TIME, 10L );
        curl_easy_setopt( Curl, CURLOPT_URL, Url.c_str() );

        curl_easy_perform( Curl );

        curl_easy_cleanup( Curl );

        size_t p = 0;

        if( ( p = Content.find( "<iframe" ) ) != std::string::npos )
        {
            Content = Content.substr( p );

            if ( ( p = Content.find( "http://" ) ) != std::string::npos )
            {
                if ( p > 0 )
                {
                    char separator = Content[ p - 1 ];
                    Content = Content.substr( p );

                    if ( ( p = Content.find( separator ) ) != std::string::npos )
                    {
                        return Content.substr( 0, p );
                    }
                }
            }
        }

    }

    return "";
}

//------------------------------------------------------------------------------
bool GetDownloadInfo( const std::string& Url, std::string& Cookie, std::string& Final, std::string& FileName, int64_t& FileSize, bool Prefixed )
{
    TRedirectionResult* Res = (TRedirectionResult*)0;

    if ( Prefixed )
    {
        Res = RedirectToUrl( GetUrlFromContent( Url, Cookie  ), Cookie );
    }
    else
    {
        Res = RedirectToUrl( Url, Cookie );
    }

    if ( Res )
    {
        Final               = Res->LastUrl;
        FileName            = Res->FileName;

        FileSize            = Res->FileSize;
        bool HasSucess      = Res->Sucess; // conseguindo nome ou tamanho do arquivo, indicar que houve sucesso

        delete     Res;

        return HasSucess;
    }

    return false;
}
