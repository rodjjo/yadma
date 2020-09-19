
//int main(){ return 0; }

#include <stdio.h>

#include <vector>
#include <map>

#include "objectcontainer.h"

//---------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
	printf( "yadma 1.0 (yes another download manager application)\n\n" );
	
	if( CObjectContainer::GetDataBase()->IsOpen() )  
	{ 
		CObjectContainer::GetParameterParser()->SetParams( argc, argv );
	 	
	 	fprintf( stdout, "database opened successfull...\nStarting download manager...\n" );
 	
		if( CObjectContainer::GetDownloaderManager()->Start() )
		{
			printf( "download manager started... :)\nStarting WebServer...\n" );
			
			if( CObjectContainer::GetWebServer()->Start() )
			{
				fprintf( stdout, "very good !!! yadma started with success\n" );

				CObjectContainer::GetDownloaderManager()->Loop();
				
				CObjectContainer::GetWebServer()->Stop();
				
				return 0;			
			}
			else
			{
				fprintf( stderr, "web server not started...\n" );
			}
		}
		else
		{
			fprintf( stderr, "download manager not started...\n" );
		}
	}
	else
	{
		fprintf( stderr, "Db not open... exiting..." );
	}
	
	return 1;
} 
