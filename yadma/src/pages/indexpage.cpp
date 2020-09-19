#include "pagedefines.h"
#include "indexpage.h"

//-----------------------------------------------------------------------------------------------------
CIndexPage::CIndexPage()
{
	
}

//-----------------------------------------------------------------------------------------------------
CIndexPage::~CIndexPage()
{
	
}

//-----------------------------------------------------------------------------------------------------
void CIndexPage::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	mg_printf( conn, "%s", PAGE_HEADER PAGE_HTML_HEADER 
				"<html><head><title>yadma (yes another download manager</title>"
				"<link rel=\"stylesheet\" type=\"text/css\" href=\"/styles/yadma.css\">"
				"</head>"
				"<noscript>	JavaScript Needed. </noscript>"
				"</head>"
				"<body><div id=\"Div_Window\" class=\"Div_Window\"></div>"
				"<div align=center>"
				"<table class=\"global_table\">"
				"<tr>" 
					"<td class=\"top_bar\" ><div id=\"Div_Main\"></div></td>"
				"</tr>"
				"<tr>" 
					"<td class=\"base_bar\" > <div id=\"Div_Base\"> </div></td>"
				"</tr>"
				"<tr>"
					"<td class=\"content_bar\" valign=\"top\">"
				"<div id=\"Div_Content\"></div> </td>"
				"</tr>"
				"</table>"
				"</div>"
				"</body>"
					"<script language=\"JavaScript1.2\" src=\"/scripts/yadma.js\"></script>"
				"</html>" );
}
