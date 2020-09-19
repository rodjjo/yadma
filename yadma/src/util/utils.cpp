#include <algorithm>
#include <sstream>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include "utils.h"

//----------------------------------------------------------------------------------------
void getAsDec(char* hex) 
{
	char tmp = tolower( hex[0] );
	
	if(tmp == 'a') {
		strcpy(hex,"10");
	}else if(tmp == 'b') {
		strcpy(hex,"11");
	}else if(tmp == 'c') {
		strcpy(hex,"12");
	}else if(tmp == 'd') {
		strcpy(hex,"13");
	}else if(tmp == 'e') {
		strcpy(hex,"14");
	}else if(tmp == 'f') {
		strcpy(hex,"15");
	}else if(tmp == 'g') {
		strcpy(hex,"16");
	}
	
}

//----------------------------------------------------------------------------------------
int convertToDec( const char* hex ) 
{
	char buff[12];
	
	sprintf(buff,"%s",hex);
	
	int ret = 0;
	
	int len = strlen(buff);
	
	for(int i=0;i<len;i++) 
	{
		char tmp[4];
		
		tmp[0] = buff[i];
		tmp[1] = '\0';
		
		getAsDec( tmp );
		
		int tmp_i = atoi( tmp );
		
		int rs = 1;
		
		for(int j=i;j<(len-1);j++) 
		{
			rs *= 16;
		}
		
		ret += (rs * tmp_i);
	}
	
	return ret;
}

//----------------------------------------------------------------------------------------
std::string UrlDecode( const std::string& Value )
{
	int len = Value.length();
	
	char* buff = new char[len + 1];
	
	strcpy(buff,Value.c_str());
	
	std::string ret;
	
	for(int i=0;i<len;i++) 
	{
		if( buff[i] == '+' ) 
		{
			ret = ret + " ";
		}else if( buff[i] == '%' ) 
		{
			char tmp[4];
			char hex[4];	
					
			hex[0] = buff[++i];
			hex[1] = buff[++i];
			hex[2] = '\0';
					
			sprintf(tmp,"%c",convertToDec(hex));
			
			ret = ret + tmp;
		}else 
		{
			ret = ret + buff[i];
		}
	}
	
	delete[] buff;
	
	return ret;
}

//----------------------------------------------------------------------------------------
static inline bool is_base64(unsigned char c) {
  return ( isalnum(c) || (c == '+') || (c == '/') );
}

//----------------------------------------------------------------------------------------
std::string Base64Decode( const std::string& encoded_string )
{
	static const std::string base64_chars =  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             								 "abcdefghijklmnopqrstuvwxyz"
                                             "0123456789+/";
	
	int in_len = encoded_string.size();
  	int i = 0;
  	int j = 0;
  	int in_ = 0;
  
  	unsigned char char_array_4[4], char_array_3[3];
  
  	std::string ret;

  	while (in_len-- && ( encoded_string[in_] != '=') && is_base64( encoded_string[in_] )) 
  	{
    	char_array_4[i++] = encoded_string[in_]; in_++;
    
    	if (i == 4) 
    	{
    	
			for (i = 0; i <4; i++)
  			{
    			char_array_4[i] = base64_chars.find(char_array_4[i]);
  			}

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
  			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
  			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

 			for (i = 0; (i < 3); i++)
  			{
				ret += char_array_3[i];
  			}
  			i = 0;
    	}
  	}

  	if (i) 
  	{
    	for (j = i; j <4; j++)
    	{
      	char_array_4[j] = 0;
    	}

    	for (j = 0; j <4; j++)
    	{
      	char_array_4[j] = base64_chars.find(char_array_4[j]);
    	}

    	char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    	char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    	char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    	for (j = 0; (j < i - 1); j++) 
    	{
    		ret += char_array_3[j];
    	}
  	}

  	return ret;
}

//----------------------------------------------------------------------------------------
int StrToInt( const std::string& Value, bool* Error )
{
	int RetVal = 0;
	
	if( sscanf( Value.c_str(), "%d", &RetVal ) == 1 )
	{
		return RetVal;
	}
	
	if( Error )
	{
		*Error = true;
	}
	
	return 0;
}

//----------------------------------------------------------------------------------------
std::string IntToStr( int Value )
{
	char RetVal[12] = "";
	
	sprintf( RetVal, "%d", Value );
	
	return RetVal;	
} 

//----------------------------------------------------------------------------------------
std::string CurrentDate()
{
	std::string RetVal;
	
	time_t rawtime;
  	
  	struct tm * timeinfo;
  	
  	time ( &rawtime );
  	
  	timeinfo = localtime ( &rawtime );
	
	if( timeinfo->tm_yday < 10 )
	{
		RetVal = "0";
	} 
	
	RetVal += IntToStr(timeinfo->tm_yday ) + "/";
	
	if( timeinfo->tm_mon < 10 )
	{
		RetVal += "0";
	}
	
	RetVal += IntToStr( timeinfo->tm_mon ) + "/" + IntToStr( timeinfo->tm_year );
	
	return RetVal;
}

//----------------------------------------------------------------------------------------
std::string CurrentTime()
{
	std::string RetVal;
	
	time_t rawtime;
  	
  	struct tm * timeinfo;
  	
  	time ( &rawtime );
  	
  	timeinfo = localtime ( &rawtime );
	
	if( timeinfo->tm_hour < 10 )
	{
		RetVal = "0";
	} 
	
	RetVal += IntToStr( timeinfo->tm_hour ) + ":";
	
	if( timeinfo->tm_min < 10 )
	{
		RetVal += "0";
	}
	
	RetVal += IntToStr( timeinfo->tm_min ) + ":";
	
	if( timeinfo->tm_sec < 10 )
	{
		RetVal += "0";
	}
	
	RetVal += IntToStr( timeinfo->tm_sec );
	
	return RetVal;
}

//----------------------------------------------------------
std::string Trim( const std::string& Value )
{
	if( Value.empty() )
	{
		return Value;
	}
	
	size_t p1 = 0;
	for( p1 = 0; p1 < Value.size(); p1++ )
	{
		if( Value[p1] > ' ' )
		{
			break;
		}
	}
	
	size_t p2 = 0;
	for( p2 = Value.size(); p2 > 0; p2-- )
	{
		if( Value[p2-1] > ' ' )
		{
			break;
		}
	}
	
	return Value.substr( p1, p2 ); 
}

//----------------------------------------------------------
void ToHexadecimal( size_t Value, char* Buffer )
{
	sprintf( Buffer, "%X", Value );	
}

//----------------------------------------------------------
std::string ReplaceAll( const std::string& Value, const std::string& Math, const std::string& Replace )
{
	std::string rs = Value;
	
	std::string::size_type start = rs.find_first_of( Math );
		
	while ( start != std::string::npos ) 
	{
		rs.replace( start, Math.size(), Replace );
		start = rs.find_first_of( Math, start + Replace.size() );
	}
	
	return rs;
}

//----------------------------------------------------------
std::string HtmlEncode( const std::string& Value )
{
	static struct HTMLReplace 
	{
		const char* match;
		const char* replace;
	} codes[3] = { {"&", "&amp;"}, {"<", "&lt;"}, {">", "&gt;"} };
	
	std::string rs = Value;

	for ( size_t i = 0; i < 3; i++ ) 
	{
		std::string::size_type start = rs.find_first_of( codes[i].match );
		
		while ( start != std::string::npos ) 
		{
			rs.replace( start, strlen( codes[i].match ), codes[i].replace );
			start = rs.find_first_of( codes[i].match, start + strlen( codes[i].replace ) );
		}
	}
	
	return rs;
}


//------------------------------------------------------------------------------
std::string FormatBytes( size_t Len )
{
    if ( Len == (size_t)-1 )
    {
        return "???";
    }

    const char* Sufix[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

    double LastResult = Len;
    double Result = Len;

    for ( int i = 1; i < 10; i++ )
    {
        Result /= 1024.0;
        if (  Result <= 1.0 )
        {
            char Buffer[20] = "";
            sprintf( Buffer, "%0.2f %s",LastResult, Sufix[ i - 1] );
            return Buffer;
        }
        LastResult = Result;
    }

    return  "???";
}


//------------------------------------------------------------------------------
std::string FormatTime( size_t Seconds )
{
    size_t Years = Seconds / ( 3600 * 24 * 365);
    
    if( Years > 3 )
    {
		return ":(";
    }

    std::stringstream RetVal;

    if ( Years > 0 )
    {
        RetVal << Years << " A. " << Seconds / ( 3600 * 24) - ( Years * 365) << " D.";
        return RetVal.str();
    }

    size_t Days = Seconds / (3600 * 24);

    if ( Days > 0 )
    {
        RetVal <<  Days << " D. " << (Seconds / ( 3600)) - ( Days * 24) << " H.";
        return RetVal.str();
    }

    size_t Hours = Seconds / 3600;
    char Buffer[ 20 ] = "";
    sprintf( Buffer, "%02lu:%02lu:%02lu", (unsigned long)Hours, (unsigned long)( (Seconds / 60) - ( Hours * 60) ), (unsigned long)(Seconds % 60) );
    
    return Buffer;
}

//------------------------------------------------------------------------------
std::string StrToLower( const std::string& Value )
{
    std::string RetVal = Value;
    std::transform( RetVal.begin(), RetVal.end(), RetVal.begin(), tolower );
    return RetVal;
}
