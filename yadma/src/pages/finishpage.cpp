#include <stdio.h>
#include "pagedefines.h"
#include "finishpage.h"

//-----------------------------------------------------------------------------------------------------
CFinishPage::CFinishPage( yadmaptr<IDownloaderManager> DownloadManager )
{
    m_DownloadManager = DownloadManager;
}

//-----------------------------------------------------------------------------------------------------
CFinishPage::~CFinishPage()
{

}

//-----------------------------------------------------------------------------------------------------
std::pair<int, std::string> CFinishPage::HandleRequest( void *ev_data )
{
    return std::make_pair(200, PAGE_HTML_BODY "<br><br><br><center>yadma was finished...</center>");
    m_DownloadManager->Stop();
}
