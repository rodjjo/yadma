
//int main(){ return 0; }

#include <stdio.h>

#include <iostream>
#include <vector>
#include <map>

#include "objectcontainer.h"

//---------------------------------------------------------------------------
int entry( int argc, char* argv[] )
{
    std::cout << "yadma 1.0 (yes another download manager application)\n\n";

    if( CObjectContainer::GetDataBase()->IsOpen() )
    {
        CObjectContainer::GetParameterParser()->SetParams( argc, argv );
        std::cout << "database opened successfully...\nStarting download manager...\n";

        if( CObjectContainer::GetDownloaderManager()->Start() )
        {
            std::cout << "download manager started... :)\nStarting WebServer...\n";
            if( CObjectContainer::GetWebServer()->Start() )
            {
                std::cout << stdout, "very good !!! yadma started with success\n";
                CObjectContainer::GetDownloaderManager()->Loop();
                CObjectContainer::GetWebServer()->Stop();
                return 0;
            }
            else
            {
                std::cout << stderr, "web server not started...\n";
            }
        }
        else
        {
            std::cout << stderr, "download manager not started...\n";
        }
    }
    else
    {
        std::cout << stderr, "Db not open... exiting...";
    }

    return 1;
}

#ifdef WIN32
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
    return entry(__argc, __argv);
}
#else
int main( int argc, char* argv[] ) {
    return entry(argc, argv);
}
#endif