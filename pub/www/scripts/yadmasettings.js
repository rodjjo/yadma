var yadmasettings = new Object();

//--------------------------------------------------------------------------------------
yadmasettings.SettingsHeaders = new Array
					    (
							{ Name : "ServerName",      Id : "1" },
							{ Name : "ServerPort",      Id : "2" },
							{ Name : "DownloadDir",     Id : "3" },
							{ Name : "DownloadMax",     Id : "4" },
							{ Name : "ProxyAddress",    Id : "5" },
							{ Name : "ProxPort",        Id : "6" },
							{ Name : "ProxActivate",    Id : "7" },
							{ Name : "ProxHttps",      	Id : "8" }
						);					
						
//--------------------------------------------------------------------------------------
yadmasettings.showSettings = function( Settings )
{
	$("#d_content").html( "<br><div align=center><table width=760px>" +
      "<tr><td align=right width=300px>Web Server:</td><td><input type=text style='width:100%' name='settingsWebServerName' id='settingsWebServerName' value='" + Settings.ServerName + "'>" + 
      "</td><tr><td align=right>Port:</td><td><input type=text style='width:100%' name='settingsWebServerPort' id='settingsWebServerPort' value='" + Settings.ServerPort + "'>" +   
      "</td><tr><td align=right>Download Directory:</td><td><input type=text style='width:100%' name='settingsDownloadDir' id='settingsDownloadDir' value='" + Settings.DownloadDir + "'>" +
      "</td><tr><td align=right>Max Active Downloads:</td><td><input type=text style='width:100%' name='settingsDownloadMax' id='settingsDownloadMax' value='" + Settings.DownloadMax + "'>" +
      "</td><tr><td align=right>&nbsp;</td><td align=right><input type=button value='Save' style='width:120px' onclick='yadmasettings.saveSettings()'></td></table></div>" );
}

//--------------------------------------------------------------------------------------
yadmasettings.rebuildIt = function()
{
	var data = yadma.get( "/yadma/settings.txt" );
	if( data.Success )
	{
		var Settings = yadma.parseTableData( yadmasettings.SettingsHeaders, data.Data );
		if ( Settings.length == 1 )
		{
			yadmasettings.showSettings( Settings[0] );
		}
	}
	else
	{
		$("#d_content").html( "<br><b>connection error</b>" );
	}
}

//--------------------------------------------------------------------------------------
yadmasettings.showWindow = function()
{
	$("#d_content").html( "" );
	
	yadmasettings.rebuildIt();
}

//--------------------------------------------------------------------------------------
yadmasettings.saveSettings = function()
{
	var ServerName          = document.getElementById( "settingsWebServerName" ).value; 
	var ServerPort          = document.getElementById( "settingsWebServerPort" ).value; 
	var DownloadDirectory   = document.getElementById( "settingsDownloadDir" ).value;
	var DownloadMax         = document.getElementById( "settingsDownloadMax" ).value;
	//future support
	var ProxyAddr           = ""; 
	var ProxyPort           = "8080";
	var ProxyHttps          = "n";
    var ProxActivate        = "n";
   
    if ( yadma.post( "/yadma/settings.txt", "server_name=" + urlencode( ServerName ) + "&server_port=" + urlencode( ServerPort ) 
               + "&down_dir=" + urlencode( DownloadDirectory ) + "&down_max=" + urlencode( DownloadMax )
               + "&proxy_addr=" + urlencode( ProxyAddr ) + "&proxy_port=" + urlencode( ProxyPort )  
               + "&proxy_activate=" + ProxActivate + "&proxy_https=" + ProxyHttps ).Success )
	{
		alert( "The settings was saved with success!" );
	}
			   
	yadmasettings.rebuildIt();
}
