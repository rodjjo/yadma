#ifndef IPARAMETERPARSER_H_
#define IPARAMETERPARSER_H_

#include <string>

class IParameterParser
{
public:
    virtual ~IParameterParser(){};
    virtual void SetParams( int argc, char* argv[] ) = 0;
    virtual std::string GetParamValue( const std::string& ParamName ) = 0;
};

#endif /*IPARAMETERPARSER_H_*/
