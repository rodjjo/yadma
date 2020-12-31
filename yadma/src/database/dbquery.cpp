#include <stdio.h>

#include "dbquery.h"

//-----------------------------------------------------------------------------
CDBQuery::CDBQuery( sqlite3* db, const std::string& Query, const std::vector<std::string>& Params )
{
    m_Statement = NULL;

    if( db == NULL )
    {
        return;
    }

    sqlite3_prepare_v2( db, Query.c_str() , -1, &m_Statement, NULL);

    if( m_Statement )
    {
        for( size_t i = 0; i < Params.size(); i++ )
        {
            sqlite3_bind_text( m_Statement, i + 1, Params[i].c_str(), -1, SQLITE_STATIC );
        }
    }
}

//-----------------------------------------------------------------------------
CDBQuery::CDBQuery( sqlite3* db, const std::string& Query )
{
    m_Statement = NULL;

    if( db == NULL )
    {
        return;
    }

    sqlite3_prepare( db, Query.c_str() , -1, &m_Statement, NULL);
}

//-----------------------------------------------------------------------------
CDBQuery::~CDBQuery()
{
    if( m_Statement )
    {
        sqlite3_finalize( m_Statement );
    }
}

//-----------------------------------------------------------------------------
bool CDBQuery::Next()
{
    if( m_Statement )
    {
        if( sqlite3_step( m_Statement ) == SQLITE_ROW )
        {
            return true;
        }
        sqlite3_finalize( m_Statement );
        m_Statement = NULL;
    }
    return false;
}

//-----------------------------------------------------------------------------
std::string CDBQuery::AsString( size_t Column )
{
    if( m_Statement )
    {
        if( sqlite3_column_text( m_Statement, Column ) != NULL )
        {
            return (const char*)sqlite3_column_text( m_Statement, Column );
        }
    }
    return "";
}

//-----------------------------------------------------------------------------
std::string CDBQuery::AsString( const std::string& ColumnName )
{
    if( m_Statement )
    {
        for( int i = 0; i < sqlite3_column_count(m_Statement); i++ )
        {
            if( ColumnName == sqlite3_column_name( m_Statement, i ) )
            {
                return AsString( (size_t) i );
            }
        }
    }
    return "";
}

//-----------------------------------------------------------------------------
int64_t CDBQuery::AsInteger( const std::string& ColumnName )
{
    if( m_Statement )
    {
        for( int i = 0; i < sqlite3_column_count(m_Statement); i++ )
        {
            if( ColumnName == sqlite3_column_name( m_Statement, i ) )
            {
                return AsInteger( (int64_t) i );
            }
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
int64_t CDBQuery::AsInteger( size_t Column )
{
    if( m_Statement )
    {
        return sqlite3_column_int64( m_Statement, Column );
    }
    return 0;
}

//-----------------------------------------------------------------------------
size_t CDBQuery::ColCount()
{
    if( m_Statement )
    {
        return sqlite3_column_count( m_Statement );
    }

    return 0;
}

//-----------------------------------------------------------------------------
std::string CDBQuery::GetColName( size_t Column )
{
    if( Column < ColCount() )
    {
        return sqlite3_column_name( m_Statement, Column );
    }

    return "";
}
