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
	void HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info );
	
private:
	bool SaveAccount(struct mg_connection* conn );	
	std::string GetParam( struct mg_connection* conn, const std::string& ParamName, bool& Found );
	
private:
	yadmaptr<IDataBase> m_DataBase;	
	yadmaptr<IAutenticator> m_Autenticator;	
};	

#endif /*ACCOUNTSPAGES_H_*/
