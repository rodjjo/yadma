#ifndef DOWNLOADFACTORY_H_
#define DOWNLOADFACTORY_H_

#include "../util/yadmaptr.h"
#include "../autenticator/iautenticator.h"
#include "idownloadfactory.h"
#include "../settings/isettings.h"
#include "../database/idatabase.h"

class CDownloadFactory: public IDownloadFactory
{
public:
	CDownloadFactory( yadmaptr<ISettings> Settings, yadmaptr<IDataBase> DataBase, yadmaptr<IAutenticator> Autenticator );
	virtual ~CDownloadFactory();
	yadmaptr<IDownload> CreateDownload( int DownloadId );
	
private:
	yadmaptr<IAutenticator> 	m_Autenticator;
	yadmaptr<ISettings> 		m_Settings;
	yadmaptr<IDataBase>  		m_DataBase;
};

#endif /*DOWNLOADFACTORY_H_*/
