#ifndef DIRECTORIESPAGES_H_
#define DIRECTORIESPAGES_H_

#include "../util/yadmaptr.h"
#include "../webserver/iwebserverlistener.h"
#include "../settings/isettings.h"

class CDirectoriesPages: public IWebServerListener
{
public:
    CDirectoriesPages( yadmaptr<ISettings> Settings );
    virtual ~CDirectoriesPages();
    std::pair<int, std::string> HandleRequest( void *ev_data );
    
private:
    std::string GetVar( void *ev_data, const char* var_name );
    std::string ProcessDirList( void *ev_data );
    std::string getDirInfo( void *ev_data, const char* BaseDir, void* Entry );
    std::string ProcessDirInclusion( void *ev_data );
    std::string ProcessDirExclusion( void *ev_data );
    
private:
    yadmaptr<ISettings> m_Settings;
};

#endif /*DIRECTORIESPAGES_H_*/
