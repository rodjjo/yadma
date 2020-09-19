#include "pagedefines.h"
#include "toppage.h"

//-----------------------------------------------------------------------------------------------------
CTopPage::CTopPage()
{
	
}

//-----------------------------------------------------------------------------------------------------
CTopPage::~CTopPage()
{
	
}

//-----------------------------------------------------------------------------------------------------
void CTopPage::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	mg_printf( conn, "%s", PAGE_HEADER PAGE_HTML_BODY );
	//mg_printf( conn, "%s", "<table width=100% height=100% bgcolor=#191919 style='background-image:url(\"/img/bar.bmp\"); background-repeat: repeat-x '>" );
	mg_printf( conn, "%s", "<table width=100% height=100% bgcolor=#3193939>" );
	mg_printf( conn, "%s", "<td valign='center'>" );
	mg_printf( conn, "%s", "<a href='/yadma/downloads.html' target=frame2>Downloads</a>&nbsp;&nbsp;" );
	mg_printf( conn, "%s", "<a href='/extension/index.html' target=frame2>Extension</a>&nbsp;&nbsp;" );
	/*mg_printf( conn, "%s", "<a href='/yadma/filemanager.html' target=frame2>File Manager</a>&nbsp;&nbsp;" );
	mg_printf( conn, "%s", "<a href='/yadma/mediaplayer.html' target=frame2>Media Player</a>&nbsp;&nbsp;" );*/
	mg_printf( conn, "%s", "<a href='/yadma/settings.html' target=frame2>Settings</a>&nbsp;&nbsp;" );
	mg_printf( conn, "%s", "<a href='/yadma/about.html' target=frame2>About</a>&nbsp;&nbsp;" );
	mg_printf( conn, "%s", "</td>" );
	mg_printf( conn, "%s", "<td width='140px' valing='center' align='center'>" );
	mg_printf( conn, "%s", "<a href='/stop.html' target='_parent' >Parar Servidor</a>" );
	mg_printf( conn, "%s", "</td>" );
	mg_printf( conn, "%s", PAGE_HTML_BOTTOM );
}
