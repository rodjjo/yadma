#ifndef DATABASE_H_
#define DATABASE_H_

#include <vector>
#include <string>
#include <pthread.h>
#include <sqlite3.h>

#include "../thread/mutex.h"
#include "idatabase.h"

/*    Classe:    CDataBase
 *    Centralizar o acesso ao banco de dados      
 * */
 
//-----------------------------------------------------------------------------
class CDataBase: public IDataBase
{
public:
    CDataBase( const std::string& DataBasePath );
    virtual ~CDataBase();
    bool IsOpen();
    yadmaptr<IDBQuery> Query( const std::string& Sql, const std::vector<std::string>& Params );
    yadmaptr<IDBQuery> Query( const std::string& Sql );
    bool Execute( const std::string& Sql, const std::vector<std::string>& Params );
    bool Execute( const std::string& Sql );
    int Insert( const std::string& Sql, const std::vector<std::string>& Params );
    int Insert( const std::string& Sql );
    
private:
    sqlite3* m_db;
    CMutex m_mtxInsertKey;
    
};

#endif /*DATABASE_H_*/
