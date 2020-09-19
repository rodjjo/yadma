function formatBytes( Bytes )
{
    var Sufix = new Array ( "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" );

    var LastResult 	= parseFloat( Bytes );
    var Result		= LastResult;
	 
    for ( ti = 1; ti < 10; ti++ )
    {
        Result /= 1024.0;
    
        if (  Result <= 1.0 )
        {
        		return  LastResult.toFixed(2) + " " + Sufix[ ti - 1 ];
        }
        
        LastResult = Result;
    }

    return  "???";
}

//-----------------------------------------------------------------
function extractFileName( FilePath )
{
    var pos = FilePath.lastIndexOf( '/' );
    
    if ( pos < 0 )
    {
        pos = FilePath.lastIndexOf( '\\' );
    }
    
    if ( pos > -1 )
    {
        return FilePath.substr( pos + 1 );
    }
    
    return FilePath;
}

//-----------------------------------------------------------------
function urlencode(str) 
{
	str = escape(str);
	str = str.replace('+', '%2B');
	str = str.replace('%20', '+');
	str = str.replace('*', '%2A');
	str = str.replace('/', '%2F');
	str = str.replace('@', '%40');
	
	return str;
}

//-----------------------------------------------------------------
function computeTime( Remain, Speed )
{
    if ( Speed <= 0 )
	 {
		 return "???";
	 }	
	 
    var Seconds = parseInt( Remain / Speed );
    
	 if ( Seconds < 10 )
	 {
       return "00:00:0" + Seconds;
	 }
	 
	 if ( Seconds < 60 )
	 {
	    return "00:00:" + Seconds;
    }
	 
    var Years = parseInt( Seconds / ( 3600 * 24 * 365) );
    
    if( Years > 3 )
    {
		return ":(";
    } 

    if ( Years > 0 )
    {
		return Years + " A. " + ( parseInt(Seconds / ( 3600 * 24) ) - ( Years * 365) ) + " D.";
    }

    Days = parseInt(Seconds / (3600 * 24)); 

    if ( Days > 0 )
    {
        return  Days + " D. " + parseInt(Seconds/3600) - ( Days * 24) + " H.";
    }

    var Hours 	= parseInt(Seconds / 3600);	 
    var Minutes = ( parseInt(Seconds / 60) - ( Hours * 60) );  
    Seconds = (Seconds % 60);
     
    if ( Hours < 10 )
    {
    	Hours = "0" + Hours;
    }

    if ( Minutes < 10 )
    {
    	Minutes = "0" + Minutes; 
    }
    
    if ( Seconds < 10 )
    {
    	Seconds = "0" + Seconds;
    }
    
    return Hours + ":" + Minutes + ":" + Seconds;
}


