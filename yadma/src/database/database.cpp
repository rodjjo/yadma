#include <stdio.h>
#include "dbquery.h"
#include "database.h"
#include "database_schema.h"

//-------------------------------------------------------------------------------------------------
CDataBase::CDataBase( const std::string& DataBasePath  )
{
    m_db =  (sqlite3*)0;

    int rc =  sqlite3_open_v2( DataBasePath.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL );

    if ( rc )
    {
        fprintf(stderr, "Can't open database: %s (%s)\n", sqlite3_errmsg(m_db), DataBasePath.c_str() );

        sqlite3_close( m_db );

        m_db = (sqlite3*)0;
    } else {
        Execute(SCHEMA);
    }
}

//-------------------------------------------------------------------------------------------------
CDataBase::~CDataBase()
{
    if ( m_db != (sqlite3*)0 )
    {
        sqlite3_close( m_db );
        m_db = (sqlite3*)0;
    }
}

//-------------------------------------------------------------------------------------------------
bool CDataBase::IsOpen()
{
    return m_db != (sqlite3*)0;
}

//-------------------------------------------------------------------------------------------------
yadmaptr<IDBQuery> CDataBase::Query( const std::string& Sql, const std::vector<std::string>& Params )
{
    return yadmaptr<IDBQuery>( new CDBQuery( m_db, Sql, Params ) );
}

//-------------------------------------------------------------------------------------------------
yadmaptr<IDBQuery> CDataBase::Query( const std::string& Sql )
{
    return yadmaptr<IDBQuery>( new CDBQuery( m_db, Sql ) );
}

//-------------------------------------------------------------------------------------------------
bool CDataBase::Execute( const std::string& Sql, const std::vector<std::string>& Params )
{
    if( !IsOpen() )
    {
        return false;
    }

    CMutexLocker Locker( m_mtxInsertKey );

    sqlite3_stmt *statement = NULL;

    sqlite3_prepare_v2( m_db, Sql.c_str() , -1, &statement, NULL);

    if( statement )
    {
        for( size_t i = 0; i < Params.size(); i++ )
        {
            sqlite3_bind_text( statement, i + 1, Params[i].c_str(), -1, SQLITE_STATIC );
        }

        int Result = sqlite3_step( statement );

        sqlite3_finalize( statement );

        if( Result == SQLITE_DONE )
        {
            return true;
        }

    }

    return false;
}

//-------------------------------------------------------------------------------------------------
bool CDataBase::Execute( const std::string& Sql )
{
    if( !IsOpen() )
    {
        return false;
    }
    CMutexLocker Locker( m_mtxInsertKey );

    return sqlite3_exec( m_db, Sql.c_str(), NULL, NULL, NULL ) == SQLITE_DONE;
}

//-------------------------------------------------------------------------------------------------
int CDataBase::Insert( const std::string& Sql, const std::vector<std::string>& Params )
{
    if( !IsOpen() )
    {
        return 0;
    }

    CMutexLocker Locker( m_mtxInsertKey );

    sqlite3_stmt *statement = NULL;

    sqlite3_prepare_v2( m_db, Sql.c_str() , -1, &statement, NULL);

    if( statement )
    {
        for( size_t i = 0; i < Params.size(); i++ )
        {
            sqlite3_bind_text( statement, i + 1, Params[i].c_str(), -1, SQLITE_STATIC );
        }

        int Result = sqlite3_step( statement );

        sqlite3_finalize( statement );

        if( Result == SQLITE_DONE )
        {
            return (size_t)sqlite3_last_insert_rowid( m_db );
        }

    }

    return 0;
}

//-------------------------------------------------------------------------------------------------
int CDataBase::Insert( const std::string& Sql )
{
    if( !IsOpen() )
    {
        return 0;
    }

    CMutexLocker Locker( m_mtxInsertKey );

    if( sqlite3_exec( m_db, Sql.c_str(), NULL, NULL, NULL ) == SQLITE_DONE )
    {
        return (int)sqlite3_last_insert_rowid( m_db );
    }

    return 0;
}
