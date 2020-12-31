#ifndef TOPPAGE_H_
#define TOPPAGE_H_

#include <string>
#include <utility>
#include "../webserver/iwebserverlistener.h"

class CTopPage: public IWebServerListener
{
public:
    CTopPage( );
    virtual ~CTopPage();
    std::pair<int, std::string> HandleRequest( void *ev_data );;
};

#endif /*TOPPAGE_H_*/
