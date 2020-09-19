#ifndef DBQUERY_H_
#define DBQUERY_H_

#include <vector>
#include <string>
#include <sqlite3.h>
#include "idbquery.h"

//-----------------------------------------------------------------------------
class CDBQuery: public IDBQuery
{
public:
	CDBQuery( sqlite3* db, const std::string& Query, const std::vector<std::string>& Params );
	CDBQuery( sqlite3* db, const std::string& Query );
	virtual ~CDBQuery();
	bool Next();
	size_t ColCount();
	std::string GetColName( size_t Column );
	std::string AsString( size_t Column );
	std::string AsString( const std::string& ColumnName );
	int AsInteger( const std::string& ColumnName );
	int AsInteger( size_t Column );
	
private:
	sqlite3_stmt* 	m_Statement;
}; 

#endif /*DBQUERY_H_*/

