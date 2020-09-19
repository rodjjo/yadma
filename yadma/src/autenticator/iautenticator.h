#ifndef IAUTENTICATOR_H_
#define IAUTENTICATOR_H_

#include <string>

class IAutenticator
{
public:
	virtual ~IAutenticator(){};
	virtual std::string GetAutenticationCookie( const std::string& Url, std::string* Prefix = (std::string*)0 ) = 0;
	virtual std::string GetLastCookie( const std::string& Url, std::string* Prefix = (std::string*)0 ) = 0;
	virtual void Update() = 0;
};

#endif /*IAUTENTICATOR_H_*/
