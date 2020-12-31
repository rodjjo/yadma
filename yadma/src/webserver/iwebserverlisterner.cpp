#include "iwebserverlistener.h"


//----------------------------------------------------------------------------------
std::string IWebServerListener::get_var(const char* name, void *ev_data)
{
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    char temp[4096] = { 0, };

    if (is_post(ev_data)) {
        mg_http_get_var(&hm->body, name, temp, sizeof(temp));
    } else if ( hm->query.ptr ) {
        mg_http_get_var(&hm->query, name, temp, sizeof(temp));
    }
    return temp;
}

//----------------------------------------------------------------------------------
bool IWebServerListener::is_post(void *ev_data)
{
    struct mg_http_message *hm = (struct mg_http_message *) ev_data;
    return  mg_vcasecmp(&hm->method, "POST") == 0;
}