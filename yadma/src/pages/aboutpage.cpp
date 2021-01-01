#include <string.h>

#include "pagedefines.h"
#include "../util/utils.h"
#include "aboutpage.h"

//-----------------------------------------------------------------------------------------------------
CAboutPage::CAboutPage()
{
}

//-----------------------------------------------------------------------------------------------------
CAboutPage::~CAboutPage()
{
}

//-----------------------------------------------------------------------------------------------------
std::pair<int, std::string> CAboutPage::HandleRequest( void *ev_data )
{
    return std::make_pair(200, std::string("rodrigo.araujo"));
}
