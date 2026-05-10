// COPYRIGHT (c) 2019
// HONEYWELL INTERNATIONAL Sàrl.
// ALL RIGHTS RESERVED
//
// This software is a copyrighted work and/or information protected
// as a trade secret. Legal rights of Honeywell International Sàrl in this
// software is distinct FROM ownership of any medium in which the
// software is embodied. Copyright or trade secret notices included
// must be reproduced in any copies authorized by Honeywell International Sàrl
// The information in this software is subject to change without
// notice and should not be considered as a commitment by Honeywell International Sàrl.
//------------------------------------------------------------------------------------
// <dbpath> globally defined in "Common_GSH"
// <bSafeviewActive> globally defined in "Common_GSH"
//------------------------------------------------------------------------------------
var yoking_srcFileName = "yoking_gsh.js";
var yoking_releaseNum = "R511.1";
var yoking_versionDate = "June 2019";

//***************************************************************************************************************
// Associated focus routine
//***************************************************************************************************************
//Modified for cross display assoc focus
function assoc_focus() {
var oShape, sTgNmPrincipal;

	//Opening the database
	if( typeof (window.dbConn) == "undefined" ){
		dbConn = new ActiveXObject("ADODB.Connection");
	}	
	dbConn.Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dbPath);

		if(arguments.length>0){
			sTgNmPrincipal = arguments[0];
		}else{
			oShape=document.getElementById(Page.getAttribute("lastselection")).parentNode.parentNode;
			sTgNmPrincipal = getShapeTagname(oShape); //Common_GSH Required
		}

	var sql = "SELECT Tagname FROM Tag_Family WHERE Family=(SELECT Family FROM Tag_Family WHERE Tagname=\'" + sTgNmPrincipal + "\')";

	var rs = dbConn.Execute(sql);
	//select the family
	sTagsToSelect = "',";
	if( rs.eof ){
		sTagsToSelect = sTagsToSelect + sTgNmPrincipal + ",";
	}else{
		while ( !rs.eof ){
			if( rs.Fields.Item("Tagname").value != "" ){
				sTagsToSelect = sTagsToSelect + rs.Fields.Item("Tagname").value + ",";
			}
			rs.MoveNext();
		}
	}
	//Get shapes to select from local scripting dictionary (populated once on pagecomplete)
	if( g_oTagDict ){
		for (var key in g_oTagDict){

			//if(oKey){
				var arrShapes = g_oTagDict[key].toString().split(",");
				for(j=0;j<arrShapes.length;j++){				
				var oShp = document.getElementById(arrShapes[j]);	
				if( oShp.Objects("RecFrame") ){
						if(sTgNmPrincipal.toLowerCase() == key.toLowerCase()){
								oShp.Objects("RecFrame").styleclass="RecFrameInPrincipalFocus";
						}else if(sTagsToSelect.toLowerCase().indexOf(("," + key + ",").toLowerCase()) > -1 ){
								oShp.Objects("RecFrame").styleclass="RecFrameInAssociatedFocus";
						}else{
								oShp.Objects("RecFrame").styleclass = oShp.getAttribute("RecFrame_Class"); //stored on page_pagecomplete
						}
					}
					if( oShp.Objects("PlgH1") ){
						oShp.Objects("PlgH1").fillcolor=oShp.Objects("RecFrame").fillcolor;
						oShp.Objects("PlgH1").linecolor=oShp.Objects("RecFrame").fillcolor;
					}
				}
			//}
		}
	}
	//Close DB - Yoking: assoc_focus
	if( dbConn.State != 0 ){
		dbConn.Close(); 
	} 
	//if( err.number !=0 && window.external.Application.environment == 0 && spDebug ){ window.external.Application.LogMessage(window.external.Application.currentpage + " : assoc_focus ->" + oSource.id + " " + err.description); }
}
//*****************************************************************************************************************
// Database reading routines
//*****************************************************************************************************************
function ReadAndCallAssociates(oSource){
var sTablename, sFieldname, sDisplayName, bYoke , myDsp,CallDsp0, retVal, CurrDisplayLevel, lastDspIdx;
var DisplaysToBeCalled = [];
var bReturn, hwnd, objSView;

	if(window.g_oSafeView){
		objSView = window.g_oSafeView;
	}else{
		try{
			objSView = GetObject("","Honeywell.Workspace.Client");
		}catch(e){
			alert("error = " + e.desc);
		}
	}

	var iYokeEnabled = (Page.GetStyleClassProperty("Enable_Yoking","key").toUpperCase() == "ON");
	if( !objSView || !iYokeEnabled ){
		bYoke=false;
	}else{
		bYoke=true;
	}
	var oShape = oSource.parentNode.parentNode;
	isOnTopNavBar=false;
	if( document.URL.toLowerCase().indexOf("dest=touchpanel-toolbar") > -1 ){
		isOnTopNavBar = true;
	}

	if( oSource.title != "" ){
		sDisplayName = oSource.title;
	}else{
		sDisplayName = oShape.getcustomproperty("Text","DspName");
	}
	//Opening the database
	if( typeof (window.dbConn) == "undefined" ){
		dbConn = new ActiveXObject("ADODB.Connection");
	}
	dbConn.Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dbPath);
	//var the field && table name for the first query
	//Check display level
	sTablename = "Level2_Links";
	sFieldname = "Level2";
	CurrDisplayLevel = "None";
	DisplaysToBeCalled[0] = sDisplayName;

	if( sDisplayName == window.external.Application.currentpage ){
		CallDsp0 = false;
	}else{
		CallDsp0 = true;
	}
	lastDspIdx=0;
	if( bYoke ){
		//execute the first query
		var sql = "SELECT * FROM " + sTablename + " WHERE " + sFieldname + " Like \'" + DisplaysToBeCalled[0] + "\'";
		var rs = dbConn.Execute(sql);
		if( !rs.EOF ){
			CurrDisplayLevel = "Level2";
			//Allow multiple L3 (comma separated)
			if( rs.Fields.Item(1).value.indexOf(",") > -1 ){
				var arrL3 = rs.Fields.Item(1).value.toString().split(",");
				for(var i=0;i<=arrL3.length;i++){
					if( arrL3[i] != "" ){
						lastDspIdx=lastDspIdx+1;
						DisplaysToBeCalled[lastDspIdx] = arrL3[i]; //rs.Fields.Item(1).value;
					}
				}
			}else{
				lastDspIdx=lastDspIdx+1;
				DisplaysToBeCalled[lastDspIdx] = rs.Fields.Item(1).value;
			}
		}
		// Level 3
		sTablename = "Level3_Links";
		sFieldname = "Level3";
		if( CurrDisplayLevel == "Level2" ){
			sql = "SELECT * FROM " + sTablename + " WHERE " + sFieldname + " Like \'" + DisplaysToBeCalled[1] + "\'";
		}else{
			sql = "SELECT * FROM " + sTablename + " WHERE " + sFieldname + " Like \'" + DisplaysToBeCalled[0] + "\'";
			CurrDisplayLevel = "Level3";
		}
		rs = dbConn.Execute(sql);
		if( !rs.EOF ){
			//Allow multiple L4 (comma separated)
			if( rs.Fields.Item(1).value.indexOf(",") > -1 ){
				arrL4 = rs.Fields.Item(1).value.split(",");
				for(var j=0;j<=arrL4.length;j++){
					if( arrL4[j] != "" ){
						lastDspIdx=lastDspIdx+1;
						DisplaysToBeCalled[lastDspIdx] = arrL4[j]; //rs.Fields.Item(1).value;
					}
				}
			}else if( rs.Fields.Item(1).value != "" ){
				lastDspIdx=lastDspIdx+1;
				DisplaysToBeCalled[lastDspIdx] = rs.Fields.Item(1).value;
			}
		}
	}
	//--------- Added for disabling global focus ------------
	if( window.bSafeviewActive ){
		for(var k= 0;k<=(window.external.Application.Parent.StationWindows.Count - 1);k++){
			hwnd = window.external.Application.Parent.StationWindows.Item(k).Appwindow.hwnd;
			bReturn = objSView.IsWindowGlobalFocus(hwnd);
			if( bReturn ) break;
		}
	}
	if( bReturn ){ objSView.setglobaloutputfocuswnd(hwnd, false); } 
	//---------------------- end ----------------------------
	if( DisplaysToBeCalled[0] != "" && CallDsp0 ){
		if( isOnTopNavBar ){
			myDsp = DisplaysToBeCalled[0] + "?dest=StationScreen";
		}else{
			myDsp = DisplaysToBeCalled[0];
		}
		retVal = OpenDisplay (myDsp,2,0);
	}
	sYokeDelay = Page.GetStyleClassProperty("SP_Options","hw-sp-yokingdelay");
	if( isNumeric(sYokeDelay) ){
		iDelay = parseInt(sYokeDelay);
	}else{
		iDelay = 100;
	}
	//Multiple displays call-up
	for(var m= 1;m<=lastDspIdx;m++){
		if( DisplaysToBeCalled[m].toLowerCase().indexOf(".xml") > -1 ){
			execute(DisplaysToBeCalled[m]);
		}else{
			if( DisplaysToBeCalled[m] != "" ){
				if( isOnTopNavBar ){
					if( DisplaysToBeCalled[m].indexOf("?") > -1 ){
						myDsp = DisplaysToBeCalled[m] + Chr(38) + "dest=StationScreen";
					}else{
						myDsp = DisplaysToBeCalled[m] + "?dest=StationScreen";
					}
				}else{
					myDsp = DisplaysToBeCalled[m];
				}
				retVal=OpenDisplay(myDsp,2,m*iDelay);
			}
		}
	}
	//Close DB - Yoking: ReadAndCallAssociates (Display associations)
	if( dbConn.State != 0 ){
		dbConn.Close(); 
	}
}
//
// DspYoking - performs Tag Yoking - call up utility displays for selected Tag
//
function DspYoking(oSource){
var  myDsp, isOnTouchPanel, retVal;

	isOnTouchPanel = false;
	if( document.URL.toLowerCase().indexOf("dest=touchpanel") > -1 ){ 
		isOnTouchPanel = true;
	}
	//DspYoking disabled on touchpanel
	if( !isOnTouchPanel ){
		//Opening the database
		if( typeof (window.dbConn) == "undefined" ){
			dbConn = new ActiveXObject("ADODB.Connection");
		}
		dbConn.Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + dbPath);
		var sTablename = "TagnameYoking";
		var sFieldname1 = "Tagname";
		var oShape=document.getElementById(oSource.id).parentNode.parentNode;
		var sname = getShapeTagname(oShape); //Common_GSH Required
		var sql = "SELECT * FROM " + sTablename + " WHERE " + sFieldname1 + " Like \'" + sname + "\'";

		var rs = dbConn.Execute(sql);
		sYokeDelay = Page.GetStyleClassProperty("SP_Options","hw-sp-yokingdelay");

		if( isNumeric(sYokeDelay) ){
			iDelay = parseInt(sYokeDelay);
		}else{
			iDelay = 100;
		}
		if(!rs.EOF){
			if( rs.Fields.Item("Level4_Display").value != "" ){
				myDsp = rs.Fields.Item("Level4_Display").value + "?currentpoint=" + sname;
				retVal = OpenDisplay(myDsp,1,0);
			}
			if( rs.Fields.Item("Support_Display").value != "" ){
				myDsp = rs.Fields.Item("Support_Display").value + "?currentpoint=" + sname;
				retVal = OpenDisplay(myDsp,1,iDelay);
			}
			if( rs.Fields.Item("Aux_Display").value != "" ){ 
				myDsp = rs.Fields.Item("Aux_Display").value + "?currentpoint=" + sname;
				retVal = OpenDisplay(myDsp,1,2*iDelay);
			}
		}
		//Close DB - Yoking: DspYoking (Tagname associations)
		if( dbConn.State != 0 ){
			dbConn.Close();
		}
	}
}
//
//iCallMethod: 1-CurrentPage, 2-RequestTask | iDelay : ms
//
function OpenDisplay(sURL,iCallMethod,iDelay) {
var  bOpen, strPageCallup;

	bOpen = false;
	if( sURL != "" ){
		bOpen=true;
		var sDisplay = sURL.toLowerCase().split("?")[0];
		var sYokeOpened = Page.GetStyleClassProperty("SP_Options","hw-sp-yokeopeneddisplays");
		if( sDisplay.indexOf(".htm") < 0 ) {sDisplay += ".htm"; }
		if( sYokeOpened == "no" || sYokeOpened == "off" || sYokeOpened == "false" ){
			for (var oWindow in window.external.parent.StationWindows){
				if( oWindow.CurrentPage.toLowerCase() == sDisplay ){
					bOpen=false; 
				}
			}
		}
		if( bOpen==true ){
			if( iCallMethod == 1 ){
				strPageCallup = "window.external.Application.currentpage = \"" + sURL + "\"";
				//window.external.Application.currentpage = sURL
			}else{
				strPageCallup = "window.external.Application.RequestTask(21, 1, 0, 0, 0, \"" + sURL + "\")";
				//window.external.Application.RequestTask 21, 1, 0, 0, 0, sURL
			}
			window.setTimeout(strPageCallup, iDelay, "JavaScript");
		}
	}
	if( window.external.application.environment == 0 ){ window.external.Application.LogMessage(">> delayed call-up: " + sURL + " : " + iDelay + "ms"); }
	return bOpen;
}
//
//support for cross display assoc focus
//
function xdsp_onpagecomplete(){
	var fpOpen = false;

	if(window.external.Application.Parent.WindowMode == 1){
		//Multi-window
		for(var i=0;i<window.external.Application.Parent.StationWindows.count;i++){
			var oWnd = window.external.Application.Parent.StationWindows.Item(i);
			var docLocation = oWnd.document.URL.toLowerCase();
			var strFind = "currentpoint=" + window.external.application.lastselectedpointid.toLowerCase();
			if(docLocation.indexOf(strFind)>-1){
				fpOpen = true;
				break;
			}
		}
	}else{
		//Single-window
		var objPopupWindow = HDXPopupBehavior.FindPopup("currentpoint=" + window.external.application.lastselectedpointid);
	
		if (objPopupWindow){
			fpOpen = true;
		}else{
			fpOpen = false;
		}
	}
	//Trigger associated focus for the last selected point in case there is a FP still open for it
	if(fpOpen){
		assoc_focus(window.external.application.lastselectedpointid);
	}
}
//
//	SP Custom "OnLastSelectedPointIDChanged" event handler function
//
var fn = function(){
		var bAssocFocusEnabled = false;
		var lastSelectedPointID = window.event.getAttribute("OnLastSelectedPointIDChanged", 0);
		//Filter out event fired for null tag
		if (lastSelectedPointID === null) return;

		var oPage = document.getElementById("Page");
		var sKey = oPage.getStyleClassProperty("Enable_Assoc_Focus","key");
		if (oPage.getAttribute("GSH_Yoking") && sKey.toUpperCase() == "ON"){
			bAssocFocusEnabled = true;
		}
		if (oPage){
			var srcClick = oPage.getAttribute("ShapeIsSelected");
			var lastSelPntID = oPage.getAttribute("lastSelPntID");
			
			if(lastSelPntID == null){
				oPage.setAttribute("lastSelPntID", window.external.Application.LastSelectedPointID);
			}else{
				if(lastSelPntID.toLowerCase() != lastSelectedPointID.toLowerCase() && srcClick !== null){
					oPage.removeAttribute("ShapeIsSelected");	
					var lastSelBox = document.getElementById(Page.getAttribute("lastselection"));
					if(lastSelBox){
						lastSelBox.styleClass = "ScTxtSelectBox_NotSelected";
					}
			
				}
			}
			if(bAssocFocusEnabled){
				assoc_focus(lastSelectedPointID);
			}
		}
	};
//	
//Attach custom "OnLastSelectedPointIDChanged" handler as early as the Page object is available
//
function yoking_ondocstatechange(){
	if(document.readyState == "complete") {
		//document.getElementById("Page").attachEvent("onlastselectedpointidchanged",fn);
		document.getElementById("Page").onlastselectedpointidchanged = fn;
	}
}
//
//Attach to document.onreadystatechange
//
if(document){
	document.attachEvent("onreadystatechange",yoking_ondocstatechange);
}
//
// Custom debug function
//
if(!dbg){
	var dbg = function (str) { 
		var bLog = true; //true-LogMessage,false-Alert
		var err = window.external.Application.currentpage + " >>> " + str;
		//if(!spDebug){return;}
		if (window.external.Application.environment == 0) {
			if(bLog)
			{
				window.external.Application.LogMessage (err);
			}
			else
			{
				alert(err);
			}	   
		}   
	};
}
// Util js functions
if(!isNumeric){
	var isNumeric = function (n) {
		return !isNaN(parseFloat(n)) && isFinite(n);
	};
}
//
if(window.external.Application.environment == 0) { window.external.Application.LogMessage(window.external.Application.currentpage + " : >>> Yoking_GSH " + yoking_releaseNum + " version date: " + yoking_versionDate); }