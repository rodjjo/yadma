#include <stdio.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sstream>

#include "../util/pathutil.h"
#include "directoriespages.h"

#define ENTRY_TYPE 1
#define ENTRY_NAME 2
#define ENTRY_SIZE 3

//-------------------------------------------------------------------------------------------------
CDirectoriesPages::CDirectoriesPages( yadmaptr<ISettings> Settings )
{
    m_Settings = Settings;
}

//-------------------------------------------------------------------------------------------------
CDirectoriesPages::~CDirectoriesPages()
{
}

//-------------------------------------------------------------------------------------------------
std::pair<int,std::string> CDirectoriesPages::HandleRequest( void *ev_data )
{
    std::string response = ProcessDirList( ev_data );

    if ( response.size() == 0 )
    {
        response = ProcessDirInclusion( ev_data );
        if ( response.size() == 0 )
        {
            ProcessDirExclusion( ev_data );
        }
    }
}

//-------------------------------------------------------------------------------------------------
std::string CDirectoriesPages::getDirInfo( void *ev_data, const char* BaseDir, void* Entry )
{
    struct dirent* adir = (struct dirent*) Entry;

    if( ( strncmp( adir->d_name, ".", PATH_MAX ) == 0 ) ||
        ( strncmp( adir->d_name, "..", PATH_MAX ) == 0 ) )
    {
        return std::string();
    }

    struct stat entryInfo;

    char pathName[PATH_MAX + 1] = "";

    strncpy( pathName, BaseDir, PATH_MAX );
    strncat( pathName, "/", PATH_MAX );
    strncat( pathName, adir->d_name, PATH_MAX );
 #ifdef WIN32
    auto attr = GetFileAttributesA( pathName );
    if( attr != 0xFFFFFFFF )
    {
        int Type = 0;
        long long Size = 0;

        if( attr && FILE_ATTRIBUTE_DIRECTORY )
        {
            Type = 1;
        } else {
            Type = 2;
            Size = 0; // TODO: implement on windows
        }

        std::stringstream ss;

        ss << ENTRY_TYPE << ":" << Type << "\n";
        ss << ENTRY_NAME << ":" <<  adir->d_name << "\n";
        ss << ENTRY_SIZE << ":" << Size << "\n";

        return ss.str();
    }
 #else
    if( lstat( pathName, &entryInfo ) == 0 )
    {
        int Type = 0;
        long long Size = 0;

        if( S_ISDIR( entryInfo.st_mode ) )
        {
            Type = 1;
        } else if( S_ISREG( entryInfo.st_mode ) )
        {
            Type = 2;
            Size = (long long)entryInfo.st_size;
        } else
        {
            return std::string();
        }

        std::stringstream ss;

        ss << ENTRY_TYPE << ":" << Type << "\n";
        ss << ENTRY_NAME << ":" <<  adir->d_name << "\n";
        ss << ENTRY_SIZE << ":" << Size << "\n";

        return ss.str();
    }
#endif

    return std::string();
}


//-------------------------------------------------------------------------------------------------
std::string CDirectoriesPages::ProcessDirInclusion( void *ev_data )
{
    if ( get_var( "inc", ev_data ) == "1" )
     {
        std::string Name =  get_var( "name", ev_data );

        if( Name.find_first_of( ".." ) == std::string::npos )
        {
            if ( CreateDirectory(  m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" ) + std::string("/") + Name ) )
            {
                return "ok";
            }
        }
     }

    return std::string();
}

//-------------------------------------------------------------------------------------------------
std::string CDirectoriesPages::ProcessDirExclusion(  void *ev_data )
{
    if ( get_var( "del", ev_data ) == "1" )
     {
        std::string Name = get_var( "name", ev_data );

        if( Name.find_first_of( ".." ) == std::string::npos )
        {
            if ( RemoveDirectory( m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" ) + std::string("/") + Name ) )
            {
                return "ok";
            }
        }
     }

     return std::string();
}


//-------------------------------------------------------------------------------------------------
std::string CDirectoriesPages::ProcessDirList( void *ev_data )
{
    std::stringstream response;

    if ( get_var( "list", ev_data ) == "1" )
    {

#ifdef WIN32
    return "not implemented yet";
#else
        std::string From = get_var( "from", ev_data );

        if ( From.find( ".." ) == std::string::npos )//nao deixar "Hackear" ir fora do diretorio de downloads
        {
            From  = m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" ) + "/" + From;

            DIR*            dir = NULL;
            struct dirent     entry;
            struct dirent*    entryPtr = NULL;

            dir = opendir( From.c_str() );

            if( dir != NULL )
            {
                readdir_r( dir, &entry, &entryPtr );

                while( entryPtr != NULL )
                {
                    response <<  getDirInfo( ev_data, From.c_str(), &entry );

                    entryPtr = NULL;

                    readdir_r( dir, &entry, &entryPtr );
                }

                closedir( dir );
            } else
            {
                response << "error";
            }
        }
 #endif
    }

    return response.str();
}