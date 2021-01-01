#ifndef IWEBSERVERLISTENER_H_
#define IWEBSERVERLISTENER_H_

#include <stdio.h>
#include  <string>
#include  <utility>
#include "mongoose.hpp"

//-----------------------------------------------------------------------------------------------------------------------
class IWebServerListener
{
public:
    virtual ~IWebServerListener(){};
    virtual std::pair<int, std::string> HandleRequest( void *ev_data ) = 0;
    // virtual void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info ) = 0;
    std::string get_var(const char* name, void *ev_data);
    bool is_post(void *ev_data);
};

#endif /*IWEBSERVERLISTENER_H_*/
