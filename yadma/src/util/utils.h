#ifndef UTILS_H_
#define UTILS_H_

#include <inttypes.h>
#include <string>

int64_t StrToInt( const std::string& Value, bool* Error = (bool*)0 );
std::string IntToStr( int64_t Value );
std::string CurrentDate();
std::string CurrentTime();
std::string Trim( const std::string& Value );
std::string ReplaceAll( const std::string& Value, const std::string& Math, const std::string& Replace );
std::string HtmlEncode( const std::string& Value );
void ToHexadecimal( size_t Value, char* Buffer );
std::string FormatBytes( size_t Len );
std::string FormatTime( size_t Seconds );
std::string StrToLower( const std::string& Value );
std::string UrlDecode( const std::string& Value );
std::string Base64Decode( const std::string& encoded_string );

#endif /*UTILS_H_*/
