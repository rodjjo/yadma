#ifndef PATHUTIL_H_
#define PATHUTIL_H_

#include <string>

std::string GetAppPath();
std::string GetAppDirectory();
bool MakeDirectory( const std::string& Directory );
bool DeleteDirectory( const std::string& Directory );
std::string MakeExclusiveName( const std::string& FileName );
bool FileExists( const std::string& FileName );

#endif /*PATHUTIL_H_*/
