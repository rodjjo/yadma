var yadmaacc = new Object();

yadmaacc.AccountsHeaders = new Array(
										{ Name : "Id",      		Id : "1" },
										{ Name : "Url",      		Id : "2" },
										{ Name : "UserName",     	Id : "3" },
										{ Name : "Password",     	Id : "4" },
										{ Name : "LoginUrl",    	Id : "5" },
										{ Name : "LoginUserVar",    Id : "6" },
										{ Name : "LoginPassVar",    Id : "7" },
										{ Name : "LoginPrefix",     Id : "8" },
										{ Name : "LoginAdditional", Id : "9" }
									);
//--------------------------------------------------------------------------------------						

yadmaacc.AccIndex = 0;
yadmaacc.Items    = new Array();

//--------------------------------------------------------------------------------------
yadmaacc.getNewAccountObj = function()
{
return {
			Id:0,
			Url: "",
			UserName: "",
			Password: "",
			LoginUrl: "",
			LoginUserVar: "",
			LoginPassVar: "",
			LoginPrefix: "",
			LoginAdditional: "",
			_Modified: false
		};
}

//--------------------------------------------------------------------------------------
yadmaacc.newAccount = function()
{
	yadmaacc.AccIndex = yadmaacc.Items.length - 1;
	yadmaacc.showAccount();
}

//--------------------------------------------------------------------------------------
yadmaacc.deleteAccount = function()
{
	if ( yadmaacc.AccIndex + 1< yadmaacc.Items.length  )
	{
		if ( confirm( "delete account ?" ) )
		{
			yadma.get( "/yadma/accounts.txt?delete=1&id=" + yadmaacc.Items[ yadmaacc.AccIndex ].Id );
			yadmaacc.AccIndex--;
			yadmaacc.rebuildIt();
		}
	}
}

//--------------------------------------------------------------------------------------
yadmaacc.saveAccount = function()
{
	if ( ( yadmaacc.AccIndex < 0 ) || ( yadmaacc.AccIndex > yadmaacc.Items.length ) )
	{
		return;
	}
	
	var PostData = "url=" + urlencode( $( "#accServer" ).val() ) +
				   "&username=" + urlencode( $( "#accUserName" ).val() ) +
				   "&password=" + urlencode( $( "#accPassword" ).val() ) +
				   "&loginurl=" + urlencode( $( "#accLoginUrl" ).val() ) +
				   "&uservar=" + urlencode( $( "#accUserVar" ).val() ) +
				   "&passvar=" + urlencode( $( "#accPassVar" ).val() ) +
				   "&additional=" +  urlencode( $( "#accAddVars" ).val() ) +
				   "&prefix=" + urlencode( $( "#accPluginUrl" ).val() ); 
				   
	if ( yadmaacc.Items[ yadmaacc.AccIndex ].Id > 0 )
	{
		PostData = "id=" + yadmaacc.Items[ yadmaacc.AccIndex ].Id + "&" + PostData;
	}
	
	var result = yadma.post( "/yadma/accounts.txt", PostData );
	
	if ( result.Success )
	{
		alert( "Account saved with success.");
		yadmaacc.rebuildIt();
	}else
	{
		alert( "Account not saved" );
	}
}

//--------------------------------------------------------------------------------------
yadmaacc.previousAccount = function()
{
	if ( yadmaacc.AccIndex < 1 )
	{
		return;
	}
	yadmaacc.AccIndex--;
	yadmaacc.showAccount();
}

//--------------------------------------------------------------------------------------
yadmaacc.nextAccount = function()
{
	if ( yadmaacc.AccIndex + 1 >= yadmaacc.Items.length )
	{
		return;
	}
	yadmaacc.AccIndex++;
	yadmaacc.showAccount();
}

//--------------------------------------------------------------------------------------
yadmaacc.showAccount  = function()
{
	if ( yadmaacc.AccIndex > yadmaacc.Items.length )
	{
		yadmaacc.AccIndex = yadmaacc.Items.length - 1;
	}
	
	else if ( yadmaacc.AccIndex < 0 )
	{
		yadmaacc.AccIndex = 0;
	}
	
	if ( (yadmaacc.AccIndex >= 0) && (yadmaacc.AccIndex < yadmaacc.Items.length) )
	{
		$("#d_content").html(  "<br><div align=center><table width=760px>"
							+ "<tr><td align=right width=300px>Server:</td><td><input type=edit id='accServer' style='width:100%'></td></tr>"
							+ "<tr><td align=right width=300px>UserName:</td><td><input type=edit id='accUserName' style='width:100%'></td></tr>"
							+ "<tr><td align=right width=300px>Password:</td><td><input type=password id='accPassword' style='width:100%'></td></tr>"
							+ "<tr><td align=right width=300px>Login Url:</td><td><input type=edit id='accLoginUrl' style='width:100%'></td></tr>"
							+ "<tr><td align=right width=300px>Login User Var:</td><td><input type=edit id='accUserVar' style='width:100%'></td></tr>"
							+ "<tr><td align=right width=300px>Login Pass Var:</td><td><input type=edit id='accPassVar' style='width:100%'></td></tr>"
							+ "<tr><td align=right width=300px>Addtional Vars:</td><td><input type=edit id='accAddVars' style='width:100%'></td></tr>"
							+ "<tr><td align=right width=300px>Plugin Url:</td><td><input type=edit id='accPluginUrl' style='width:100%'></td></tr></table>"
							+ "<div style='width:760px' align=right>"
								+ ( yadmaacc.AccIndex + 1 ) + " Of " + ( yadmaacc.Items.length - 1 ) +"&nbsp;&nbsp;"
								+"<input type=button value='Delete' onclick='yadmaacc.deleteAccount()'>&nbsp;&nbsp;&nbsp;"
								+"<input type=button value='New' onclick='yadmaacc.newAccount()'>&nbsp;&nbsp;"
								+"<input type=button value='Save' onclick='yadmaacc.saveAccount()'>&nbsp;&nbsp;"
								+"<input type=button value='<<<<' onclick='yadmaacc.previousAccount()'>"
								+"<input type=button value='>>>>' onclick='yadmaacc.nextAccount()'>"
						+"</div></div> " ) ;

		
		var Item =  yadmaacc.Items[ yadmaacc.AccIndex ];
		
		$( "#accServer" ).val( Item.Url );
		$( "#accUserName" ).val( Item.UserName );
		$( "#accPassword" ).val( Item.Password );
		$( "#accLoginUrl" ).val( Item.LoginUrl );
		$( "#accUserVar" ).val( Item.LoginUserVar );
		$( "#accPassVar" ).val( Item.LoginPassVar );
		$( "#accAddVars" ).val( Item.LoginAdditional );
		$( "#accPluginUrl" ).val( Item.LoginPrefix );
		
	}
}

//--------------------------------------------------------------------------------------
yadmaacc.rebuildIt = function()
{
	var data = yadma.get( "/yadma/accounts.txt" );

	if( data.Success )
	{
		yadmaacc.Items = yadma.parseTableData( yadmaacc.AccountsHeaders, data.Data );
		yadmaacc.Items.push( yadmaacc.getNewAccountObj() );
		yadmaacc.showAccount();
	}
	else
	{
		$("#d_content").html( "<br><b>connection error</b>" );
	}
}

//--------------------------------------------------------------------------------------
yadmaacc.showWindow = function()
{
	$("#d_content").html( "" );
	
	yadmaacc.AccIndex = 0;
	
	yadmaacc.rebuildIt();
}
