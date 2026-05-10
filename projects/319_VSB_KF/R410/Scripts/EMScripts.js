var g_oPermPopup = null;
var g_iPermPopupWidth = 0;
var g_iPermPopupHeight = 0;
var g_iPermValue = null;
var g_oPermElement = null;
var g_iDisabledValue = null;
var g_oDisabledElem = null;
var g_oConfirmPopup = null;
var g_iConfirmPopupWidth = 0;
var g_iConfirmPopupHeight = 0;
//--------------------------------------------------------------------------------
var i_CONFIRM_CANCEL	= 0;
var i_CONFIRM_YES		= 1;
var i_CONFIRM_NO		= 2;
//--------------------------------------------------------------------------------
// Strings that need localization
var str_PERM_FULL					= "Full access";
var str_PERM_CTRL_NO_ACK				= "Control without acknowledge";
var str_PERM_ACK					= "View and acknowledge";
var str_PERM_READ					= "View only";
var str_PERM_READ_NO_ALM                                = "View without alarms";
var str_PERM_DENY					= "No access";
var str_PERM_HIGHER_LEVEL			= "Higher security level required";
var str_PERM_AREA_NOT_ASSIGNED		= "Asset not assigned to operator/station";
var str_PERM_CLUSTER_DOWN			= "The requested operation cannot be performed while the cluster server is unavailable";
var str_PERM_GENERIC_FAIL			= "The requested operation could not be performed";
var str_PERM_OVERRIDE_PERMISSION	= "Some sub-items are configured with a different permission value." + "\n" + "Would you like to override these with the new value?";
var str_PERM_OVERRIDE_DISABLED		= "Some sub-items are configured with a different disabled value." + "\n" + "Would you like to override these with the new value?";
var str_CONFIRM_YES					= "Yes";
var str_CONFIRM_NO					= "No";
var str_CONFIRM_CANCEL				= "Cancel";
//--------------------------------------------------------------------------------
function ShowPermissionMenu(x, y, elem)
{	
	if( g_oPermPopup == null )
	{
		g_oPermPopup = window.createPopup();
		var oBody = g_oPermPopup.document.body;
		oBody.style.cssText = "font-family: Helvetica; font-size: 8pt; margin: 0px; padding: 0px; background-color: menu; border: 2px outset; overflow: visible; white-space: nowrap; cursor: hand;";
		var strContent = "";
		var arr = new Array("Images/perm_full.gif", str_PERM_FULL, "4", 
							"Images/perm_ack.gif", str_PERM_ACK, "3", 
							"Images/perm_read.gif", str_PERM_READ, "2", 
							"Images/perm_read_no_alm.gif", str_PERM_READ_NO_ALM, "1", 
							"Images/perm_deny.gif", str_PERM_DENY, "0");
		strContent += "<div onselectstart='return false' ondragstart='return false' style='width: 100%; height: 100%;'>";
		for( var i=0; i<arr.length; i+=3 )
		{
			strContent += "<div style='padding:2px; width: 100%;' onmouseenter='this.className=\"On\"' onmouseleave='this.className=\"\"' onmouseup='this.className=\"\"; parent.ApplyPermission(";
			strContent += arr[i+2];
			strContent += ", document.body.targetElement)'><img style='height: 15px; width: 15px;' src='";
			strContent += arr[i];
			strContent += "' /> ";
			strContent += arr[i+1];
			strContent += " </div>";
		}
		strContent += "</div>";
		strContent += "<style type='text/css'> .On{ background-color: highlight; color: highlighttext } </style>";
		oBody.innerHTML = strContent;
		g_oPermPopup.show(x, y, 0, 0, elem);
		g_iPermPopupWidth = oBody.firstChild.offsetWidth + 10;
		g_iPermPopupHeight = oBody.firstChild.offsetHeight + 5;
	}
	g_oPermPopup.hide();
	g_oPermPopup.document.body.setAttribute("targetElement", elem);
	g_oPermPopup.show(x, y, g_iPermPopupWidth, g_iPermPopupHeight, elem);
}
//--------------------------------------------------------------------------------
function ShowConfirmation(msg, callback)
{
	var oBody = null;
	if( g_oConfirmPopup == null )
	{
		g_oConfirmPopup = window.createPopup();
		oBody = g_oConfirmPopup.document.body;
		oBody.style.cssText = "font-family: Helvetica; font-size: 8pt; margin: 0px; padding: 0px; background-color: menu; border: 2px outset; overflow: visible; white-space: nowrap; text-align: center; cursor: default;";
		var strContent = "<div onselectstart='return false' ondragstart='return false' style='width: 100%; height: 100%;'>";
		strContent += "<div id='msg' style='padding: 15px; text-align: left;'></div>";
		strContent += "<div>";
		strContent += "<button id='btnYes' style='margin: 3px; width: 60px; overflow: visible;'>" + str_CONFIRM_YES + "</button>";
		strContent += "<button id='btnNo' style='margin: 3px; width: 60px; overflow: visible;'>" + str_CONFIRM_NO + "</button>";
		strContent += "<button id='btnCancel' style='margin: 3px; width: 60px; overflow: visible;'>" + str_CONFIRM_CANCEL + "</button>";
		strContent += "</div>";
		strContent += "</div>";
		oBody.innerHTML = strContent;
	}
	else
	{
		oBody = g_oConfirmPopup.document.body;
	}
	g_oConfirmPopup.hide();
	g_oConfirmPopup.document.all.msg.innerText = msg;
	g_oConfirmPopup.show(0, 0, 0, 0);
	var iConfirmPopupWidth = oBody.firstChild.offsetWidth + 10;
	var iConfirmPopupHeight = oBody.firstChild.offsetHeight + 5;
	var iConfirmPopupTop = window.screenTop + (document.body.clientHeight - iConfirmPopupHeight) / 2;
	var iConfirmPopupLeft = window.screenLeft + (document.body.clientWidth - iConfirmPopupWidth) / 2;
	g_oConfirmPopup.hide();
	g_oConfirmPopup.show(iConfirmPopupLeft, iConfirmPopupTop, iConfirmPopupWidth, iConfirmPopupHeight);
	g_oConfirmPopup.document.all.btnYes.onclick = function() { callback(i_CONFIRM_YES); };
	g_oConfirmPopup.document.all.btnNo.onclick = function() { callback(i_CONFIRM_NO); };
	g_oConfirmPopup.document.all.btnCancel.onclick = function() { callback(i_CONFIRM_CANCEL); };
}
//--------------------------------------------------------------------------------
function EnsureSecurityLevel(value)
{
	if( external.SecurityLevel < value )
	{
		external.MessageZoneText = str_PERM_HIGHER_LEVEL;
		return false;
	}
	return true;
}
//--------------------------------------------------------------------------------
function CheckServerAvailable(errormsg)
{
	var oRuntimeStatus = null;
	try { oRuntimeStatus = external.Parent.RuntimeStatus; } catch( err ) { }
	if( (oRuntimeStatus != null) && (oRuntimeStatus.IsConsoleStation != 0) )
	{
		var state = oRuntimeStatus.FieldState(5);
		if( state == 1 || state == 3 )
		{
			if (errormsg)
				external.MessageZoneText = str_PERM_CLUSTER_DOWN;
			return false;
		}
	}
	return true;
}
//--------------------------------------------------------------------------------
function Callback_Permission(val)
{
	switch( val )
	{
	case i_CONFIRM_YES:
		if( g_oPermElement.allowPropagatedWrite == 0 )
			g_oPermElement.permPropagated = g_iPermValue;
		else if( g_oPermElement.allowPropagatedWrite == -28622 )
			external.MessageZoneText = str_PERM_AREA_NOT_ASSIGNED;
		else
			external.MessageZoneText = str_PERM_HIGHER_LEVEL;
		break;
	case i_CONFIRM_NO:
		if( g_oPermElement.allowWrite == 0 )
			g_oPermElement.permLocal = g_iPermValue;
		else if( g_oPermElement.allowWrite == -28622 )
			external.MessageZoneText = str_PERM_AREA_NOT_ASSIGNED;
		else
			external.MessageZoneText = str_PERM_HIGHER_LEVEL;
		break;
	default:
		// do nothing
		break;
	}
	if( g_oConfirmPopup != null )
		g_oConfirmPopup.hide();
}
//--------------------------------------------------------------------------------
function ApplyPermission(value, elem)
{
	// Check to see if the server is unavailable
	if( !CheckServerAvailable(true) )
	{
		g_oPermPopup.hide();
		return;
	}
	// Make sure we have valid values
	if( typeof(elem.matching) != "number" )
	{
		external.MessageZoneText = str_PERM_GENERIC_FAIL;
		g_oPermPopup.hide();
		return;
	} 
	g_iPermValue = value;
	g_oPermElement = elem;
	// Check to see if sub-items have a different value	
	if( elem.matching != 1 )
		ShowConfirmation(str_PERM_OVERRIDE_PERMISSION, Callback_Permission);
	else
		Callback_Permission(i_CONFIRM_YES);
	g_oPermPopup.hide();
}
//--------------------------------------------------------------------------------
function Callback_Disabled(val)
{
	switch( val )
	{
	case i_CONFIRM_YES:
		g_oDisabledElement.disabledPropagated = g_iDisabledValue;
		break;
	case i_CONFIRM_NO:
		g_oDisabledElement.disabledLocal = g_iDisabledValue;
		break;
	default:
		// do nothing
		break;
	}
	if( g_oConfirmPopup != null )
		g_oConfirmPopup.hide();
}
//--------------------------------------------------------------------------------
function ApplyDisabled(value, elem, secLevelRequired)
{
	// Make sure we have the appropriate security level
	if( !EnsureSecurityLevel(secLevelRequired) )
		return;
	// Check to see if the server is unavailable
	if( !CheckServerAvailable(true) )
		return;
	// Make sure we have valid values
	if( typeof(elem.matching) != "number" )
	{
		external.MessageZoneText = str_PERM_GENERIC_FAIL;
		return;
	}
	g_iDisabledValue = value;
	g_oDisabledElement = elem;
	// Check to see if sub-items have a different value	
	if( elem.matching != 1 )
		ShowConfirmation(str_PERM_OVERRIDE_DISABLED, Callback_Disabled);
	else
		Callback_Disabled(i_CONFIRM_YES);
}

