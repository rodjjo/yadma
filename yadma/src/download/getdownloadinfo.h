#ifndef GETDOWNLOAD_INFO_H
#define GETDOWNLOAD_INFO_H

#include <string>

bool GetDownloadInfo( const std::string& Url, std::string& Cookie, std::string& Final, std::string& FileName, size_t& FileSize, bool Prefixed );

#endif
