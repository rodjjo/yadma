// yadma download manager javascript interface builder
// Rodrigo A. Araujo
// drigofast@gmail.com
//------------------------------------------------------------------------------------------

var yadma                 = new Object();

yadma.Version             = "1.0.0";
yadma.RefreshInterval     = 1500;
yadma.Started             = false;
yadma.ListVersion         = -1;
yadma.DownCurrentTab      = "running";
yadma.DownPreviousTab     = { Filter: "", Data: new Array() };

//------------------------------------------------------------------------------------------
yadma.Dimensions          = new Object();
yadma.Dimensions.MinWidth = 1024;
yadma.Dimensions.MaxWidth = 1650;
yadma.Dimensions.Width    = 1024;
yadma.Dimensions.Height   = 800;
//------------------------------------------------------------------------------------------
yadma.Themes 		        = { Id : 1, Data : null };
yadma.DownloadList 	     = new Array();

//------------------------------------------------------------------------------------------
yadma.DownloadHeaders = new Array
    (
        { Name : "Id",              Id : "1" },
        { Name : "Status",          Id : "2" },
        { Name : "Url",             Id : "3" },
        { Name : "Redirected",      Id : "4" },
        { Name : "FileName",        Id : "5" },
        { Name : "Site",            Id : "6" },
        { Name : "FileSize",        Id : "7" },
        { Name : "Downloaded",      Id : "8" },
        { Name : "Speed",           Id : "9" },
        { Name : "Folder",          Id : "10" }
    );


//------------------------------------------------------------------------------------------
yadma.parseTableData = function( Reference, Text )
{
 	var RawData = Text.split( "\n" );
	var RetVal = new Array();
	var Row;
	var nObject = new Object();
	var b = true;

	for ( var i = 0; i < RawData.length; i++ )
	{
	    var c = RawData[i].indexOf(':');

		Row = new Array( RawData[i].substr( 0, c ), RawData[i].substr( c + 1 ) );

		for ( var i2 = 0; i2 < Reference.length; i2++ )
		{
			if ( Reference[i2].Id == Row[0] )
			{
				if ( i2 == 0 )
				{
					if ( i > 0 )
					{
						RetVal.push( nObject );
					}
					nObject = new Object();
				}

            nObject[Reference[i2].Name] = Row[1];

				break;
			}
		}
	}

    if ( RawData.length > 0 )
	{
		RetVal.push( nObject );
	}

	return RetVal;
}

//------------------------------------------------------------------------------------------
yadma.defineLimits = function()
{
    yadma.Dimensions.Width = screen.width - 100;

    if ( yadma.Dimensions.Width > yadma.Dimensions.MaxWidth )
    {
        yadma.Dimensions.Width = yadma.Dimensions.MaxWidth;
    }
    else if ( yadma.Dimensions.Width < yadma.Dimensions.MinWidth )
    {
        yadma.Dimensions.Width = yadma.Dimensions.MinWidth;
    }

}

//------------------------------------------------------------------------------------------
yadma.getComparator = function()
{
   var Comparator = function( Row, Action ){  };

   if (  yadma.DownCurrentTab == "active" )
   {
      Comparator = function( Row, Action ) { if( Row.Status < 2 ) Action( Row ); };
   }
   else if ( yadma.DownCurrentTab == "running" )
   {
      Comparator = function( Row, Action ) { if ( Row.Status == 0 ) Action( Row ); };
   }
   else if ( yadma.DownCurrentTab == "waiting" )
   {
      Comparator = function( Row, Action ) { if ( Row.Status == 1 ) Action( Row ); };
   }
   else if ( yadma.DownCurrentTab == "stopped" )
   {
      Comparator = function( Row, Action ) { if ( Row.Status == 2 ) Action( Row ); };
   }
   else if ( yadma.DownCurrentTab == "finished" )
   {
      Comparator = function( Row, Action ) { if ( Row.Status == 4 ) Action( Row ); };
   }

   return Comparator;
}

//------------------------------------------------------------------------------------------
yadma.countVisibleRows = function()
{
   var Comparator = yadma.getComparator();

   var RetVal = 0;

   for ( var i = 0; i <  yadma.DownloadList.length; i++ )
   {
      Comparator(  yadma.DownloadList[i] , function( Row ) { RetVal++; } );
   }

   return RetVal;
}

//------------------------------------------------------------------------------------------
yadma.stretchContentArea = function()
{
	$( '#dl_area' ).css( 'height', ( $(window).height() - 120 )  + 'px' );
}

//-----------------------------------------------------------------------------------
yadma.getLines = function()
{
  var RetVal = { Header : "", FirstLine : "", Sumary : "" };

   if (  ( yadma.DownCurrentTab == "active" ) || ( yadma.DownCurrentTab == "running" ) )
   {
      RetVal.Header = "<th width='28%' class='tbl_down_title'>File</th><th width='12%' class='tbl_down_title'>Size</th><th width='12%' class='tbl_down_title'>Downloaded</th>"
               + "<th width='12%' class='tbl_down_title'>Remain</th><th width='12%' class='tbl_down_title'>Speed</th>"
               + "<th width='12%' class='tbl_down_title'>Time</th><th width='12%' class='tbl_down_title'>Action</th>";

      RetVal.FirstLine = "<td class='tbl_cell_txt' width='28%'>&nbsp;</td><td class='tbl_cell' width='12%'>&nbsp;</td><td class='tbl_cell' width='12%'>&nbsp;</td>"
               + "<td class='tbl_cell' width='12%'>&nbsp;</td><td class='tbl_cell' width='12%'>&nbsp;</td>"
               + "<td class='tbl_cell' width='12%'>&nbsp;</td><td class='tbl_cell' width='12%'>&nbsp;</td>";

	  RetVal.Sumary = "<td class='tbl_down_sumary' width='28%'>&nbsp;</td><td class='tbl_down_sumary' width='12%'>&nbsp;</td><td class='tbl_down_sumary' width='12%'>&nbsp;</td>"
               + "<td class='tbl_down_sumary' width='12%'>&nbsp;</td><td class='tbl_down_sumary' width='12%'>&nbsp;</td>"
               + "<td class='tbl_down_sumary' width='12%'>&nbsp;</td><td class='tbl_down_sumary' width='12%'>&nbsp;</td>";
   }
   else if ( ( yadma.DownCurrentTab == "waiting" ) || ( yadma.DownCurrentTab == "stopped")  )
   {
      RetVal.Header = "<th width='40%' class='tbl_down_title'>File</th><th width='15%' class='tbl_down_title'>Size</th><th width='15%' class='tbl_down_title'>Downloaded</th>"
               + "<th width='15%' class='tbl_down_title'>Remain</th><th width='15%' class='tbl_down_title'>Action</th>";

      RetVal.FirstLine = "<td class='tbl_cell_txt' width='40%'>&nbsp;</td><td class='tbl_cell' width='15%'>&nbsp;</td><td class='tbl_cell' width='15%'>&nbsp;</td>"
               + "<td class='tbl_cell' width='15%'>&nbsp;</td><td class='tbl_cell' width='15%'>&nbsp;</td>";

	  RetVal.Sumary = "<td class='tbl_down_sumary' width='40%'>&nbsp;</td><td class='tbl_down_sumary' width='15%'>&nbsp;</td><td class='tbl_down_sumary' width='15%'>&nbsp;</td>"
               + "<td class='tbl_down_sumary' width='15%'>&nbsp;</td><td class='tbl_down_sumary' width='15%'>&nbsp;</td>";
   }
   else if ( yadma.DownCurrentTab == "finished" )
   {
      RetVal.Header = "<th width='66%' class='tbl_down_title'>File</th><th width='16%' class='tbl_down_title'>Downloaded</th><th width='15%' class='tbl_down_title'>Action</th>";

      RetVal.FirstLine = "<td  class='tbl_cell_txt' width='66%'>&nbsp;</td><td class='tbl_cell' width='16%'>&nbsp;</td><td class='tbl_cell' width='15%'>&nbsp;</td>";

	  RetVal.Sumary = "<td  class='tbl_down_sumary' width='66%'>&nbsp;</td><td class='tbl_down_sumary' width='16%'>&nbsp;</td><td class='tbl_down_sumary' width='15%'>&nbsp;</td>";
   }

   return RetVal;
}

//------------------------------------------------------------------------------------------
yadma.InitializeDownTable = function()
{
   var Lines  = yadma.getLines();

   var d_ContentPanel = $("#d_content");

   d_ContentPanel.css( "height", "100%" );

   d_ContentPanel.html(
            "<table style='border-collapse: collapse' width=" + yadma.Dimensions.Width + "px height=100%>"
               +"<tbody>"
                  +"<tr>"
                      +"<td height=30px>"
                          +"<table width=" + yadma.Dimensions.Width + "px id='tbl_downs_t' class='tbl_downs_class'>"
                             +"<thead class='tbl_down_title'>" + Lines.Header
                             +"</thead>"
                          +"</table>"
                      +"</td>"
                  +"</tr>"
                  +"<tr>"
                      +"<td id='dl_area' valign=top>"
                         +"<div style='height:100%;overflow-x:hidden;overflow-y:auto;margin:0;width=" + (yadma.Dimensions.Width + 25) + "px' >"
                           +"<table width=" + yadma.Dimensions.Width + "px id='tbl_downs' class='tbl_downs_class' >"
                             +"<tbody>"
                                   +"<tr class='tbl_down_2' >"
                                      + Lines.FirstLine
                                   +"</tr>"
                              +"</tbody>"
                          +"</table>"
                        +"</div>"
                      +"</td>"
                  +"</tr>"
                  +"<tr>"
                      +"<td height=30px >"
                         +"<table height=30px width=" + yadma.Dimensions.Width + "px id='tbl_downs_s' class='tbl_downs_class' >"
                           +"<tbody>"
                              +"<tr id='tbl_down_sumary' class='tbl_down_sumary' >"
                                  + Lines.Sumary
                               +"</tr>"
                            +"</tbody>"
                          +"</table>"
                      +"</td>"
                  +"</tr>"
               +"</tbody>"
            +"</table>"  );

	yadma.stretchContentArea();
}

//------------------------------------------------------------------------------------------
yadma.countVisibleCols = function()
{
   if (  ( yadma.DownCurrentTab == "active" ) || ( yadma.DownCurrentTab == "running" ) )
   {
      return 7;
   }
   else if ( ( yadma.DownCurrentTab == "waiting" ) || ( yadma.DownCurrentTab == "stopped")  )
   {
      return 5;
   }
   else if ( yadma.DownCurrentTab == "finished" )
   {
      return 3;
   }

   return 0;
}

//------------------------------------------------------------------------------------------
yadma.stopDownload = function( Id )
{
   if ( confirm("do you want to stop download ?") )
	{
	   yadma.get( "/yadma/downloads.txt?stop=" + Id );
      yadma.ListVersion         = -1;
   }
}

//------------------------------------------------------------------------------------------
yadma.resumeDownload = function( Id )
{
   yadma.get( "/yadma/downloads.txt?resume=" + Id );
   yadma.ListVersion         = -1;
}

//------------------------------------------------------------------------------------------
yadma.deleteDownload = function( Id )
{
   if ( confirm("do you want to delete download ?") )
	{
		yadma.get( "/yadma/downloads.txt?delete=" + Id );
		yadma.ListVersion         = -1;
	}
}

//------------------------------------------------------------------------------------------
yadma.clearDownload = function( Id )
{
   if ( confirm("Clear command does not delete the file.\n Do you wanna to remove this download from list ?") )
	{
		yadma.get( "/yadma/downloads.txt?clear=" + Id );
		yadma.ListVersion         = -1;
	}
}

//------------------------------------------------------------------------------------------
yadma.getRowAction = function( RowData )
{
   if( RowData.Status < 2 )
   {
      return "<input style='height:100%' type=button value='Stop' onclick=yadma.stopDownload(" + RowData.Id + ")>";
   }
   if ( RowData.Status == 2 )
   {
      return "<input style='height:100%' type=button value='Resume' onclick=yadma.resumeDownload(" + RowData.Id + ")>"
			+"<input style='height:100%' type=button value='Delete' onclick=yadma.deleteDownload(" + RowData.Id + ")>";
   }

   if ( RowData.Status == 4 )
   {
      return "<input style='height:100%' type=button value='Clear' onclick=yadma.clearDownload(" + RowData.Id + ")>";
   }

   return  "";
}

//------------------------------------------------------------------------------------------
yadma.updateCell = function( Cell, Value  )
{
   if ( Cell.html() !== Value )
   {
      Cell.html( Value );
   }
}

//------------------------------------------------------------------------------------------
yadma.showRow = function( Row, RowData )
{
   var Cols = Row.find( "td" );

   yadma.updateCell( Cols.eq(0), extractFileName( RowData.FileName ) );
   yadma.updateCell( Cols.eq(1), formatBytes( RowData.FileSize ) );

   if ( Cols.size() == 3 )
   {
      yadma.updateCell( Cols.eq(2), yadma.getRowAction( RowData ) );
   }
   else
   {
      yadma.updateCell( Cols.eq(2), formatBytes( RowData.Downloaded ) );
      yadma.updateCell( Cols.eq(3), formatBytes( RowData.FileSize - RowData.Downloaded ) );
      if ( Cols.size() == 7 )
      {
         if ( RowData.Status == 1 )
         {
            yadma.updateCell( Cols.eq(4), "waiting" );
         }
         else
         {
            yadma.updateCell( Cols.eq(4), formatBytes( RowData.Speed ) + "/s" );
         }
         yadma.updateCell( Cols.eq(5), computeTime( RowData.FileSize - RowData.Downloaded, RowData.Speed ) );
         yadma.updateCell( Cols.eq(6), yadma.getRowAction( RowData ) );
      }else if ( Cols.size() == 5 )
      {
         yadma.updateCell( Cols.eq(4), yadma.getRowAction( RowData ) );
      }
   }

}

//------------------------------------------------------------------------------------------
yadma.resizeDownloadTable = function( VisibleRows )
{
    var Table       = $( "#tbl_downs" ).find( "tbody" );
    var VisibleCols = yadma.countVisibleCols();
    var XChange		= Table.find("tr").size();

    while( VisibleRows > yadma.DownPreviousTab.Data.length + 1 )
    {
        var trow = $("<tr>").addClass( ( XChange % 2 == 0 ) ? "tbl_down_2" : "tbl_down_1" );
        for ( var c = 1; c <= VisibleCols; c++ )
        {
            $("<td>").data("col", c).addClass( (c == 1) ? "tbl_cell_txt" : "tbl_cell").html("&nbsp;").appendTo( trow );
        }
        trow.appendTo( Table );
        yadma.DownPreviousTab.Data.push(-1);
		XChange++;
    }

    if ( VisibleRows < yadma.DownPreviousTab.Data.length )
    {
        var Rows = Table.find("tr:gt(" + (VisibleRows - 1) + ")");

        if ( Rows.size() >= 0 )
        {
            Rows.remove();
        }

        yadma.DownPreviousTab.Data.length = VisibleRows;
   }

   return Table;
}

//------------------------------------------------------------------------------------------
yadma.setupTable = function()
{
    if ( yadma.DownPreviousTab.Filter !== yadma.DownCurrentTab )
    {
        yadma.DownPreviousTab.Data    = new Array();
        yadma.DownPreviousTab.Filter  = yadma.DownCurrentTab;
        yadma.InitializeDownTable();
    }

    var VisibleRows = yadma.countVisibleRows();

    if( (VisibleRows < 1) && ( yadma.DownPreviousTab.Data.length > 0 ) )
    {
        yadma.DownPreviousTab.Data = new Array();
        yadma.InitializeDownTable();
        return 0;
    }

    return VisibleRows;
}

//------------------------------------------------------------------------------------------
yadma.showSumary = function( Count, Size, Downloaded, Speed )
{
    var Cols = $("#tbl_down_sumary").find("td");

    var VisibleCols = yadma.countVisibleCols();

    yadma.updateCell( Cols.eq(0), "Total of downloads = " + Count );
    yadma.updateCell( Cols.eq(1), formatBytes( Size )  );

    if ( VisibleCols > 3 )
    {
        yadma.updateCell( Cols.eq(2), formatBytes( Downloaded ) );
        yadma.updateCell( Cols.eq(3), formatBytes( Size - Downloaded ) );
        if ( Cols.size() == 7 )
        {
            yadma.updateCell( Cols.eq(4), formatBytes( Speed ) + "/s" );
            yadma.updateCell( Cols.eq(5), computeTime( Size - Downloaded, Speed ) );
        }
    }else
	{
		yadma.updateCell( Cols.eq(2), "<input style='height:100%' type=button value='Clear All' onclick=yadma.clearDownload(-1)>"  );
	}
}

//------------------------------------------------------------------------------------------
yadma.showList = function()
{
   var VisibleRows = 0;

   if ( ( VisibleRows = yadma.setupTable() ) < 1 )
   {
      return;
   }

   var Rows        = yadma.resizeDownloadTable( VisibleRows ).find( "tr" );
   var Id          = 0;
   var Comparator  = yadma.getComparator();
   var Downloaded  = 0;
   var Size        = 0;
   var Speed       = 0;

   for ( var i = 0; i < yadma.DownloadList.length; i++ )
   {
        Comparator( yadma.DownloadList[i], function( RowData )
        {
            yadma.DownPreviousTab.Data[Id] = RowData.Id;
            yadma.showRow( Rows.eq(Id), RowData ); Id++;

            Downloaded = Downloaded + ( isNaN( RowData.Downloaded ) ? 0 : parseInt(RowData.Downloaded) );
            Size       = Size + ( isNaN( RowData.FileSize ) ? 0 : parseInt(RowData.FileSize) );
            Speed      = Speed + ( isNaN( RowData.Speed ) ? 0 : parseInt(RowData.Speed) );

        } );
   }

   yadma.showSumary( Id, Size, Downloaded, Speed );
}

//------------------------------------------------------------------------------------------
yadma.restoreMenuColors = function()
{
	$("#lnk_downloads").removeClass( "menu_selected_link" );
	$("#lnk_settings").removeClass( "menu_selected_link" );
	$("#lnk_about").removeClass( "menu_selected_link");
}

//------------------------------------------------------------------------------------------
yadma.downloadClick = function()
{
	yadma.restoreMenuColors();
	yadma.DownPreviousTab.Filter = "";

	$("#lnk_downloads").addClass( "menu_selected_link" );

	$("#d_smenu_downs").css( "display", "block" );
	$("#d_smenu_settings").css( "display", "none" );
	$("#d_smenu_about").css( "display", "none" );

	yadma.updateDownloadData();

	yadma.showRunningClick();

}

//-----------------------------------------------------------------------------------------
yadma.initializeWindow = function( DivId, Title, Content, Width, Height )
{
   var adiv = $( "#" + DivId );

   adiv.css( "left", ((yadma.Dimensions.Width/2) - (Width/2)) + "px" );
   adiv.css( "top", "120px" );
   adiv.css( "width", Width + "px" );
   adiv.css( "height", Height + "px" );
   adiv.css( "display", "block" );

   adiv.html( "<div class='main_window_title' align=center style='width:100%'><b>" + Title + "</b></div>" + Content );

   return adiv;
}

//------------------------------------------------------------------------------------------
yadma.settingsClick = function()
{
    yadma.DownCurrentTab = "none";
    $("#d_content").html( "" );

	yadma.restoreMenuColors();

	$("#lnk_settings").addClass( "menu_selected_link" );

	$("#d_smenu_downs").css( "display", "none" );
	$("#d_smenu_settings").css( "display", "block" );
	$("#d_smenu_about").css( "display", "none" );

	yadma.showGenSettingsClick();

}

//------------------------------------------------------------------------------------------
yadma.aboutClick = function()
{
    yadma.DownCurrentTab = "none";
    $("#d_content").html( "" );

	yadma.restoreMenuColors();

	$("#lnk_about").addClass( "menu_selected_link");
	$("#d_smenu_downs").css( "display", "none" );
	$("#d_smenu_settings").css( "display", "none" );
	$("#d_smenu_about").css( "display", "block" );

	var AboutData = yadma.get( "/yadma/about.html" );

	if ( AboutData.Success )
	{
		 $("#d_content").html( AboutData.Data );
	}
	else
	{
		$("#d_content").html( "drigofast@gmail.com" );
	}
}

//------------------------------------------------------------------------------------------
yadma.restoreSMenuDownsColors = function()
{
	$("#a_down_running").removeClass("smenu_selected_link");
	$("#a_down_active").removeClass("smenu_selected_link");
	$("#a_down_waiting").removeClass("smenu_selected_link");
	$("#a_down_stopped").removeClass("smenu_selected_link");
	$("#a_down_finished").removeClass("smenu_selected_link");
}

//------------------------------------------------------------------------------------------
yadma.addDownloads = function()
{
    var links = document.getElementById( "newDownloadLinks" ).value;
	var site  = document.getElementById( "newDownloadPicture" ).value;
	var folder = document.getElementById( "newDownloadFolder" ).value;

	$('#gbl_window1').css('display','none');

   var params =  "links=" + urlencode( links ) + "&site=" + urlencode( site ) + "&folder=" + urlencode( folder );

   yadma.post( "/yadma/downloads.txt", params );
   yadma.ListVersion         = -1;
}

//------------------------------------------------------------------------------------------
yadma.newDownloadClick = function()
{
//DivId, Title, Content, Width, Height
   html =  '<div align="left">Urls:<br><textarea wrap="off" style="width: 100%; height: 200px;" id="newDownloadLinks"></textarea>'
   + '<br>Folder:<br><input id="newDownloadFolder" style="width: 100%;" type="text">'
   + 'Site:<br><input id="newDownloadPicture" style="width: 100%;" type="text">'
   + '</div><div align="right"><input value="OK" style="width: 120px;" onclick="yadma.addDownloads()" type="button">'
   + '<input value="Cancel" style="width: 120px;" onclick="$(\'#gbl_window1\').css(\'display\',\'none\')" type="button"></div></div>';
   var wnd = yadma.initializeWindow( "gbl_window1", "New Download", html, 600, 360 );

}

//------------------------------------------------------------------------------------------
yadma.showRunningClick = function()
{
	yadma.restoreSMenuDownsColors();
	$("#a_down_running").addClass("smenu_selected_link");
	yadma.DownCurrentTab = "running";
	yadma.showList();
}

//------------------------------------------------------------------------------------------
yadma.showActiveClick = function()
{
	yadma.restoreSMenuDownsColors();
	$("#a_down_active").addClass("smenu_selected_link");
    yadma.DownCurrentTab = "active";
	yadma.showList();
}

//------------------------------------------------------------------------------------------
yadma.showWaitingClick = function()
{
	yadma.restoreSMenuDownsColors();
	$("#a_down_waiting").addClass("smenu_selected_link");
	yadma.DownCurrentTab = "waiting";
	yadma.showList();
}

//------------------------------------------------------------------------------------------
yadma.showStoppedClick = function()
{
	yadma.restoreSMenuDownsColors();
	$("#a_down_stopped").addClass("smenu_selected_link");
	yadma.DownCurrentTab = "stopped";
	yadma.showList();
}

//------------------------------------------------------------------------------------------
yadma.showFinishedClick = function()
{
	yadma.restoreSMenuDownsColors();
	$("#a_down_finished").addClass("smenu_selected_link");
	yadma.DownCurrentTab = "finished";
	yadma.showList();
}

//------------------------------------------------------------------------------------------
yadma.showGenSettingsClick = function()
{
	$( "#a_acc_settings" ).removeClass( "smenu_selected_link" );
	$( "#a_gen_settings" ).addClass( "smenu_selected_link" );

	yadmasettings.showWindow();
}

//------------------------------------------------------------------------------------------
yadma.showAccSettingsClick = function()
{
	$( "#a_gen_settings" ).removeClass( "smenu_selected_link" );
	$( "#a_acc_settings" ).addClass( "smenu_selected_link" );

	yadmaacc.showWindow();
}

//------------------------------------------------------------------------------------------
yadma.buildInterface = function()
{
	$("#t_main").css( "width",  yadma.Dimensions.Width + "px" );
	$("#d_menu").css( "width",  "100%" );
	$("#d_smenu").css( "width", "100%" );
	$("#d_content").css( "width", "100%" );

	$("#lnk_downloads").click( yadma.downloadClick );
	$("#lnk_settings").click( yadma.settingsClick );
	$("#lnk_about").click( yadma.aboutClick );

	$("#a_down_new").click( yadma.newDownloadClick );
	$("#a_down_running").click( yadma.showRunningClick );
	$("#a_down_active").click( yadma.showActiveClick );
	$("#a_down_waiting").click( yadma.showWaitingClick );
	$("#a_down_stopped").click( yadma.showStoppedClick );
	$("#a_down_finished").click( yadma.showFinishedClick );

	$( "#a_acc_settings" ).click( yadma.showAccSettingsClick );
	$( "#a_gen_settings" ).click( yadma.showGenSettingsClick );

	yadma.downloadClick();
}

//------------------------------------------------------------------------------------------
yadma.updateDownloadData = function()
{
    try
    {
       if( yadma.updateList() )
       {
          return true;
       }
    }catch(error)
    {
    }

    return false;
}

//------------------------------------------------------------------------------------------
yadma.updateInterface = function()
{
    if ( yadma.DownCurrentTab !== "none" )
    {
        if( yadma.updateDownloadData() )
        {
            yadma.showList();
        }
    }
    setTimeout( "yadma.updateInterface()", yadma.RefreshInterval );
}

//------------------------------------------------------------------------------------------
yadma.startTimer = function()
{
    if ( yadma.Started == false )
    {
        yadma.Started = true;

        setTimeout( "yadma.updateInterface()", yadma.RefreshInterval );
    }
}

//------------------------------------------------------------------------------------------
yadma.updateList = function()
{

    var LstVersion = parseInt( yadma.get( "/yadma/downloads.txt?version=1" ).Data );

    if ( isNaN( LstVersion ) )
    {
      return false;
    }

    if ( ( LstVersion !== yadma.ListVersion ) ||
          ( yadma.DownCurrentTab == "active" ) ||
          ( yadma.DownCurrentTab == "running" ) )
    {
		if ( LstVersion !== yadma.ListVersion )
		{
			yadma.DownPreviousTab.Filter = "";
		}

        dllist = yadma.get( "/yadma/downloads.txt" );

        if ( dllist.Success == true )
        {
            yadma.ListVersion = LstVersion;

            yadma.DownloadList  = yadma.parseTableData( yadma.DownloadHeaders, dllist.Data );

            return true;
        }
    }

   return false;
}

//------------------------------------------------------------------------------------------
yadma.get = function( Url )
{
   return yadma.getContents( "GET", Url, "", 2000 );
}

//------------------------------------------------------------------------------------------
yadma.post = function( Url, PostData )
{
   return yadma.getContents( "POST", Url, PostData, 6000 );
}

//------------------------------------------------------------------------------------------
yadma.getContents = function( Method, Url, PostData , TimeOut )
{
    var RetVal = { Success: false, Data: "" };

    $.ajax( { async: false, url: Url, type: Method, cache: false,
              contentType: 'application/x-www-form-urlencoded; charset=UTF-8',
              data: PostData, timeout: TimeOut,
              success: function( Data )
              {
                 RetVal.Success = true;
                 RetVal.Data = Data;
              }
             } );

   return RetVal;
}

//------------------------------------------------------------------------------------------
yadma.start = function()
{
    yadma.defineLimits();

    yadma.buildInterface();

    yadma.startTimer();
}
