#ifndef IDOWNLOADFACTORY_H_
#define IDOWNLOADFACTORY_H_

#include <string>
#include "../util/yadmaptr.h"
#include "idownload.h"

class IDownloadFactory
{
public:
    virtual ~IDownloadFactory(){};
    virtual yadmaptr<IDownload> CreateDownload( int DownloadId ) = 0;
};

#endif /*IDOWNLOADFACTORY_H_*/
