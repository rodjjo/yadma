#ifndef IDBQUERY_H_
#define IDBQUERY_H_

#include <string>


//------------------------------------------------------------------------------
class IDBQuery
{
public:
	virtual ~IDBQuery(){};
	virtual bool Next() = 0;
	virtual size_t ColCount() = 0;
	virtual std::string GetColName( size_t Column ) = 0;
	virtual std::string AsString( size_t Column ) = 0;
	virtual std::string AsString( const std::string& ColumnName ) = 0;
	virtual	int AsInteger( const std::string& ColumnName ) = 0;
	virtual int AsInteger( size_t Column ) = 0;
};

#endif /*IDBQUERY_H_*/
