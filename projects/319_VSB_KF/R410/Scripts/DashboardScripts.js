var isDashboardGlobalScriptLinked = true;

var em_type_STA        = 1;
var em_type_ANA        = 2;
var em_type_ACC        = 3;
var em_type_ACS        = 4;
var em_type_CON        = 5;
var em_type_ASH        = 6;
var em_type_CLH        = 7;
var em_type_SYS        = 8;
var em_type_ORH        = 9;
var em_type_EQU        = 10;
var em_type_CDA        = 101;
var em_type_RDA        = 102;
var em_type_PSA        = 103;

var em_stype_CLH_SYSALMGRP              = 1002;
var em_stype_SYS_MODEL                  = 1001;
var em_stype_SYS_SERVER                 = 1002;
var em_stype_SYS_CONTROLLERS            = 1003;
var em_stype_SYS_CHANNEL                = 1004;
var em_stype_SYS_CONTROLLER             = 1005;
var em_stype_SYS_PRINTERS               = 1006;
var em_stype_SYS_PRINTER                = 1007;
var em_stype_SYS_SYSINTERFACES          = 1008;
var em_stype_SYS_SYSINTERFACE           = 1009;
var em_stype_SYS_STATIONS               = 1010;
var em_stype_SYS_FLEXSTATIONS           = 1011;
var em_stype_SYS_CONSSTATIONS           = 1012;
var em_stype_SYS_FLEXSTATION            = 1013;
var em_stype_SYS_CONSSTATION            = 1014;
var em_stype_SYS_CONSOLES               = 1015;
var em_stype_SYS_CONSOLE                = 1016;
var em_stype_SYS_CSTNCLIENT             = 1017;
var em_stype_SYS_UNASSIGNEDITEMS        = 1018;
var em_stype_SYS_COMPUTERS              = 1019;
var em_stype_SYS_COMPUTER               = 1020;
var em_stype_SYS_DOMAIN                 = 1021;
var em_stype_SYS_ORGUNIT                = 1022;
var em_stype_SYS_NETEQUIP               = 1023;
var em_stype_SYS_SWITCH                 = 1024;
var em_stype_SYS_FTE                    = 1025;
var em_stype_SYS_FTECOMMUNITY           = 1026;
var em_stype_SYS_FTEDEVICE              = 1027;
var em_stype_SYS_9PSWITCH               = 1028;
var em_stype_SYS_UNKNOWNCONTROLLERS     = 1029;
var em_stype_SYS_PNTSRV_CONTROLLER      = 1030;
var em_stype_SYS_PNTSRV_CHANNEL         = 1031;
var em_stype_SYS_PNTSRV_NETWORK         = 1032;
var em_stype_SYS_METER                  = 1034;

var em_inum_SYS_MODEL_ASSETS            = 1;
var em_inum_SYS_MODEL_ALARMGROUPS       = 2;
var em_inum_SYS_MODEL_SYSTEM            = 3;
var em_inum_SYS_MODEL_CANE              = 4;
var em_inum_SYS_MODEL_ORGANIZATIONS     = 5;
var em_inum_SYS_MODEL_SYSTEMALARMGROUPS = 8;

var ALARM_SHELVED            = 0x00004000;
var ALARM_SUPPRESSED         = 0x00008000;
var ALARM_STATUS             = 0x00001000; 
var ALARM_ACK                = 0x00000800; 
var ALARM_ACK_REQ            = 0x00000400; 
var ALARM_CRITICAL           = 0x00000040;  
var ALARM_URGENT             = 0x00000030;  
var ALARM_HIGH               = 0x00000020;  
var ALARM_LOW                = 0x00000010;
var ALARMPRIORITY_MASK       = 0x000000f0;

var PNTALM_INALM_MASK        = 0x0400;
var PNTALM_UNACKN_MASK       = 0x0100;
var PNTALM_SHELVED_MASK      = 0x1000;
var PNTALM_SUPPRESSED_MASK   = 0x2000;
var PNTALM_ALMPRI_MASK       = 0x00f0;
var PNTALM_ALMPRI_BIT        = 4;

var STATUS_BLANK            = 1;
var STATUS_DISABLED         = 2;
var STATUS_FAILED           = 3;
var STATUS_MARGINAL         = 4;
var STATUS_OK               = 5;
var STATUS_SIMULATION       = 6;

var em_stype_PSA_TPS_LOW  = 1042;
var em_stype_PSA_TPS_HIGH = 1146;
var em_stype_SYS_LCN	  = 1144;
var em_stype_SYS_GATEWAYS = 1146;
var em_stype_PSA_TPS_IOP_LOW  = 1150;
var em_stype_PSA_TPS_IOP_HIGH = 1213;

var OK_ICON = "Icon_OK_2x.png"
var FAIL_ICON = "Icon_Fail_2x.png"
var FAIL_BLINK_ICON = "Icon_Fail_Blink_2x.png"
var MARGINAL_ICON = "Icon_Marginal_2x.png"
var MARGINAL_BLINK_ICON = "Icon_Marginal_Blink_2x.png"
var SEVERE_ICON = "Icon_Severe_2x.png"
var SEVERE_BLINK_ICON = "Icon_Severe_Blink_2x.png"
var TRANSITIONAL_ICON = "Icon_Transitional_2x.png"
var TRANSITIONAL_BLINK_ICON = "Icon_Transitional_Blink_2x.png"
var UNKNOWN_ICON = "Icon_Unknown_2x.png"
var UNKNOWN_BLINK_ICON = "Icon_Unknown_Blink_2x.png"
var DISABLED_ICON = "Icon_Disabled_2x.png"
var BLANK = "blank.png"
var MAINT_ICON = "maint_2x.png"
var MAINT_BLINK_ICON = "maint_blink_2x.png"
var COLLECTION_ICON = "collection_2x.png"
var PERIPHERAL_ICON = "Peripheral_2x.png"
var PERIPHERAL_BLINK_ICON = "Peripheral Blink_2x.png"
var REMOTE_ICON = "remote_node_2x.png"
var BIAS_ICON = "Bias Off_2x.png"
var SYNC_WARN = "Sync Warn_2x.png"
var SYNC_FAIL = "Sync Fail_2x.png"
var DB_INVALID_ICON = "Database_Invalid_2x.png"

var MARGINAL_BACKGROUND = "Background_Marginal_2x.png"
var MARGINAL_PRIMARY_BACKGROUND = "Background_Marginal_Primary_2x.png"
var MARGINAL_SECONDARY_BACKGROUND = "Background_Marginal_Secondary_2x.png"
var FAIL_BACKGROUND = "Background_Fail_2x.png"
var FAIL_PRIMARY_BACKGROUND = "Background_Fail_Primary_2x.png"
var FAIL_SECONDARY_BACKGROUND = "Background_Fail_Secondary_2x.png"
var SEVERE_BACKGROUND = "Background_Severe_2x.png"
var SEVERE_PRIMARY_BACKGROUND = "Background_Severe_Primary_2x.png"
var SEVERE_SECONDARY_BACKGROUND = "Background_Severe_Secondary_2x.png"
var TRANSITIONAL_BACKGROUND = "Background_Transitional_2x.png"
var TRANSITIONAL_PRIMARY_BACKGROUND = "Background_Transitional_Primary_2x.png"
var TRANSITIONAL_SECONDARY_BACKGROUND = "Background_Transitional_Secondary_2x.png"
var UNKNOWN_BACKGROUND = "Background_Unknown_2x.png"
var UNKNOWN_PRIMARY_BACKGROUND = "Background_Unknown_Primary_2x.png"
var UNKNOWN_SECONDARY_BACKGROUND = "Background_Unknown_Secondary_2x.png"
var DISABLED_BACKGROUND = "Background_Disabled_2x.png"
var DISABLED_PRIMARY_BACKGROUND = "Background_Disabled_Primary_2x.png"
var DISABLED_SECONDARY_BACKGROUND = "Background_Disabled_Secondary_2x.png"

var LARGE_MARGINAL_BACKGROUND = "Background_Large_Marginal_2x.png"
var LARGE_FAIL_BACKGROUND = "Background_Large_Fail_2x.png"
var LARGE_SEVERE_BACKGROUND = "Background_Large_Severe_2x.png"
var LARGE_TRANSITIONAL_BACKGROUND = "Background_Large_Transitional_2x.png"
var LARGE_UNKNOWN_BACKGROUND = "Background_Large_Unknown_2x.png"
var LARGE_DISABLED_BACKGROUND = "Background_Large_Disabled_2x.png"

var g_lPanePadding = 4;

function NavigateToDashboard(DashboardItemName)
{
    try
    {
        var entFilter = window.parent.document.getElementById("divEntityFilter");
        if (entFilter)
            entFilter.dashboardSelection = DashboardItemName;
    }
    catch( err ) {}
}

function SelectCompNode(node, faceplateLink, selectionBox)
{
    var divCurrentSelection = document.getElementById("divCurrSelection");
    if (divCurrentSelection && (divCurrentSelection.selection !== undefined) && (divCurrentSelection.compItem !== undefined))
    {
        if (divCurrentSelection.compItem !== null)
        {
            // Another compItem is current selected, deselect it first
            divCurrentSelection.compItem.selected = "false";
            divCurrentSelection.compItem = null;
        }
		
        divCurrSelection.selection = node;
		faceplateLink.selected = "true";
        divCurrSelection.compItem = faceplateLink;
		
        window.event.cancelBubble = true;
		selectionBox.style.visibility = "visible";
    }
}

function DeselectCompNode(selectionBox)
{
	selectionBox.style.visibility = "hidden";	
}

function SelectShape(tooltipElem, selectionBox, filterString)
{
    var divCurrentSelection = document.getElementById("divCurrSelection");
	
	if(GetCompositeMenuVersion() > 1)
	{
		HideDetailedCompositeMenu();
	}
    
    if (divCurrentSelection && (divCurrentSelection.selection !== undefined) && (divCurrentSelection.shapeGroup !== undefined))
    {
        if (divCurrentSelection.shapeGroup !== null)
        {
            // Another shapeGroup is current selected, deselect it first
            divCurrentSelection.shapeGroup.selected = "false";
            divCurrentSelection.shapeGroup = null;
        }
		
        if (divCurrentSelection.compItem !== undefined && divCurrentSelection.compItem !== null)
        {
            //A composite item is selected, deselect it when a new shape is selected
            divCurrentSelection.compItem.selected = "false";
            divCurrentSelection.compItem = null;			
		}
	
        divCurrSelection.selection = filterString;

        // Set the tooltip element of this shape as the selected shapeGroup
        tooltipElem.selected= "true";
        divCurrSelection.shapeGroup = tooltipElem; 
        
        window.event.cancelBubble = true;
        selectionBox.style.visibility = "visible";
    }
}

function DeselectShape(selectionBox)
{
    selectionBox.style.visibility = "hidden";
}

function GetSysdspPath(sysDspElem)
{
    // trigger a sysdsp path update
    var systemSummaryPath = window.external.parent.FindFile("sysSystemSummary.htm");
    sysDspElem.value = systemSummaryPath.substr(0, systemSummaryPath.lastIndexOf("\\") + 1);
}

// redundant, 0=none, 1=primary, 2=backup
function DetermineGraphicIconFile(pointType, pointSubType, itemNumber, redundant, virtual)
{
    if (redundant == undefined)
    {
        redundant = 0;
    }
	
	if (virtual == undefined)
	{
		virtual = false;
	}

    var imageFileName = "Unknown.png";
    
    switch (pointType){
    case em_type_SYS:
        if (pointSubType == em_stype_SYS_MODEL)
        {
            switch (itemNumber){
            case em_inum_SYS_MODEL_ASSETS:
                return "node_sys_mdl_assets.png";
            case em_inum_SYS_MODEL_ALARMGROUPS:
                return "node_sys_mdl_alarmgroups.png";
            case em_inum_SYS_MODEL_SYSTEMALARMGROUPS:
                return "node_sys_mdl_alarmgroups.png";
            case em_inum_SYS_MODEL_SYSTEM:
                return "node_sys_mdl_system.png";
            case em_inum_SYS_MODEL_CANE:
                return "node_sys_mdl_cane.png";
            case em_inum_SYS_MODEL_ORGANIZATIONS:
                return "node_sys_mdl_organization.png";
            default:
                break;
            }
        }
        
        if ((pointSubType >= em_stype_SYS_SERVER) && (pointSubType <= em_stype_SYS_METER))
        {
            imageFileName = "sys" + pointSubType + ".png";
        }
		
		if (pointSubType == em_stype_SYS_LCN || pointSubType == em_stype_SYS_GATEWAYS)
		{
            imageFileName = "node_sys_tps_" + pointSubType + ".png"
		}
        break;

    case em_type_CDA:
        redun_text = "";
        if (redundant)
        {
            redun_text = "_redun";
            if(redundant == 3)
            {
                redun_text += "_backup";
            }
        }
        imageFileName = "cda" + pointSubType + redun_text + ".png";
        break;
    
    case em_type_RDA:
        imageFileName = "rda" + pointSubType + ".png";
        break;
    
    case em_type_PSA:
        if((pointSubType >= em_stype_PSA_TPS_LOW) && (pointSubType <= em_stype_PSA_TPS_HIGH))
        {
			if(virtual == true)
			{
				imageFileName = "node_sys_tps_" + pointSubType + "_virtual.png"
			}
			else
			{
				imageFileName = "node_sys_tps_" + pointSubType + ".png"
			}
        }
		else if((pointSubType >= em_stype_PSA_TPS_IOP_LOW) && (pointSubType <= em_stype_PSA_TPS_IOP_HIGH))
		{
			if(redundant)
			{
				imageFileName = "node_sys_tps_iop_redun.png"
			}
			else
			{
				imageFileName = "node_sys_tps_iop.png"	
			}
		}
        else
        {
            imageFileName = "psa" + pointSubType + ".png";
        }
        break;
    
    case em_type_CLH:
        if (pointSubType == em_stype_CLH_SYSALMGRP)
            imageFileName =  "dashboardLink.png";    
        break;

    default:
        break;
    }

    return imageFileName;
}

function TransformAlarmState(serverAlarmFlag)
{
    var lStationAlarmState = 0;

    if (serverAlarmFlag & PNTALM_INALM_MASK)
        lStationAlarmState |= ALARM_STATUS;

    if (serverAlarmFlag & PNTALM_UNACKN_MASK)
        lStationAlarmState |= ALARM_ACK_REQ;
    else
        lStationAlarmState |= ALARM_ACK;

    if (serverAlarmFlag & PNTALM_SHELVED_MASK)
        lStationAlarmState |= ALARM_SHELVED;

    if (serverAlarmFlag & PNTALM_SUPPRESSED_MASK)
        lStationAlarmState |= ALARM_SUPPRESSED;

    switch ((serverAlarmFlag & PNTALM_ALMPRI_MASK) >> PNTALM_ALMPRI_BIT){
    case 1:
        lStationAlarmState |= ALARM_LOW;
        break;
    case 2:
        lStationAlarmState |= ALARM_HIGH;
        break;
    case 3:
        lStationAlarmState |= ALARM_URGENT;
        break;
    case 4:
        lStationAlarmState |= ALARM_CRITICAL;
        break;
    }

    return lStationAlarmState;
}

function AggregateAlarmState(previousState, newState)
{
    var previousPriority = previousState & 0xf0;
    var newPriority = newState & 0xf0;

    // At least one state is in alarm and unacked
    if ( (previousState & (ALARM_STATUS|ALARM_ACK_REQ))== (ALARM_STATUS|ALARM_ACK_REQ) && 
         (newState & (ALARM_STATUS|ALARM_ACK_REQ)) == (ALARM_STATUS|ALARM_ACK_REQ) )
    {
        return (ALARM_STATUS | ALARM_ACK_REQ | Math.max(previousPriority, newPriority));
    }
    else if ((previousState & (ALARM_STATUS|ALARM_ACK_REQ)) == (ALARM_STATUS|ALARM_ACK_REQ))
    {
        return previousState;
    }
    else if ((newState & (ALARM_STATUS|ALARM_ACK_REQ)) == (ALARM_STATUS|ALARM_ACK_REQ))
    {
        return newState;
    }

    // At least one state is unacked
    if ((previousState & ALARM_ACK_REQ) && (newState & ALARM_ACK_REQ))
    {
        return (ALARM_ACK_REQ | Math.max(previousPriority, newPriority));
    }
    else if (previousState & ALARM_ACK_REQ)
    {
        return previousState;
    }
    else if (newState & ALARM_ACK_REQ)
    {
        return newState;
    }

    // At least one state is in alarm
    if ((previousState & ALARM_STATUS) && (newState & ALARM_STATUS))
    {
        return (ALARM_STATUS | ALARM_ACK | Math.max(previousPriority, newPriority));
    }
    else if (previousState & ALARM_STATUS)
    {
        return previousState;
    }
    else if (newState & ALARM_STATUS)
    {
        return newState;
    }

    // At least one state is shelved
    if ((previousState & ALARM_SHELVED) || (newState & ALARM_SHELVED))
    {
        return ALARM_SHELVED;
    }

    // At least one state is suppressed
    if ((previousState & ALARM_SUPPRESSED) || (newState & ALARM_SUPPRESSED))
    {
        return ALARM_SUPPRESSED;
    }
    
    return previousState;
}

function AggregateStatus(previousStatus, newStatus)
{
    // fix up status if newStatus happens to be an invalid param binding
    if ((!newStatus) || isNaN(newStatus) )
            newStatus = 1;
    
    if (previousStatus == STATUS_FAILED || newStatus == STATUS_FAILED)
        return STATUS_FAILED;
    else if (previousStatus == STATUS_MARGINAL || newStatus == STATUS_MARGINAL)
        return STATUS_MARGINAL;
    else if (previousStatus == STATUS_DISABLED || newStatus == STATUS_DISABLED)
        return STATUS_DISABLED;
    else if (previousStatus == STATUS_SIMULATION || newStatus == STATUS_SIMULATION)
        return STATUS_SIMULATION;
    else if ( (previousStatus < STATUS_BLANK) || (previousStatus > STATUS_SIMULATION) || (newStatus < STATUS_BLANK) || (newStatus > STATUS_SIMULATION) )
        return STATUS_UNKNOWN;
    else  if (previousStatus == STATUS_OK || newStatus == STATUS_OK)
        return STATUS_OK;
    else
        return STATUS_BLANK;
}

function OnIFrameFilterChange()
{
    try
    {
        if( window.event.propertyName == 'selection' )
        {
            var entFilter = window.parent.document.getElementById("divEntityFilter");
            if (entFilter)
                entFilter.dashboardFilterString = divCurrSelection.selection;
        }
    }
    catch( err ) {}
}

function ClearCurrSelection(updateFilter)
{
    try
    {
        if(divCurrSelection.shapeGroup)
        {
            divCurrSelection.shapeGroup.selected = 'false'; 
            divCurrSelection.shapeGroup = null;
        }
		
		if(divCurrSelection.compItem !== undefined && divCurrSelection.compItem !== null)
		{
			divCurrSelection.compItem.selected = 'false'; 
			divCurrSelection.compItem = null;
						
		}

        if (updateFilter)
        {
            var histArray = window.parent.g_DashboardHistoryArray;
            if (histArray && (histArray.length > 0)) 
                divCurrSelection.selection = histArray[histArray.length-1];
            else
                divCurrSelection.selection = '';
        }
    }
    catch( err ) {}
}

function SetDetailedCompositeMenuBindings(strAuxPointList, strAuxTitleList, strAuxStatusList, strShowDesc)
{
    try
    {	
        compositeMenu_pointList.stringValue = strAuxPointList;
		compositeMenu_statusList.stringValue = strAuxStatusList;
		compositeMenu_titleList.stringValue = strAuxTitleList;
		compositeMenu_showDescription.stringValue = strShowDesc;
	
    }
    catch(err)
    {
		alert(err.message);
        window.external.logMessage("SetDetailedCompositeMenuBindings(" + strAuxPointList + "," + strAuxTitleList + "," + strAuxStatusList + ") -- error: " + err.message);
    }
}

function GetCompositeMenuVersion()
{
	if(typeof compositeMenu != 'undefined')
	{
		if(typeof compositeMenu_version != 'undefined')
		{
			return parseFloat(compositeMenu_version.value);
		}
		else
		{
			return 1; //If no compositeMenu version exists, assume it is version 1
		}
	}
	else
	{
		return -1; //No composite menu exists
	}
}

function ShowDetailedCompositeMenu(shapeElem)
{
    try
    {
        if (!(divCurrSelection.compositeMenuShape === ""))
        {
            divCurrSelection.compositeMenuShape.selected = "false";
            divCurrSelection.compositeMenuShape = "";
        }
		
		if(compositeMenu_showDescription.value == "TRUE")
		{
			compositeMenu_description.style.top = parseInt(compositeMenu_menuBackground.style.top, 10) + parseInt(compositeMenu_menuBackground.style.height, 10) - 2;
			compositeMenu_menuBackground.style.height = parseInt(compositeMenu_menuBackground.style.height) + 15;
			compositeMenu_description.style.visibility = "inherit";
		}
		else
		{
			compositeMenu_description.style.visibility = "hidden";
		}
		
        // calculate menu position depending on calling shape position
        var callingShape = shapeElem.parentElement.parentElement;
        var newTop = parseInt(callingShape.style.top, 10) + parseInt(callingShape.style.height, 10) - parseInt(compositeMenu_menuBackground.style.height) - 5;
		var newLeft = parseInt(callingShape.style.left, 10) + parseInt(callingShape.style.width, 10) - 5;

        if ( (parseInt(Page.style.width, 10) - newLeft) < parseInt(compositeMenu.style.width))
        {
            // running out of room for composite menu on right, move it to left of shape
            newLeft = parseInt(callingShape.style.left) - parseInt(compositeMenu.style.width) + 2;
        }
            
		if (newTop < 0)
        {
            // running out of room for composite menu at top, move so it's flush with top of page 
            newTop = 0;
        }
		
		if(newTop > (parseInt(Page.style.height, 10) - parseInt(compositeMenu_menuBackground.style.height)))
		{
			newTop = (parseInt(Page.style.height, 10) - parseInt(compositeMenu_menuBackground.style.height));
		}
		
        compositeMenu.style.left = newLeft;
        compositeMenu.style.top = newTop;
        compositeMenu.style.zIndex= 50;
        compositeMenu.style.visibility = "inherit";
        divCurrSelection.compositeMenuShape = shapeElem;
    }
    catch (err)
    {
        window.external.logMessage("ShowDetailedCompositeMenu() -- error: " + err.message);
    }
}

function HideDetailedCompositeMenu()
{
    try
    {
        compositeMenu_pointList.stringValue = "";
		compositeMenu_statusList.stringValue = "";
		compositeMenu_titleList.stringValue = "";
        compositeMenu.style.visibility = "hidden";
        if (!(divCurrSelection.compositeMenuShape === ""))
        {
            divCurrSelection.compositeMenuShape.selected = "false";
            divCurrSelection.compositeMenuShape = "";
        }
    }
    catch (err)
    {
        window.external.logMessage("HideDetailedCompositeMenu() -- error: " + err.message);
    }
}

function SetCompositeMenuBindings(strAuxPointList)
{
    try
    {
        compositeMenu_pointList.stringValue = strAuxPointList;
    }
    catch(err)
    {
        window.external.logMessage("SetCompositeMenuBindings(" + strAuxPointList + ") -- error: " + err.message);
    }
}

function HideCompositeMenu()
{
    try
    {
        compositeMenu_pointList.stringValue = "";
        compositeMenu.style.visibility = "hidden";
        if (!(divCurrSelection.compositeMenuShape === ""))
        {
            divCurrSelection.compositeMenuShape.selected = "false";
            divCurrSelection.compositeMenuShape = "";
        }
    }
    catch (err)
    {
        window.external.logMessage("HideCompositeMenu() -- error: " + err.message);
    }
}

function ShowCompositeMenu(shapeElem)
{
    try
    {
        if (!(divCurrSelection.compositeMenuShape === ""))
        {
            divCurrSelection.compositeMenuShape.selected = "false";
            divCurrSelection.compositeMenuShape = "";
        }
        // calculate menu position depending on calling shape position
        var callingShape = shapeElem.parentElement.parentElement;
        var newTop = parseInt(callingShape.style.top, 10) + parseInt(callingShape.style.height, 10);
        var newLeft = parseInt(callingShape.style.left, 10) + parseInt(callingShape.style.width, 10);

        if ( (parseInt(Page.style.width, 10) - newLeft) < 135)
        {
            // running out of room for composite menu on right, move it to left of shape 
            newLeft = parseInt(callingShape.style.left, 10) - 135;
        }
            
        if ( (parseInt(Page.style.height, 10) - newTop) < 180)
        {
            // running out of room for composite menu on bottom, move so it's flush with bottom of page 
            newTop = parseInt(Page.style.height, 10) - (compositeMenu_pointList.stringValue.split(";").length - 1) * 25 - 10;
        }

        compositeMenu.style.left = newLeft;
        compositeMenu.style.top = newTop;
        compositeMenu.style.zIndex= 50;
        compositeMenu.style.visibility = "inherit";
        divCurrSelection.compositeMenuShape = shapeElem;
    }
    catch (err)
    {
        window.external.logMessage("ShowCompositeMenu() -- error: " + err.message);
    }
}

function ClickBlank()
{
    HideCompositeMenu();
    ClearCurrSelection(true);
}

// Check if the content of the text box is more than what it can show. 
// If so, expand the text box to new size, allow text wrapping and clip excess text with ellipsis.
function checkTextBoxOverflow(txtBoxElem, newLeft, newTop, newWidth, newHeight, alignment)
{
    //if text overflows, make text box two lines and change to left text alignment, allow word wrap within word
    if ((txtBoxElem.scrollWidth > txtBoxElem.clientWidth) ||
        (txtBoxElem.scrollHeight > txtBoxElem.clientHeight))
    {
        txtBoxElem.style.left = newLeft;
        txtBoxElem.style.top = newTop;
        txtBoxElem.style.height = newHeight;
        txtBoxElem.style.width = newWidth;
        txtBoxElem.style.textAlign = alignment;
        txtBoxElem.style.wordWrap = "break-word";
    }
    
    // use timeout to allow the UI to update to the new sizes set above
    setTimeout(function(){
        //if text still overflows, add ellipsis at the end.
        while ((txtBoxElem.scrollWidth > txtBoxElem.clientWidth) ||
            (txtBoxElem.scrollHeight > txtBoxElem.clientHeight))
        {
            var strContent = txtBoxElem.value;
            txtBoxElem.value = strContent.substr(0, strContent.length - 4) + "...";
        }
    }, 0);
}

function ResizeDashboardIframe()
{
	var headerHeight = window.parent.document.getElementById("DashboardTitlebar").clientHeight;
	var SystemStatusPane = window.parent.document.getElementById("SystemStatusPane");
    var maxWidth = SystemStatusPane.clientWidth;
    var maxHeight = SystemStatusPane.clientHeight - headerHeight;
    var width = maxWidth - g_lPanePadding;
    var height = maxHeight - g_lPanePadding;
	
	var divDashboardIframeWrapper = window.parent.document.getElementById("divDashboardIframeWrapper");
    divDashboardIframeWrapper.style.pixelHeight = height;
    divDashboardIframeWrapper.style.pixelWidth = width;

	divDashboardIframeWrapper.resized = "true";
}

function ZoomDashboard()
{
	var d=document.getElementById("Page");
	var zoomWidth = parseInt(document.body.clientWidth,10) / parseInt(d.style.width,10);
	var zoomHeight = parseInt(document.body.clientHeight,10) / parseInt(d.style.height,10);
	if(zoomWidth < zoomHeight)
	{
		zoom = zoomWidth
	}
	else
	{
		zoom = zoomHeight
	}
	d.style.zoom=zoom;
	d=document.getElementById("Background");
	d.style.zoom=zoom;	
}

function resetResized()
{
	var divDashboardIframeWrapper = window.parent.document.getElementById("divDashboardIframeWrapper");
	divDashboardIframeWrapper.resized = "false";	
}

function AttachBlinkEvent(functionToAttach)
{
	var page = document.getElementById("Page")
	page.attachEvent("OnBlink", functionToAttach);
}

function DetachBlinkEvent(functionToDetach)
{
	var page = document.getElementById("Page")
	page.detachEvent("OnBlink", functionToDetach);
}

function BlinkStatus(iconElement)
{
	if(iconElement.blinkStatus == "true")
	{
		blinkstate = window.event.getAttribute("OnBlink", 0);
		if(blinkstate == 0)
		{
			iconElement.src = iconElement.iconImageBlinkPath;	
		}
		else if(blinkstate == -1)
		{
			iconElement.src = iconElement.iconImagePath;
		}	
	}
}

function GetDefaultStatusColor()
{
	statusColor = "#000000";
	try
    {
        var colorTheme = window.external.application.ColorTheme;
        switch ( colorTheme )
        {
            case 2:
                themeFilePath = systemSummaryPath + "/Styles/EPKS_SystemDashboard_Light.css";
				statusColor = "#000000";
                break;
            case 1:
                themeFilePath = systemSummaryPath + "/Styles/EPKS_SystemDashboard_Dark.css";
				statusColor = "#FFFFFF";
                break;
            default:
                break;
        }
    }
    catch (e)
    {
    }
	
	return statusColor;
}

function GetOffStatusColor() {
	statusColor = "#000000";
	try {
		var colorTheme = window.external.application.ColorTheme;
		switch (colorTheme) {
			case 2:
				statusColor = "#F0F0F0";
				break;
			case 1:
				statusColor = "#E0E0E0";
				break;
			default:
				break;
		}
	}
	catch (e) {
	}

	return statusColor;
}

function DetermineStatusImages(status, redun)
{
	if (redun === undefined) {
        fruit = "N";
    }
	
	statusNonBlinkIcon = BLANK;
	statusBlinkIcon = BLANK;
	backgroundStatusImage = BLANK;
	largeBackgroundStatusImage = BLANK;
	
	//Ensure status is a number
	status = parseInt(status);
	
	//0-Unknown, 1 - Unknown, 2 - Off/Disabled, 3 - Fail, 4 - Marginal, 5 - OK, 7 - Transitional, 8 - Severe
	switch(status)
	{
		case 0:
			statusNonBlinkIcon = UNKNOWN_ICON;
			statusBlinkIcon = UNKNOWN_BLINK_ICON;
			largeBackgroundStatusImage = LARGE_UNKNOWN_BACKGROUND;
			
			if(redun == "P")
			{
				backgroundStatusImage = UNKNOWN_PRIMARY_BACKGROUND
			}
			else if(redun == "S")
			{
				backgroundStatusImage = UNKNOWN_SECONDARY_BACKGROUND
			}
			else 
			{
				backgroundStatusImage = UNKNOWN_BACKGROUND;	
			}
			break;
		case 1:
			break;
		case 2:
			statusNonBlinkIcon = DISABLED_ICON;
			largeBackgroundStatusImage = LARGE_DISABLED_BACKGROUND;
			
			if(redun == "P")
			{
				backgroundStatusImage = DISABLED_PRIMARY_BACKGROUND
			}
			else if(redun == "S")
			{
				backgroundStatusImage = DISABLED_SECONDARY_BACKGROUND
			}
			else 
			{
				backgroundStatusImage = DISABLED_BACKGROUND;
			}
			break;
		case 3: 
			statusNonBlinkIcon = FAIL_ICON;
			statusBlinkIcon = FAIL_BLINK_ICON;
			largeBackgroundStatusImage = LARGE_FAIL_BACKGROUND;

			if(redun == "P")
			{
				backgroundStatusImage = FAIL_PRIMARY_BACKGROUND
			}
			else if(redun == "S")
			{
				backgroundStatusImage = FAIL_SECONDARY_BACKGROUND
			}
			else
			{
				backgroundStatusImage = FAIL_BACKGROUND;
			}
			break;
		case 4: 
			statusNonBlinkIcon = MARGINAL_ICON;
			statusBlinkIcon = MARGINAL_BLINK_ICON;
			largeBackgroundStatusImage = LARGE_MARGINAL_BACKGROUND;
			
			if(redun == "P")
			{
				backgroundStatusImage = MARGINAL_PRIMARY_BACKGROUND
			}
			else if(redun == "S")
			{
				backgroundStatusImage = MARGINAL_SECONDARY_BACKGROUND
			}
			else
			{
				backgroundStatusImage = MARGINAL_BACKGROUND;
			}
			break;
		case 5: 
			statusNonBlinkIcon = OK_ICON;
			break;
		case 7:
			statusNonBlinkIcon = TRANSITIONAL_ICON;
			statusBlinkIcon = TRANSITIONAL_BLINK_ICON;
			largeBackgroundStatusImage = LARGE_TRANSITIONAL_BACKGROUND;
			
			if(redun == "P")
			{
				backgroundStatusImage = TRANSITIONAL_PRIMARY_BACKGROUND
			}
			else if(redun == "S")
			{
				backgroundStatusImage = TRANSITIONAL_SECONDARY_BACKGROUND
			}
			else
			{
				backgroundStatusImage = TRANSITIONAL_BACKGROUND;
			}
			break;
		case 8:
			statusNonBlinkIcon = SEVERE_ICON;
			statusBlinkIcon = SEVERE_BLINK_ICON;
			largeBackgroundStatusImage = LARGE_SEVERE_BACKGROUND;
			
			if(redun == "P")
			{
				backgroundStatusImage = SEVERE_PRIMARY_BACKGROUND
			}
			else if(redun == "S")
			{
				backgroundStatusImage = SEVERE_SECONDARY_BACKGROUND
			}
			else
			{
				backgroundStatusImage = SEVERE_BACKGROUND;
			}
			break;
	}
	
	return {
		nonblinkIcon: statusNonBlinkIcon,
		blinkIcon: statusBlinkIcon,
		background: backgroundStatusImage,
		largeBackground: largeBackgroundStatusImage
	};
}

function ConvertStatusTextToNumber(statusText)
{
	//0-Unknown, 1 - Blank, 2 - Off/Disabled, 3 - Fail, 4 - Marginal, 5 - OK, 7 - Transitional, 8 - Severe
	if(typeof statusText === 'string' || statusText instanceof String)
	{
		upperCaseStatus = statusText.toUpperCase();
	}
	else
	{
		return 0;
	}
	
	switch(upperCaseStatus)
	{
		case "NONE":
			return 1;
			break;
		case "OFF":
		case "NOTCONF":
		case "UNAVL":
		case "UNAVAIL":
		case "NOTCONFG":
			return 2;
			break;
		case "FAIL":
		case "FAILURE":
		case "FAILED":
		case "OFFNET":
		case "SOFTFAIL":
		case "IDLESF":
		case "COMMERR":
		case "BSF":
			return 3;
			break;
		case "MARGINAL":
		case "WARN":
		case "IOIDL":
		case "IDLE":
		case "ALIVE":
		case "S_IOIDL":
		case "S_IDLE":
		case "UPGRADE":
		case "SUSPCT": //LCN COMM Status
		case "????": //LCN COMM Status
		case "BACKUP_WARN": //IOP A and B Backup status when primary is in idle state
			return 4;
			break;
		case "OK":
		case "BACKUP":
		case "S_OK":
			return 5;
			break;
		case "QUALIF":
		case "PWR_ON":
		case "NET_LD":
		case "LOC_LD":
		case "TEST":
		case "READY":
		case "UPGRADE":
		case "LOADING":
		case "TEST":
		case "TESTSF":
		case "STANDBY":
		case "S_PAUSE":
		case "STANDBSF":
		case "POWERON":
			return 7;
			break;
		case "SEVERE":
		case "BKUP_PF":
		case "PF_IOIDL":
		case "PF_IDLE":
		case "PARTFAIL":
		case "NOSYNCH":
		case "CONFGMIS":
		case "UNAVAIL":
		case "COMMFAIL":
		case "S_PFIOIL":
		case "S_PFIDLE":
		case "S_PTFAIL":
		case "UPGRADESF":
			return 8;
			break;
		case "ISOL":
		case "UNKNOWN":
		case "NONEXIST":
		case "NORESP":
			return 0;
			break;
		default:
			return 0;
	}
}

/*** ELCN Specific Functions ***/

function ShowLcnRedundancyBorder(redundancyBorder, redundancy, nodeActState)
{
	if(redundancy == "P" && nodeActState == "RUNNING")
	{
		redundancyBorder.style.visibility = "visible";
		redundancyBorder.style.border = "1px solid #808080";
	}
	else if(redundancy == "S" || redundancy == "P")
	{
		redundancyBorder.style.visibility = "visible";
		redundancyBorder.style.border = "1px dashed #808080";
	}
	else
	{
		//Hide if value is "N", unknown or invalid
		redundancyBorder.style.visibility = "hidden";
	}	
}

function ShowPeripheralStatus(periphSts)
{
	if(periphSts == "SERVICE" || periphSts == "FAIL")
	{
		return true;
	}
	
	return false;
}

function ShowHstColl(hstColl)
{
	if(hstColl == "Y")
	{
		return true;
	}
	
	return false;
}

