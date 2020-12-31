#ifndef PARAMETERPARSER_H_
#define PARAMETERPARSER_H_

#include <map>
#include "iparameterparser.h"

class CParameterParser : public IParameterParser 
{
public:
    typedef std::map<std::string,std::string> TParameterMap;
    
public:
    CParameterParser();
    virtual ~CParameterParser();
    void SetParams( int argc, char* argv[] );
    std::string GetParamValue( const std::string& ParamName );
    
private:
    TParameterMap                 m_Parameters;
};

#endif /*PARAMETERPARSER_H_*/
