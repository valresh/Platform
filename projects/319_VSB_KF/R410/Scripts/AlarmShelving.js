var ALMSHELVING_DISABLED	= 2;
var ALMSHELVING_INVALID 	= 1;
var ALMSHELVING_ENABLED 	= 0;
var m_lShelvingDialogWidth = 320;
var m_lShelvingDialogHeight = 420;
var g_lAlmShelvingDisabled = ALMSHELVING_ENABLED;
var g_bAlmShelvingDlgReqd = false;
var g_bServerAvailable = true;
var g_ShelvingDialog;
var g_strClosingText = "Shelving dialog closed due to ";
var g_nPriority = -1;
var NO_ROW_SELECTED = -1;
var SHELVE_NONE = 0;
var SHELVE_ALARM = 1;
var SHELVE_LOCATION = 2;
var g_bShelveFrom = SHELVE_NONE;
var g_ValidationTimer = -1;
var g_ValidationTimerSet = false;
var PRIORITY_JOURNAL = 0;
var PRIORITY_LOW = 16;
var PRIORITY_HIGH = 32;
var PRIORITY_URGENT = 48;
function OnDisplayShelvingDialog()
{
	var src = "Src";
	var cdn = "Cdn";
	var desc = "Desc";
	var rtn = false;
	var nSelectedRow = GetSelectedRow();
	// Check there's something selected...
	if (nSelectedRow < 0)
	{
		window.external.application.TemporaryMessageZoneText = g_strClosingText + "selected alarm out of view.";
		return;
    }
    CloseShelvingDialog("new dialog opened");
	src = Table.Fields.Item("Source").GetValue(nSelectedRow);
	if (src == undefined)
		src = "";
	cdn = Table.Fields.Item("ConditionName").GetValue(nSelectedRow);
	if (cdn == undefined)
		cdn = "";
	desc = Table.Fields.Item("Description").GetValue(nSelectedRow);
	if (desc == undefined)
		desc = "";
	rtn = Table.Fields.Item("RTN").GetValue(nSelectedRow);
	if (rtn == undefined)
		rtn = false;
	// For the alert summary, where there's no priority...
	if (g_SummaryType != SUMMARY_ALERT)
	{
		g_nPriority = Table.Fields.Item("Priority").GetValue(nSelectedRow);
	}
	else
	{
		g_nPriority = 0;
	}
	g_bShelveFrom = SHELVE_ALARM;
	var dialogURL = "Dialogs/AlarmShelvingDialog.htm?HscField=" + g_nPriority + "&Source=" + src + "&Condition=" + cdn + "&Description=" + desc + "&RTN=" + rtn;
	HDXPopupBehavior.CreatePopupWindow3(dialogURL, document.body.offsetWidth / 2 - m_lShelvingDialogWidth / 2, Table.style.pixelTop, m_lShelvingDialogWidth, m_lShelvingDialogHeight, 0, false, false);
	document.onshelvingdialogcomplete=function(ext){
		g_ShelvingDialog = ext;
	};
	document.oncloseshelvingdialog=function(bShelve, varParam){
		g_nPriority = -1;
        g_bShelveFrom = SHELVE_NONE;
        g_ShelvingDialog = null;
		document.oncloseshelvingdialog = null;
		if (bShelve)
			g_TDO.ShelveAlarm(varParam);
		else if (typeof(varParam) == "string" && varParam != "")
			window.external.application.TemporaryMessageZoneText = g_strClosingText + varParam + ".";
	};
}
function OnDisplayLocationShelvingDialog() {
    var entityName = "EntityName";
    var notifCount = "";
    var nSelectedRow = GetLocationSelectedRow();
    // Check there's something selected...
    if (nSelectedRow < 0) {
        window.external.application.TemporaryMessageZoneText = g_strClosingText + "selected location out of view.";
        return;
    }
    CloseShelvingDialog("new dialog opened");
    entityName = AreaTable.Fields.Item("EntityName").GetValue(nSelectedRow);
    var fullName = AreaTable.Fields.Item("FullName").GetValue(nSelectedRow);
    if (entityName == undefined)
        entityName = "";
    if (g_SummaryType == SUMMARY_ALERT) {
        var activeAlerts = AreaTable.Fields.Item("TotalActiveAlerts(%StationNumber%)").GetValue(nSelectedRow);
        var rtnAlerts = AreaTable.Fields.Item("TotalRTNAlerts(%StationNumber%)").GetValue(nSelectedRow);
        var totalDisabledAlerts = AreaTable.Fields.Item("TotalDisabledAlerts(%StationNumber%)").GetValue(nSelectedRow);
        if ((activeAlerts == undefined) || (rtnAlerts == undefined) || (totalDisabledAlerts == undefined)) {
            notifCount = "";
        }
        else {
            notifCount = activeAlerts + rtnAlerts + totalDisabledAlerts;
        }
    } else if (g_SummaryType == SUMMARY_ALARM || g_SummaryType == SUMMARY_SYSTEM_ALARM) {
        var totalAlarms = AreaTable.Fields.Item("TotalAlarms(%StationNumber%)").GetValue(nSelectedRow);
        var totalDisabledAlarms = AreaTable.Fields.Item("TotalDisabledAlarms(%StationNumber%)").GetValue(nSelectedRow);
        if ((totalAlarms == undefined) || (totalDisabledAlarms == undefined)) {
            notifCount = "";
        }
        else
        {
            notifCount = totalAlarms + totalDisabledAlarms;
        }
    }
    g_nPriority = -1;
    if (g_SummaryType == SUMMARY_ALARM || g_SummaryType == SUMMARY_SYSTEM_ALARM) {
        if (g_SummaryType == SUMMARY_ALARM) {
            if (AreaTable.Fields.Item("TotalCriticalAlarms(%StationNumber%)").GetValue(nSelectedRow) > 0 ||
                AreaTable.Fields.Item("TotalUrgentAlarms(%StationNumber%)").GetValue(nSelectedRow) > 0) {
                g_nPriority = PRIORITY_URGENT;
            }
        }
        else {
            if (AreaTable.Fields.Item("TotalUrgentAlarms(%StationNumber%)").GetValue(nSelectedRow) > 0) {
                g_nPriority = PRIORITY_URGENT;
            }
        }
        if (g_nPriority != PRIORITY_URGENT) {
            if (AreaTable.Fields.Item("TotalHighAlarms(%StationNumber%)").GetValue(nSelectedRow) > 0) {
                g_nPriority = PRIORITY_HIGH;
            }
            else if (AreaTable.Fields.Item("TotalLowAlarms(%StationNumber%)").GetValue(nSelectedRow) > 0) {
                g_nPriority = PRIORITY_LOW;
            }
            else {
                g_nPriority = PRIORITY_JOURNAL;
            }
        }
    }
    else if (g_SummaryType == SUMMARY_ALERT) {
        // For the alert summary, where there's no priority...
        g_nPriority = 0;
    }
    g_bShelveFrom = SHELVE_LOCATION;
    var dialogURL = "Dialogs/AlarmShelvingDialog.htm?HscField=" + g_nPriority + "&EntityName=" + entityName + "&NotifCount=" + notifCount + "&FullName=" + fullName;
    HDXPopupBehavior.CreatePopupWindow3(dialogURL, document.body.offsetWidth / 2 - m_lShelvingDialogWidth / 2, Table.style.pixelTop, m_lShelvingDialogWidth, m_lShelvingDialogHeight, 0, false, false);
    document.onshelvingdialogcomplete = function (ext) {
        g_ShelvingDialog = ext;
    };
    document.oncloseshelvingdialog = function (bShelve, varParam) {
        g_nPriority = -1;
        g_bShelveFrom = SHELVE_NONE;
        g_ShelvingDialog = null;
        document.oncloseshelvingdialog = null;
        if (bShelve) {
            try 
            {
                g_TDO.ExecuteShelveLocation(varParam);
            } 
            catch (err)
            {
                window.external.application.logMessage("oncloseshelvingdialog - ExecuteShelveLocation() failed. err: " + err);
            }
        }
        else if (typeof (varParam) == "string" && varParam != "")
            window.external.application.TemporaryMessageZoneText = g_strClosingText + varParam + ".";
    };
}
function OnDisplayLocationUnshelvingDialog() {
    var notifCount = "";
    var nSelectedRow = GetLocationSelectedRow();
    // Check there's something selected...
    if (nSelectedRow < 0) {
        window.external.application.TemporaryMessageZoneText = g_strClosingText + "selected location out of view.";
        return;
    }
    CloseShelvingDialog("new dialog opened");
    var entityName = AreaTable.Fields.Item("EntityName").GetValue(nSelectedRow);
    if (entityName == undefined)
        entityName = "";
    if (g_SummaryType == SUMMARY_ALERT) {
        notifCount = AreaTable.Fields.Item("TotalShelvedAlerts(%StationNumber%)").GetValue(nSelectedRow);
        if (notifCount == undefined)
            notifCount = "";
    } else if (g_SummaryType == SUMMARY_ALARM || g_SummaryType == SUMMARY_SYSTEM_ALARM) {
        notifCount = AreaTable.Fields.Item("TotalShelvedAlarms(%StationNumber%)").GetValue(nSelectedRow);
        if (notifCount == undefined)
            notifCount = "";
    }
    g_bShelveFrom = SHELVE_LOCATION;
    var fullName = AreaTable.Fields.Item("FullName").GetValue(nSelectedRow);
    var dialogURL = "Dialogs/AlarmUnshelvingDialog.htm?EntityName=" + entityName + "&NotifCount=" + notifCount + "&FullName=" + fullName;
    HDXPopupBehavior.CreatePopupWindow3(dialogURL, document.body.offsetWidth / 2 - m_lShelvingDialogWidth / 2, Table.style.pixelTop, 300, 100, 0, false, false);
    document.onshelvingdialogcomplete = function (ext) {
        g_ShelvingDialog = ext;
    };
    document.oncloseshelvingdialog = function (bShelve, varParam) {
        g_nPriority = -1;
        g_bShelveFrom = SHELVE_NONE;
        g_ShelvingDialog = null;
        document.oncloseshelvingdialog = null;
        if (bShelve) {
            try {
                g_TDO.ExecuteUnshelveLocation(varParam);
            }
            catch (err) {
                window.external.application.logMessage("oncloseshelvingdialog - ExecuteUnshelveLocation() failed. err: " + err);
            }
        }
        else if (typeof (varParam) == "string" && varParam != "")
            window.external.application.TemporaryMessageZoneText = g_strClosingText + varParam + ".";
    };
}
function CloseShelvingDialog(strMessage)
{
	if (g_ShelvingDialog != null)
	{
		try // Failure in here means the dialog got the X button clicked
		{
			g_ShelvingDialog.close();
			if (strMessage.length > 0)
				window.external.application.TemporaryMessageZoneText = g_strClosingText + strMessage + ".";
			else
				window.external.application.TemporaryMessageZoneText = g_strClosingText + "operator security change.";
		}
		catch (err) {}
		g_nPriority = -1;
        g_bShelveFrom = SHELVE_NONE;
        g_ShelvingDialog = null;
		document.oncloseshelvingdialog = null;
	}
}
function OnAlarmShelvingDisabled(lDisabled)
{
	if (g_lAlmShelvingDisabled != lDisabled)
	{
		if (lDisabled != ALMSHELVING_ENABLED)
		{
			if (lDisabled == ALMSHELVING_INVALID && g_TDO.ShelvedAlarmSelected)
			{
                btnUnshelveAlarm.setAttribute("readonly", false);
				btnUnshelveAlarm.className='unshelveAlarm';
			}
			else
			{
                btnUnshelveAlarm.setAttribute("readonly", true);
				btnUnshelveAlarm.className='unshelveAlarmDisabled';
			}
            btnShelveAlarm.setAttribute("readonly", true);
			btnShelveAlarm.className='shelveAlarmDisabled';
            btnDialogShelveAlarm.setAttribute("readonly", true);
			btnDialogShelveAlarm.className='dialogShelveAlarmDisabled';
			if (lDisabled == ALMSHELVING_DISABLED)
			{
				lblShelvedLabel.style.visibility = "hidden";
				divShelvedFilteredTotal.style.visibility = "hidden";
				lblOfShelvedTotal.style.visibility = "hidden";
				divShelvedUnfilteredTotal.style.visibility = "hidden";
			}
		}
		else
		{
			var nSelectedRow = GetSelectedRow();
			if (nSelectedRow > NO_ROW_SELECTED)
			{
				if (g_TDO.ShelvedAlarmSelected)
				{
                    btnUnshelveAlarm.setAttribute("readonly", false);
					btnUnshelveAlarm.className='unshelveAlarm';
				}
                btnShelveAlarm.setAttribute("readonly", false);
				btnShelveAlarm.className='shelveAlarm';
                btnDialogShelveAlarm.setAttribute("readonly", false);
				btnDialogShelveAlarm.className='dialogShelveAlarm';
			}
			lblShelvedLabel.style.visibility = "visible";
			divShelvedFilteredTotal.style.visibility = "visible";
			lblOfShelvedTotal.style.visibility = "visible";
			divShelvedUnfilteredTotal.style.visibility = "visible";
			if (g_bAlmShelvingDlgReqd)
			{
                btnShelveAlarm.setAttribute("readonly", true);
				btnShelveAlarm.className='shelveAlarmDisabled';
			}
		}
		g_lAlmShelvingDisabled = lDisabled;
		CloseShelvingDialog("alarm shelving disabled");
	}
}
function OnAlarmShelvingDialogReqd(bDlgReqd)
{
	if (!g_lAlmShelvingDisabled)
	{
		if (g_bAlmShelvingDlgReqd != bDlgReqd)
		{
			if (bDlgReqd)
			{
                btnShelveAlarm.setAttribute("readonly", true);
				btnShelveAlarm.className='shelveAlarmDisabled';
			}
			else
			{
                btnShelveAlarm.setAttribute("readonly", false);
				btnShelveAlarm.className='shelveAlarm';
			}
			g_bAlmShelvingDlgReqd = bDlgReqd;
		}
	}
}
function OnSelectedAlarmShelvedState(bShelved)
{
	if (!g_lAlmShelvingDisabled)
	{
        if (bShelved && getReadOnlyAttr(btnUnshelveAlarm) == true)
		{
            btnUnshelveAlarm.setAttribute("readonly", false);
			btnUnshelveAlarm.className='unshelveAlarm';
		}
        if (!bShelved && getReadOnlyAttr(btnUnshelveAlarm) == false)
		{
            btnUnshelveAlarm.setAttribute("readonly", true);
			btnUnshelveAlarm.className='unshelveAlarmDisabled';
		}
		OnAlarmSelected("", false);
	}
	else if (g_lAlmShelvingDisabled == ALMSHELVING_INVALID)
	{
		if (g_TDO.ShelvedAlarmSelected)
		{
            btnUnshelveAlarm.setAttribute("readonly", false);
			btnUnshelveAlarm.className='unshelveAlarm';
		}
		else
		{
            btnUnshelveAlarm.setAttribute("readonly", true);
			btnUnshelveAlarm.className='unshelveAlarmDisabled';
		}
	}
}
// We only want the second parameter to determine whether to close the shelving dialog or not
function OnAlarmSelected(DetailsXML, bSelectionChange)
{
	if (!g_lAlmShelvingDisabled)
	{
		if (!g_ValidationTimerSet)
		{
			g_ValidationTimer = window.setInterval(ValidateShelvingSelection, 250);
			g_ValidationTimerSet = true;
		}
		if (bSelectionChange)
			CloseShelvingDialog("alarm selection change");
	}
}
function ValidateShelvingSelection()
{
	var nSelectedRow = GetSelectedRow();
    if (getReadOnlyAttr(btnShelveAlarm) == true && nSelectedRow > NO_ROW_SELECTED && g_lAlmShelvingDisabled == ALMSHELVING_ENABLED)
	{
        if (g_TDO.ShelvedAlarmSelected && getReadOnlyAttr(btnUnshelveAlarm) == true)
		{
            btnUnshelveAlarm.setAttribute("readonly", false);
			btnUnshelveAlarm.className='unshelveAlarm';
		}
		if (!g_bAlmShelvingDlgReqd)
		{
            btnShelveAlarm.setAttribute("readonly", false);
			btnShelveAlarm.className='shelveAlarm';
		}
        if (getReadOnlyAttr(btnDialogShelveAlarm) == true)
		{
            btnDialogShelveAlarm.setAttribute("readonly", false);
			btnDialogShelveAlarm.className='dialogShelveAlarm';
		}
	}
    else if (getReadOnlyAttr(btnShelveAlarm) == false && nSelectedRow < 0 && g_bShelveFrom == SHELVE_ALARM)
	{
        btnShelveAlarm.setAttribute("readonly", true);
		btnShelveAlarm.className='shelveAlarmDisabled';
        if (getReadOnlyAttr(btnDialogShelveAlarm) == false)
		{
            btnDialogShelveAlarm.setAttribute("readonly", true);
			btnDialogShelveAlarm.className='dialogShelveAlarmDisabled';
		}
        if (getReadOnlyAttr(btnUnshelveAlarm) == false)
		{
            btnUnshelveAlarm.setAttribute("readonly", true);
			btnUnshelveAlarm.className='unshelveAlarmDisabled';
		}
		CloseShelvingDialog("alarm out of view");
	}
	if (nSelectedRow > NO_ROW_SELECTED)
	{
		if (g_SummaryType != SUMMARY_ALERT)
		{
			if (g_bShelveFrom == SHELVE_ALARM && g_nPriority != Table.Fields.Item("Priority").GetValue(nSelectedRow) && g_nPriority != -1)
				CloseShelvingDialog("alarm priority changed");
		}
	}
}
function ClearShelvedSelection()
{
    if (getReadOnlyAttr(btnShelveAlarm) == true && getReadOnlyAttr(btnUnshelveAlarm) == true && getReadOnlyAttr(btnDialogShelveAlarm) == true)
    {
        return;
    }
	if (g_ValidationTimerSet)
	{
		clearInterval(g_ValidationTimer);
		g_ValidationTimerSet = false;
	}
    btnShelveAlarm.setAttribute("readonly", true);
	btnShelveAlarm.className='shelveAlarmDisabled';
    btnDialogShelveAlarm.setAttribute("readonly", true);
	btnDialogShelveAlarm.className='dialogShelveAlarmDisabled';
    btnUnshelveAlarm.setAttribute("readonly", true);
	btnUnshelveAlarm.className='unshelveAlarmDisabled';
	CloseShelvingDialog("no alarm selected");
}
function ShelvingErrorStatus(code)
{
	if( code == ERR_COMMENT_UNAVAILABLE )
	{
		g_bServerAvailable = false;
		CloseShelvingDialog("the server becoming unavailable");
	}
	else if( code == ERR_COMMENT_REFRESH )
	{
		g_bServerAvailable = true;
		CloseShelvingDialog("the server becoming available");
	}
}
function getReadOnlyAttr(element) {
    var attrVal = element.getAttribute("readonly");
    return attrVal == null ? false : (attrVal.toString() == "true");
}
