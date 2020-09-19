#include "parameterparser.h"

//-------------------------------------------------------------------------------
CParameterParser::CParameterParser()
{
}

//-------------------------------------------------------------------------------
CParameterParser::~CParameterParser()
{
}

//-------------------------------------------------------------------------------
void CParameterParser::SetParams( int argc, char* argv[] )
{
	m_Parameters.clear();
	
	for( int i = 1; i < argc; i++ )
	{
		if( i + i < argc )
		{
			std::string Param = std::string( argv[i] );
			if( Param[0] == '-')
			{
				Param = Param.substr( 1 );
			} 
			m_Parameters[ Param ] = argv[ i + 1 ]; 		
		}
	}
}

//-------------------------------------------------------------------------------
std::string CParameterParser::GetParamValue( const std::string& ParamName )
{
	TParameterMap::iterator iter = m_Parameters.find( ParamName );

	if( iter != m_Parameters.end() )
	{
		return iter->second;
	}
	
	return "";
}
