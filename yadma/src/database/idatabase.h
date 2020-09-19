#ifndef IDATABASE_H_
#define IDATABASE_H_

#include <vector>
#include "../util/yadmaptr.h"
#include "idbquery.h"

//--------------------------------------------------------------------------------------
class IDataBase
{
public:
	virtual ~IDataBase(){};
	virtual	bool IsOpen() = 0; 
	virtual yadmaptr<IDBQuery> Query( const std::string& Sql, const std::vector<std::string>& Params ) = 0;
	virtual yadmaptr<IDBQuery> Query( const std::string& Sql ) = 0;
	virtual bool Execute( const std::string& Sql, const std::vector<std::string>& Params ) = 0;
	virtual bool Execute( const std::string& Sql ) = 0;
	virtual int Insert( const std::string& Sql, const std::vector<std::string>& Params ) = 0;
	virtual int Insert( const std::string& Sql ) = 0;
};

#endif /*IDATABASE_H_*/
