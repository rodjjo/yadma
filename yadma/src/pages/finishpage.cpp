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
void CFinishPage::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	mg_printf( conn, "%s", PAGE_HEADER PAGE_HTML_BODY ); 
	mg_printf( conn, "%s", "<br><br><br><center>yadma was finished...</center>" );
	mg_printf( conn, "%s", PAGE_HTML_BOTTOM );
	
	m_DownloadManager->Stop();
}

