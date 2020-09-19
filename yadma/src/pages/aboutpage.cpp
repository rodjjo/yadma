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
void CAboutPage::HandleRequest( struct mg_connection* conn, const struct mg_request_info* request_info )
{
	mg_printf( conn, "%s", PAGE_HEADER );
	mg_printf( conn, "%s", "<div align=center>2011<br><strong>Rodrigo A. Araujo<br></strong>drigofast@gmail.com</div>" );
	mg_printf( conn, "%s", "the project never stops<br>" );
	mg_printf( conn, "%s", "<form action=\"https://www.paypal.com/cgi-bin/webscr\" method=\"post\">" );
	mg_printf( conn, "%s", "<input type=\"hidden\" name=\"cmd\" value=\"_s-xclick\">" );
	mg_printf( conn, "%s", "<input type=\"hidden\" name=\"encrypted\" value=\"-----BEGIN PKCS7-----MIIHPwYJKoZIhvcNAQcEo" );
	mg_printf( conn, "%s", "IIHMDCCBywCAQExggEwMIIBLAIBADCBlDCBjjELMAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQ" );
	mg_printf( conn, "%s", "wEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQKbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQ" );
	mg_printf( conn, "%s", "HBheXBhbC5jb20CAQAwDQYJKoZIhvcNAQEBBQAEgYBFLys849dm385orgGoCyASSvLzCN3oJM4OVCtZUPWs21nmfhXcGgnk+M4reQiDNq" );
	mg_printf( conn, "%s", "GGzPG3gKDRKhHW6jQboQNP9uEunk20n4RRQ3drnPv4QpSLhQ9sGz8WK6QrQ5O/hmx94725u91eEkeaOGYzq3Rqol5vIUEDkzj1AMLIkX" );
	mg_printf( conn, "%s", "ZbczELMAkGBSsOAwIaBQAwgbwGCSqGSIb3DQEHATAUBggqhkiG9w0DBwQIyOrIZLTWaAKAgZjqKcKXn+Q4jwVkX5+I2rtsKg3mgaKCO8eO" );
	mg_printf( conn, "%s", "5E5Zr8Z+3fUh+aaGL86T+48klbDMqBhgnwti/d2FDSg/9WLb8XRjCtAaVUllYibNxMMYaRYslEgeu7ecu7+W/1iFsr67O7Cou8Y8L7f5nHK" );
	mg_printf( conn, "%s", "prjOkkc1oinZO4TlIwXbyN+mZL+TnGFkIxRLcUjl4xS1JBoA8R1y9ZT1O0SIe+KCCA4cwggODMIIC7KADAgECAgEAMA0GCSqGSIb3DQEBBQU" );
	mg_printf( conn, "%s", "AMIGOMQswCQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxFDASBgNVBAoTC1BheVBhbCBJbmMuMRMwEQYD" );
	mg_printf( conn, "%s", "VQQLFApsaXZlX2NlcnRzMREwDwYDVQQDFAhsaXZlX2FwaTEcMBoGCSqGSIb3DQEJARYNcmVAcGF5cGFsLmNvbTAeFw0wNDAyMTMxMDEzMTVaF" );
	mg_printf( conn, "%s", "w0zNTAyMTMxMDEzMTVaMIGOMQswCQYDVQQGEwJVUzELMAkGA1UECBMCQ0ExFjAUBgNVBAcTDU1vdW50YWluIFZpZXcxFDASBgNVBAoTC1Bhe" );
	mg_printf( conn, "%s", "VBhbCBJbmMuMRMwEQYDVQQLFApsaXZlX2NlcnRzMREwDwYDVQQDFAhsaXZlX2FwaTEcMBoGCSqGSIb3DQEJARYNcmVAcGF5cGFsLmNvbTCBnz" );
	mg_printf( conn, "%s", "ANBgkqhkiG9w0BAQEFAAOBjQAwgYkCgYEAwUdO3fxEzEtcnI7ZKZL412XvZPugoni7i7D7prCe0AtaHTc97CYgm7NsAtJyxNLixmhLV8pyIEa" );
	mg_printf( conn, "%s", "iHXWAh8fPKW+R017+EmXrr9EaquPmsVvTywAAE1PMNOKqo2kl4Gxiz9zZqIajOm1fZGWcGS0f5JQ2kBqNbvbg2/Za+GJ/qwUCAwEAAaOB7j" );
	mg_printf( conn, "%s", "CB6zAdBgNVHQ4EFgQUlp98u8ZvF71ZP1LXChvsENZklGswgbsGA1UdIwSBszCBsIAUlp98u8ZvF71ZP1LXChvsENZklGuhgZSkgZEwgY4xC" );
	mg_printf( conn, "%s", "zAJBgNVBAYTAlVTMQswCQYDVQQIEwJDQTEWMBQGA1UEBxMNTW91bnRhaW4gVmlldzEUMBIGA1UEChMLUGF5UGFsIEluYy4xEzARBgNVBAsU" );
	mg_printf( conn, "%s", "CmxpdmVfY2VydHMxETAPBgNVBAMUCGxpdmVfYXBpMRwwGgYJKoZIhvcNAQkBFg1yZUBwYXlwYWwuY29tggEAMAwGA1UdEwQFMAMBAf8wDQY" );
	mg_printf( conn, "%s", "JKoZIhvcNAQEFBQADgYEAgV86VpqAWuXvX6Oro4qJ1tYVIT5DgWpE692Ag422H7yRIr/9j/iKG4Thia/Oflx4TdL+IFJBAyPK9v6zZNZtBgP" );
	mg_printf( conn, "%s", "BynXb048hsP16l2vi0k5Q2JKiPDsEfBhGI+HnxLXEaUWAcVfCsQFvd2A1sxRr67ip5y2wwBelUecP3AjJ+YcxggGaMIIBlgIBATCBlDCBjjE" );
	mg_printf( conn, "%s", "LMAkGA1UEBhMCVVMxCzAJBgNVBAgTAkNBMRYwFAYDVQQHEw1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKEwtQYXlQYWwgSW5jLjETMBEGA1UECxQ" );
	mg_printf( conn, "%s", "KbGl2ZV9jZXJ0czERMA8GA1UEAxQIbGl2ZV9hcGkxHDAaBgkqhkiG9w0BCQEWDXJlQHBheXBhbC5jb20CAQAwCQYFKw4DAhoFAKBdMBgGCSq" );
	mg_printf( conn, "%s", "GSIb3DQEJAzELBgkqhkiG9w0BBwEwHAYJKoZIhvcNAQkFMQ8XDTExMDEwMjE1MzczOFowIwYJKoZIhvcNAQkEMRYEFBsLGRM+HrN9ykhDefn" );
	mg_printf( conn, "%s", "AirZm/IdMMA0GCSqGSIb3DQEBAQUABIGArsJLF5E8bc9eETb6CiS1Uoco8f1u//Ihb6WhGmyC8g+pki3ayQx9N4t1mFqRXIChtZ45sOdovG+" );
	mg_printf( conn, "%s", "dCp0EeddcjlCMDFA8hHtND7gx05sV2DM4EsbL7hHZsp/uXBzEz+UwHdIl+7WkOqOogs2O4KIuqJ+vOG+xG6rHcEpjW/Gu0YA=-----END PKCS7-----\">" );
	mg_printf( conn, "%s", "<input type=\"image\" src=\"https://www.paypal.com/en_US/i/btn/btn_donateCC_LG.gif\" border=\"0\" name=\"submit\" " );
	mg_printf( conn, "%s", "alt=\"PayPal - The safer, easier way to pay online!\">" );
	mg_printf( conn, "%s", "<img alt=\"\" border=\"0\" src=\"https://www.paypal.com/pt_BR/i/scr/pixel.gif\" width=\"1\" height=\"1\">" );
	mg_printf( conn, "%s", "</form>" );
}
