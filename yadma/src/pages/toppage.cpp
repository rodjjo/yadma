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
std::pair<int, std::string> CTopPage::HandleRequest( void *ev_data )
{
    return std::make_pair(200,
        PAGE_HTML_BODY
        "<table width=100% height=100% bgcolor=#3193939>"
        "<td valign='center'>"
        "<a href='/yadma/downloads.html' target=frame2>Downloads</a>&nbsp;&nbsp;"
        "<a href='/extension/index.html' target=frame2>Extension</a>&nbsp;&nbsp;"
        "<a href='/yadma/settings.html' target=frame2>Settings</a>&nbsp;&nbsp;"
        "<a href='/yadma/about.html' target=frame2>About</a>&nbsp;&nbsp;"
        "</td>"
        "<td width='140px' valing='center' align='center'>"
        "<a href='/stop.html' target='_parent' >Parar Servidor</a>"
        "</td>"
        PAGE_HTML_BOTTOM);
}
