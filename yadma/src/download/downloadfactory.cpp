#include "download.h"
#include "downloadfactory.h"

//---------------------------------------------------------------------------
CDownloadFactory::CDownloadFactory( yadmaptr<ISettings> Settings, yadmaptr<IDataBase> DataBase, yadmaptr<IAutenticator> Autenticator )
{
    m_Settings = Settings;    
    m_DataBase = DataBase;
    m_Autenticator = Autenticator;
}

//---------------------------------------------------------------------------
CDownloadFactory::~CDownloadFactory()
{
    
}
    
//---------------------------------------------------------------------------
yadmaptr<IDownload> CDownloadFactory::CreateDownload( int DownloadId )
{
    return yadmaptr<IDownload>( new CDownload( DownloadId, m_Settings, m_DataBase, m_Autenticator ) ); 
}
