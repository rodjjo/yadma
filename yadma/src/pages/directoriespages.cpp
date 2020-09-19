#include <stdio.h>

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>


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
void CDirectoriesPages::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	if ( !ProcessDirList( conn ) )
	{
		if ( !ProcessDirInclusion( conn ) )
		{
			ProcessDirExclusion( conn );
		}
	}
}

//-------------------------------------------------------------------------------------------------
std::string CDirectoriesPages::GetVar( struct mg_connection* conn, const std::string& VarName )
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

//-------------------------------------------------------------------------------------------------
void CDirectoriesPages::SendDirInfo( struct mg_connection* conn,const char* BaseDir, void* Entry )
{
	struct dirent* adir = (struct dirent*) Entry;
	
    if( ( strncmp( adir->d_name, ".", PATH_MAX ) == 0 ) ||
        ( strncmp( adir->d_name, "..", PATH_MAX ) == 0 ) ) 
    {
        return;
    }
    
    struct stat entryInfo;
    
    char pathName[PATH_MAX + 1] = "";
    
    strncpy( pathName, BaseDir, PATH_MAX );
    strncat( pathName, "/", PATH_MAX );
    strncat( pathName, adir->d_name, PATH_MAX );
    
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
			return;
        }
		
		mg_printf( conn, "%d:%d\n", ENTRY_TYPE, Type );    
		mg_printf( conn, "%d:%s\n", ENTRY_NAME, adir->d_name );
		mg_printf( conn, "%d:%lld\n", ENTRY_SIZE, Size );
    }
}

//-------------------------------------------------------------------------------------------------
bool CDirectoriesPages::ProcessDirList( struct mg_connection* conn )
{
	if ( GetVar( conn, "list" ) == "1" )
	{
		std::string From =   GetVar( conn, "from" );
		
		if ( From.find( ".." ) == std::string::npos )//nao deixar "Hackear" ir fora do diretorio de downloads
		{
			From  = m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" ) + "/" + From;
			
			DIR*			dir = NULL;
    		struct dirent 	entry;
    		struct dirent*	entryPtr = NULL;
    		
			dir = opendir( From.c_str() );
			
			if( dir != NULL )
			{
				readdir_r( dir, &entry, &entryPtr );
				
				while( entryPtr != NULL ) 
				{
					SendDirInfo( conn, From.c_str(), &entry );
					
					entryPtr = NULL;
					
					readdir_r( dir, &entry, &entryPtr );
				}
				
				closedir( dir );
			}else
			{
				mg_printf( conn, "%s", "error" ); 
			}
		}
		
		return true;
	} 
	
	return false;
}


//-------------------------------------------------------------------------------------------------
bool CDirectoriesPages::ProcessDirInclusion( struct mg_connection* conn )
{
	if ( GetVar( conn, "inc" ) == "1" )
 	{
		std::string Name =  GetVar( conn, "name" );
		
		if( Name.find_first_of( ".." ) == std::string::npos ) 
		{
			if ( CreateDirectory(  m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" ) + "/" + Name ) )
			{
				mg_printf( conn, "ok" );
			}
		}
		
		return true;	
 	}
 	return false;
}

//-------------------------------------------------------------------------------------------------
bool CDirectoriesPages::ProcessDirExclusion( struct mg_connection* conn )
{
	if ( GetVar( conn, "del" ) == "1" )
 	{
		std::string Name =  GetVar( conn, "name" );
		
		if( Name.find_first_of( ".." ) == std::string::npos ) 
		{
			if ( RemoveDirectory(  m_Settings->GetStrValue( "DOWNLOAD_DIRECTORY", "../downloads" ) + "/" + Name ) )
			{
				mg_printf( conn, "ok" );
			}
		}
		
		return true;	
 	}
 	
 	return false;
}
