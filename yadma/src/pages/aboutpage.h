#ifndef ABOUTPAGE_H_
#define ABOUTPAGE_H_

#include "../webserver/iwebserverlistener.h"

class CAboutPage: public IWebServerListener
{
public:
    CAboutPage();
    virtual ~CAboutPage();
     std::pair<int, std::string> HandleRequest( void *ev_data );
};    
    
#endif /*ABOUTPAGE_H_*/
