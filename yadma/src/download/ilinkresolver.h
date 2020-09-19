#ifndef ILINKRESOLVER_H_
#define ILINKRESOLVER_H_

#include <string>

//----------------------------------------------------------------------------------------
class ILinkResolver
{
public:
	virtual ~ILinkResolver(){};
	bool Resolve( const std::string& OriginalUrl, std::string& OutputUrl, std::string& Cookie ) = 0;
	
}

#endif /*ILINKRESOLVER_H_*/
