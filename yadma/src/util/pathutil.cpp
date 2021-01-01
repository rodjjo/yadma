#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

#include "utils.h"
#include "pathutil.h"

//----------------------------------------------------------
std::string GetAppPath()
{
    char szBuffer[512] = "";
    char szTmp[32] = "";

    sprintf( szTmp, "/proc/%d/exe", getpid() );

    int Size = readlink( szTmp, szBuffer, sizeof( szBuffer ) );

    if( Size < 0 )
    {
        return "";
    }

    return std::string( szBuffer, Size );
}

//----------------------------------------------------------
std::string GetAppDirectory()
{
    std::string Path = GetAppPath();

    size_t p = Path.rfind( '/' );

    if( p != std::string::npos )
    {
        return Path.substr( 0, p + 1 );
    }

    return ".";
}

//----------------------------------------------------------
std::string MakeExclusiveName( const std::string& FileName )
{
    if ( FileExists( FileName ) )
    {
        size_t p = FileName.rfind( "." );

        if ( p == std::string::npos )
        {
            p = FileName.length();
        }

        std::string RetVal;

        for ( int i = 1; i < 1001; i++ )
        {
            RetVal = FileName.substr( 0 ,  p ) + "(" + IntToStr( i ) + ")" + FileName.substr(p);

            if ( !FileExists( RetVal ) )
            {
                return RetVal;
            }
        }
    }

    return FileName;
}

//----------------------------------------------------------
bool FileExists( const std::string& FileName )
{
    FILE* file = fopen( FileName.c_str(), "r" );

    if( file )
    {
        fclose(file);

        return true;
    }

    return false;
}

//----------------------------------------------------------
bool CreateDirectory( const std::string& Directory )
{
    if ( mkdir( Directory.c_str(), S_IRWXU | S_IRWXG | S_IRWXO ) == 0 )
    {
        return true;
    }

    return ( errno == EEXIST );
}

//----------------------------------------------------------
bool RemoveDirectory( const std::string& Directory )
{
    if ( rmdir( Directory.c_str() ) == 0 )
    {
        return true;
    }

    return false;
}
