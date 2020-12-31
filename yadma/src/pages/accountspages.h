#ifndef ACCOUNTSPAGES_H_
#define ACCOUNTSPAGES_H_

#include <string>
#include "../util/yadmaptr.h"
#include "../webserver/iwebserverlistener.h"
#include "../database/idatabase.h" 
#include "../autenticator/iautenticator.h"

class CAccountsPages: public IWebServerListener
{
public:
    CAccountsPages( yadmaptr<IDataBase> DataBase, yadmaptr<IAutenticator> Autenticator );
    virtual ~CAccountsPages();
    std::pair<int, std::string> HandleRequest( void *ev_data );
    
private:
    bool SaveAccount( void *ev_data );    
    std::string GetParam(  void *ev_data, const char *ParamName, bool& Found );
    
private:
    yadmaptr<IDataBase> m_DataBase;    
    yadmaptr<IAutenticator> m_Autenticator;    
};    

#endif /*ACCOUNTSPAGES_H_*/
