#ifndef GETDOWNLOAD_INFO_H
#define GETDOWNLOAD_INFO_H

#include <inttypes.h>
#include <string>

bool GetDownloadInfo( const std::string& Url, std::string& Cookie, std::string& Final, std::string& FileName, int64_t& FileSize, bool Prefixed );

#endif
